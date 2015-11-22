/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"


using namespace std;

int IND_INTEGER_SIZE = sizeof(int);
int IND_PAGE_ID_SIZE = sizeof(PageId);
int IND_ENTRY_PAIR_LEAF_NODE_SIZE = sizeof(RecordId) + IND_INTEGER_SIZE;
int IND_ENTRY_PAIR_NON_LEAF_NODE_SIZE = IND_PAGE_ID_SIZE + IND_INTEGER_SIZE;
int IND_MAX_KEYS_LEAF_NODE = floor((PageFile::PAGE_SIZE - IND_PAGE_ID_SIZE)/IND_ENTRY_PAIR_LEAF_NODE_SIZE);
int IND_MAX_KEYS_NON_LEAF_NODE = floor((PageFile::PAGE_SIZE - IND_PAGE_ID_SIZE)/IND_ENTRY_PAIR_NON_LEAF_NODE_SIZE);
const int IND_OVERFLOW = 1;
int ERROR_IN_INSERT = -1;
int OKAY = 0;

/*
 * BTreeIndex constructor
 */
BTreeIndex::BTreeIndex()
{
	rootPid = -1;
	treeHeight = 0;
	prev_page = -1;
	cursor_node = BTLeafNode();
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error code. 0 if no error
 */
RC BTreeIndex::open(const string& indexname, char mode)
{
	char buffer[PageFile::PAGE_SIZE];

	//checks if pagefile can open
	if (pf.open(indexname, mode) != 0)
	{
		fprintf(stderr, "Error opening pagefile\n");
		return -1;
	}
	
	//checks if pagefile is empty
	//that is, there is no b+ tree
	if (pf.endPid() <= 0)
	{
		rootPid = -1;
		treeHeight = 0;
		close();
		return pf.open(indexname, mode);
	}

	//pagefile is not empty
	//there must be an existing b+ tree
	else
	{
		//read first disk page into buffer
		if (pf.read(0, buffer) != 0)
		{
			fprintf(stderr, "Error reading pagefile\n");
			return -1;
		}
		memcpy(&rootPid, buffer, IND_PAGE_ID_SIZE);
		memcpy(&treeHeight, buffer + IND_PAGE_ID_SIZE, IND_INTEGER_SIZE);
	}
    return 0;
}

/*
 * Close the index file.
 * @return error code. 0 if no error
 */
RC BTreeIndex::close()
{
	char buffer[PageFile::PAGE_SIZE];
	memcpy(buffer, &rootPid, IND_PAGE_ID_SIZE);
	memcpy(buffer + IND_PAGE_ID_SIZE, &treeHeight, IND_INTEGER_SIZE);

	if (pf.write(0, buffer) != 0)
	{
		fprintf(stderr, "Error writing into pagefile\n");
		return -1;
	}
    return pf.close();
}

RC BTreeIndex::insert_helper(int& key, const RecordId& rid, int height, PageId currentPid, int& ikey, PageId& ipid)
{
	if (height = treeHeight)	//currently at a leaf node
	{
		BTLeafNode leaf = BTLeafNode();
		leaf.read(currentPid, pf);
		if(leaf.getKeyCount() < IND_MAX_KEYS_LEAF_NODE)	//there is room in the leaf node
		{
			leaf.insert(key, rid);
			leaf.write(currentPid, pf);
			return OKAY;
		}
		else	//we have to split
		{
			BTLeafNode sibling_node = BTLeafNode();
			leaf.insertAndSplit(key, rid, sibling_node, ikey);
			ipid = pf.endPid();
			sibling_node.setNextNodePtr(leaf.getNextNodePtr());
			leaf.setNextNodePtr(ipid);
			leaf.write(currentPid, pf);
			sibling_node.write(ipid, pf);
			fprintf(stderr, "We have to split due to overflow\n");
			return IND_OVERFLOW;
		}
	}
	
	else	//we are at a non leaf node
	{
		BTNonLeafNode non_leaf = BTNonLeafNode();
		non_leaf.read(currentPid, pf);
		
		PageId child_pid;
		non_leaf.locateChildPtr(key, child_pid);
		
		int res = insert_helper(key, rid, height + 1, child_pid, ikey, ipid);
		if(res == IND_OVERFLOW)	//overflow
		{
			if(non_leaf.getKeyCount() < IND_MAX_KEYS_NON_LEAF_NODE)
			{
				non_leaf.insert(ikey, ipid);
				non_leaf.write(currentPid, pf);
				ikey = -1;
				return OKAY;
			}
			else	//overflow
			{
				BTNonLeafNode sibling_node = BTNonLeafNode();
				int sibling_key;
				
				non_leaf.insertAndSplit(ikey, ipid, sibling_node, sibling_key);
				ikey = sibling_key;
				ipid = pf.endPid();
				sibling_node.write(ipid, pf);
				non_leaf.write(currentPid, pf);
				fprintf(stderr, "Error: insert_helper OVERFLOW\n");
				return IND_OVERFLOW;
			}
		}
		
		else
		{
			return res;	//either ERROR or OKAY
		}
	}
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
	if (treeHeight == 0)	//empty B+ Tree
	{
		BTLeafNode leaf = BTLeafNode();
		leaf.insert(key, rid);
		rootPid = pf.endPid();
		leaf.write(rootPid, pf);
		treeHeight++;
		return OKAY;
	}
	
	else
	{
		int ikey;
		PageId ipid;
		int res = insert_helper(key, rid, 1, rootPid, ikey, ipid);
		if(res == IND_OVERFLOW)
		{
			BTNonLeafNode parent = BTNonLeafNode();
			parent.initializeRoot(rootPid, ikey, ipid);
			rootPid = pf.endPid();
			parent.write(rootPid, pf);
			treeHeight++;
			return OKAY;
		}
		else
		{
			return res;	//either ERROR or OKAY
		}
	}
}

/**
 * Run the standard B+Tree key search algorithm and identify the
 * leaf node where searchKey may exist. If an index entry with
 * searchKey exists in the leaf node, set IndexCursor to its location
 * (i.e., IndexCursor.pid = PageId of the leaf node, and
 * IndexCursor.eid = the searchKey index entry number.) and return 0.
 * If not, set IndexCursor.pid = PageId of the leaf node and
 * IndexCursor.eid = the index entry immediately after the largest
 * index key that is smaller than searchKey, and return the error
 * code RC_NO_SUCH_RECORD.
 * Using the returned "IndexCursor", you will have to call readForward()
 * to retrieve the actual (key, rid) pair from the index.
 * @param key[IN] the key to find
 * @param cursor[OUT] the cursor pointing to the index entry with
 *                    searchKey or immediately behind the largest key
 *                    smaller than searchKey.
 * @return 0 if searchKey is found. Othewise an error code
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
	BTLeafNode leaf = BTLeafNode();
	BTNonLeafNode nonLeaf = BTNonLeafNode();

	PageId pid = rootPid;
	int eid = -1;

	//want to traverse through level if height of tree > 1
	if (treeHeight > 1)
	{
		int index = 0;
		while (index < treeHeight - 1)
		{
			if (nonLeaf.read(pid, pf) != 0)
			{
				fprintf(stderr, "Error: could not read nonLeaf\n");
				return -1;
			}

			if (nonLeaf.locateChildPtr(searchKey, pid) != 0)
			{
				fprintf(stderr, "Error: could not read nonLeaf child pointer\n");
				return -1;
			}
			index++;
		}
	}

	if (leaf.read(pid, pf) != 0)
	{
		return -1;
	}

	if (leaf.locate(searchKey, pid) != 0)
	{
		return -1;
	}

	cursor.pid = pid;
	cursor.eid = eid;

    return 0;
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error code. 0 if no error
 */
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
	if (prev_page != cursor.pid)
	{
		if (cursor_node.read(cursor.pid, pf) < 0)
		{
			return -1;
		}
		prev_page = cursor.pid;
	}
	
	if(cursor.eid == cursor_node.getKeyCount())
	{
		char buffer[IND_PAGE_ID_SIZE];
		memset(buffer, '\0', IND_PAGE_ID_SIZE);
		
		if(memcmp(buffer, &cursor.pid, IND_PAGE_ID_SIZE) == 0)
		{
			return -1;
		}
		
		cursor.pid = cursor_node.getNextNodePtr();
		cursor.eid = 0;
		if(cursor.pid == 0)
		{
			return -1;
		}
		if(cursor_node.read(cursor.pid, pf) < 0)
		{
			return -1;
		}
		prev_page = cursor.pid;
	}
	if (cursor_node.readEntry(cursor.eid, key, rid) < 0)
	{
		return -1;
	}
	else
	{
		cursor.eid++;
	}
	
	return 0;
}

void BTreeIndex::print()
{

}
