#include "BTreeNode.h"
#include "RecordFile.h"
#include "PageFile.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace std;

int entryPairLeafNodeSize = sizeof(RecordId) + sizeof(int);
int entryPairNonLeafNodeSize = sizeof(PageId) + sizeof(int);
int INTEGER_SIZE = sizeof(int);
int MAX_KEYS_LEAF_NODE = floor(PageFile::PAGE_SIZE/entryPairLeafNodeSize);
int MAX_KEYS_NON_LEAF_NODE = floor(PageFile::PAGE_SIZE/entryPairNonLeafNodeSize);

BTLeafNode::BTLeafNode()
{
	memset(buffer, 0, PageFile::PAGE_SIZE);
	numOfKeys = 0;
	FLAG_ADDED_NEW_KEY = 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)
{ 
	//memset(buffer, 0, PageFile::PAGE_SIZE);
	RC rc = pf.read(pid, buffer);
	return rc;
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)
{ 
	RC rc = pf.write(pid, buffer);
	return rc; 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTLeafNode::getKeyCount()
{ 
	if (FLAG_ADDED_NEW_KEY == 1)	//a new key(s) was added
	{
		numOfKeys = 0;
		//int index = 0;
		int indexInBuffer = 0;
		int key_holder;
		char* char_key_holder = buffer;
		memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
		while(numOfKeys < MAX_KEYS_LEAF_NODE && key_holder != 0)
		//while((indexInBuffer < PageFile::PAGE_SIZE - entryPairLeafNodeSize) && key_holder != 0)
		{
			numOfKeys++;
			indexInBuffer+=entryPairLeafNodeSize;
			char_key_holder += entryPairLeafNodeSize;
			if (numOfKeys < MAX_KEYS_LEAF_NODE)
			{
				memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
			}
		}
		FLAG_ADDED_NEW_KEY = 0;
	}
	return numOfKeys; 
}

/*
 * Insert a (key, rid) pair to the node.
 * @param key[IN] the key to insert
 * @param rid[IN] the RecordId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTLeafNode::insert(int key, const RecordId& rid)
{ 
	/*
		if (numOfKeys == 0)
	{
		memcpy(buffer, rid, sizeof(RecordId));
	}
	*/
	
	return 0;
}

/*
 * Insert the (key, rid) pair to the node
 * and split the node half and half with sibling.
 * The first key of the sibling node is returned in siblingKey.
 * @param key[IN] the key to insert.
 * @param rid[IN] the RecordId to insert.
 * @param sibling[IN] the sibling node to split with. This node MUST be EMPTY when this function is called.
 * @param siblingKey[OUT] the first key in the sibling node after split.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::insertAndSplit(int key, const RecordId& rid, 
                              BTLeafNode& sibling, int& siblingKey)
{ return 0; }

/**
 * If searchKey exists in the node, set eid to the index entry
 * with searchKey and return 0. If not, set eid to the index entry
 * immediately after the largest index key that is smaller than searchKey,
 * and return the error code RC_NO_SUCH_RECORD.
 * Remember that keys inside a B+tree node are always kept sorted.
 * @param searchKey[IN] the key to search for.
 * @param eid[OUT] the index entry number with searchKey or immediately
                   behind the largest key smaller than searchKey.
 * @return 0 if searchKey is found. Otherwise return an error code.
 */
RC BTLeafNode::locate(int searchKey, int& eid)
{ 
	int maxNumKeys = getKeyCount();
	RecordId RID;
	int retrieved_key;
	for(int index = 0; index < maxNumKeys; index++)
	{
		readEntry(index, retrieved_key, RID);
		if (retrieved_key >= searchKey)
		{
			eid = index;
			return 0;
		}
	}
	return RC_NO_SUCH_RECORD;	//failed to find the searchKey
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
	if (eid < 0 || eid >= getKeyCount())
	{
		return -1;
	}
	else
	{
		int entryKey;
		memcpy(&entryKey, buffer + (eid*entryPairLeafNodeSize), INTEGER_SIZE);
		key = entryKey;
		RecordId entryId;
		memcpy(&entryId, buffer + (eid*entryPairLeafNodeSize)+INTEGER_SIZE, sizeof(RecordId));
		rid = entryId;
	}

	return 0;
}

/*
 * Return the pid of the next sibling node.
 * @return the PageId of the next sibling node 
 */
PageId BTLeafNode::getNextNodePtr()
{
	PageId pid;
	memcpy(&pid, buffer+PageFile::PAGE_SIZE-sizeof(PageId), sizeof(PageId));
	return pid;
}

/*
 * Set the pid of the next sibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::setNextNodePtr(PageId pid)
{ 
	if (pid >= 0)
	{
		memcpy(buffer+(PageFile::PAGE_SIZE-sizeof(PageId)),&pid,sizeof(PageId));
		return 0;
	}
	return RC_INVALID_PID;
}

void BTLeafNode::print()
{  }

////////////////////////////////////////////BTNonLeafNode//////////////////////////////////////////
BTNonLeafNode::BTNonLeafNode()
{
	memset(buffer, 0, PageFile::PAGE_SIZE);
	numOfKeys = 0;
	FLAG_ADDED_NEW_KEY = 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{ 
	memset(buffer, 0, PageFile::PAGE_SIZE);
	RC rc = pf.read(pid, buffer);
	return rc;
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::write(PageId pid, PageFile& pf)
{ 
	RC rc = pf.write(pid, buffer);
	return rc;
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */
int BTNonLeafNode::getKeyCount()
{ 
	return numOfKeys;
}


/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ return 0; }

/*
 * Insert the (key, pid) pair to the node
 * and split the node half and half with sibling.
 * The middle key after the split is returned in midKey.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @param sibling[IN] the sibling node to split with. This node MUST be empty when this function is called.
 * @param midKey[OUT] the key in the middle after the split. This key should be inserted to the parent node.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::insertAndSplit(int key, PageId pid, BTNonLeafNode& sibling, int& midKey)
{ return 0; }

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{ return 0; }

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ return 0; }

void BTNonLeafNode::print()
{  }
