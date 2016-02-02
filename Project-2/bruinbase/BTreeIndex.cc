/*
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 */
 
#include "BTreeIndex.h"
#include "BTreeNode.h"
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <string.h>

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

PageId BTreeIndex::getRootPid()
{
	return rootPid;
}

int BTreeIndex::getTreeHeight()
{
	return treeHeight;
}

/*
 * Open the index file in read or write mode.
 * Under 'w' mode, the index file should be created if it does not exist.
 * @param indexname[IN] the name of the index file
 * @param mode[IN] 'r' for read, 'w' for write
 * @return error_occurred code. 0 if no error_occurred
 */

RC BTreeIndex::open(const string& indexname, char mode)
{	
	if(pf.open(indexname, mode) != 0)
		return -1;
	if(pf.endPid() == 0)
	{
		rootPid = -1;
		treeHeight = 0;
		if(pf.write(0, buffer) != 0)
			return -1;
		return 0;
	}
	
	if(pf.read(0, buffer) != 0)
		return -1;
	
	int pid;
	int height;
	memcpy(&pid, buffer, IND_INTEGER_SIZE);
	memcpy(&height, buffer + IND_INTEGER_SIZE, IND_INTEGER_SIZE);

	if(pid > 0 && height >= 0)
	{
		rootPid = pid;
		treeHeight = height;
	}

	return 0;
}

/*
 * Close the index file.
 * @return error_occurred code. 0 if no error_occurred
 */
RC BTreeIndex::close()
{
	memcpy(buffer, &rootPid, IND_INTEGER_SIZE);
	memcpy(buffer + IND_INTEGER_SIZE, &treeHeight, IND_INTEGER_SIZE);
	if(pf.write(0, buffer) != 0)
		return -1;
    return pf.close();
}

/*
 * Insert (key, RecordId) pair to the index.
 * @param key[IN] the key for the value inserted into the index
 * @param rid[IN] the RecordId for the record being inserted into the index
 * @return error_occurred code. 0 if no error_occurred
 */
RC BTreeIndex::insert(int key, const RecordId& rid)
{	
	if(key<0)
		return RC_INVALID_ATTRIBUTE;
	if(treeHeight == 0)
	{
		BTLeafNode new_tree;
		new_tree.insert(key, rid);
		if(pf.endPid() == 0)
			rootPid = 1;
		else
			rootPid = pf.endPid();
		treeHeight++;
		return new_tree.write(rootPid, pf);
	}
	int insertKey = -1;
	PageId insertPid = -1;
	if(insert_recursion(key, rid, 1, rootPid, insertKey, insertPid)!=0)
		return -1;
	
	return 0;
}

RC BTreeIndex::insert_recursion(int key, const RecordId& rid, int current_height, PageId pid_1, int& key_2, PageId& pid_2)
{
	RC error_occurred;
	key_2 = -1;
	pid_2 = -1;
	if(current_height == treeHeight)
	{
		BTLeafNode leaf_node;
		leaf_node.read(pid_1, pf);
		if(leaf_node.insert(key, rid) == 0)
		{	
			leaf_node.write(pid_1, pf);
			return 0;
		}
		BTLeafNode leaf_node_2;
		int key_3;
		error_occurred = leaf_node.insertAndSplit(key, rid, leaf_node_2, key_3);
		
		if(error_occurred != 0)
			return error_occurred;
		int end_pid = pf.endPid();
		key_2 = key_3;
		pid_2 = end_pid;

		leaf_node_2.setNextNodePtr(leaf_node.getNextNodePtr());
		leaf_node.setNextNodePtr(end_pid);

		error_occurred = leaf_node_2.write(end_pid, pf);
		
		if(error_occurred != 0)
			return error_occurred;
		
		error_occurred = leaf_node.write(pid_1, pf);
		
		if(error_occurred != 0)
			return error_occurred;
		
		if(treeHeight == 1)
		{
			BTNonLeafNode newRoot;
			newRoot.initializeRoot(pid_1, key_3, end_pid);
			treeHeight++;
			
			rootPid = pf.endPid();
			newRoot.write(rootPid, pf);
		}
		
		return 0;
	}
	else
	{
		BTNonLeafNode middle_node;
		middle_node.read(pid_1, pf);
		
		PageId pid_child = -1;
		middle_node.locateChildPtr(key, pid_child);
		
		int insertKey = -1;
		PageId insertPid = -1;
		
		error_occurred = insert_recursion(key, rid, current_height+1, pid_child, insertKey, insertPid);
		
		if(!(insertKey==-1 && insertPid==-1)) 
		{
			if(middle_node.insert(insertKey, insertPid)==0)
			{
				middle_node.write(pid_1, pf);
				return 0;
			}
		
			BTNonLeafNode middle_node_2;
			int key_3;
			
			middle_node.insertAndSplit(insertKey, insertPid, middle_node_2, key_3);
			
			int end_pid = pf.endPid();
			key_2 = key_3;
			pid_2 = end_pid;
			
			error_occurred = middle_node.write(pid_1, pf);
			
			if(error_occurred != 0)
				return error_occurred;
			
			error_occurred = middle_node_2.write(end_pid, pf);
			
			if(error_occurred != 0)
				return error_occurred;
			
			if(treeHeight == 1)
			{
				BTNonLeafNode newRoot;
				newRoot.initializeRoot(pid_1, key_3, end_pid);
				treeHeight++;
				rootPid = pf.endPid();
				newRoot.write(rootPid, pf);
			}
		}
		return 0;
	}
}

/*
 * Find the leaf_node-node index entry whose key value is larger than or 
 * equal to searchKey, and output the location of the entry in IndexCursor.
 * IndexCursor is a "pointer" to a B+tree leaf_node-node entry consisting of
 * the PageId of the node and the SlotID of the index entry.
 * Note that, for range queries, we need to scan the B+tree leaf_node nodes.
 * For example, if the query is "key > 1000", we should scan the leaf_node
 * nodes starting with the key value 1000. For this reason,
 * it is better to return the location of the leaf_node node entry 
 * for a given searchKey, instead of returning the RecordId
 * associated with the searchKey directly.
 * Once the location of the index entry is identified and returned 
 * from this function, you should call readForward() to retrieve the
 * actual (key, rid) pair from the index.
 * @param key[IN] the key to find.
 * @param cursor[OUT] the cursor pointing to the first index entry
 *                    with the key value.
 * @return error_occurred code. 0 if no error_occurred.
 */
RC BTreeIndex::locate(int searchKey, IndexCursor& cursor)
{
	RC error_occurred;	
	BTNonLeafNode middle_node;
	BTLeafNode leaf_node;
	int eid;
	int current_height = 1;
	PageId next_pid = rootPid;
	
	while(current_height != treeHeight)
	{
		error_occurred = middle_node.read(next_pid, pf);
		if(error_occurred != 0)
			return error_occurred;
		error_occurred = middle_node.locateChildPtr(searchKey, next_pid);
		if(error_occurred != 0)
			return error_occurred;
		current_height++;
	}
	
	error_occurred = leaf_node.read(next_pid, pf);	
	if(error_occurred != 0)
		return error_occurred;
	error_occurred = leaf_node.locate(searchKey, eid);
	
	if(error_occurred != 0)
		return error_occurred;
	cursor.eid = eid;
	cursor.pid = next_pid;
	
	return 0;
}

RC BTreeIndex::locate_recursion(int searchKey, IndexCursor& cursor, int current_height, PageId& next_pid)
{
	if(searchKey < 0)
		return RC_INVALID_ATTRIBUTE;	
	RC error_occurred;
	
	if(current_height == treeHeight)
	{
		int eid = -1;
		BTLeafNode leaf_node;
		error_occurred = leaf_node.read(next_pid, pf);	
		if(error_occurred != 0)
			return error_occurred;
		error_occurred = leaf_node.locate(searchKey, eid);
		if(error_occurred != 0)
			return error_occurred;
		cursor.eid = eid;
		cursor.pid = next_pid;
		return 0;
	}

	BTNonLeafNode middle_node;
	error_occurred = middle_node.read(next_pid, pf);
	if(error_occurred != 0)
		return error_occurred;
	error_occurred = middle_node.locateChildPtr(searchKey, next_pid);
	if(error_occurred != 0)
		return error_occurred;
	return locate_recursion(searchKey, cursor, current_height-1, next_pid);
}

/*
 * Read the (key, rid) pair at the location specified by the index cursor,
 * and move foward the cursor to the next entry.
 * @param cursor[IN/OUT] the cursor pointing to an leaf_node-node index entry in the b+tree
 * @param key[OUT] the key stored at the index cursor location.
 * @param rid[OUT] the RecordId stored at the index cursor location.
 * @return error_occurred code. 0 if no error_occurred
 */ 
RC BTreeIndex::readForward(IndexCursor& cursor, int& key, RecordId& rid)
{
	RC error_occurred;
	PageId cursor_pid = cursor.pid;
	int cursor_eid = cursor.eid;

	BTLeafNode leaf_node;
	error_occurred = leaf_node.read(cursor_pid, pf);
	
	if(error_occurred != 0)
		return error_occurred;

	error_occurred = leaf_node.readEntry(cursor_eid, key, rid);
	
	if(error_occurred != 0)
		return error_occurred;

	if(cursor_pid <= 0)
		return RC_INVALID_CURSOR;

	if(cursor_eid + 1 >= leaf_node.getKeyCount())
	{
		cursor_eid = 0;
		cursor_pid = leaf_node.getNextNodePtr();
	}
	else
		cursor_eid++;
	cursor.eid = cursor_eid;
	cursor.pid = cursor_pid;
	return 0;
}

void BTreeIndex::print()
{
}
