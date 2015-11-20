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
		return -1;
	}
    return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error code. 0 if no error
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{
    return 0;
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
