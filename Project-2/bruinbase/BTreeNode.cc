#include "BTreeNode.h"
#include "RecordFile.h"
#include "PageFile.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <sstream>

using namespace std;

int entryPairLeafNodeSize = sizeof(RecordId) + sizeof(int);
int entryPairNonLeafNodeSize = sizeof(PageId) + sizeof(int);
int INTEGER_SIZE = sizeof(int);
int MAX_KEYS_LEAF_NODE = floor((PageFile::PAGE_SIZE - sizeof(PageId))/entryPairLeafNodeSize);
int MAX_KEYS_NON_LEAF_NODE = floor((PageFile::PAGE_SIZE - sizeof(PageId))/entryPairNonLeafNodeSize);
int PAGE_ID_SIZE = sizeof(PageId);

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

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
	numOfKeys = getKeyCount();
	//there are no entries in the node
	if (numOfKeys == 0)	
	{
		memcpy(buffer, &rid, sizeof(RecordId));
		memcpy(buffer + sizeof(RecordId), &key, INTEGER_SIZE);
		FLAG_ADDED_NEW_KEY = 1;
	}
	//there are max number of entries in the node
	else if (numOfKeys >= MAX_KEYS_LEAF_NODE)	
	{
		return -1;
	}
	//there are less than max number of entries in the node
	else
	{
		int eid;
		RC rc = locate(key, eid);
		int temp_key;
		RecordId temp_rid;
		readEntry(eid, temp_key, temp_rid);
		if (temp_key < key)
		{
			if (numOfKeys > 1)
			{
				//if key is the max key
				if (eid == numOfKeys - 1)
				{
					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &rid, sizeof(RecordId));
					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
				}
				else
				{
					char temp[(numOfKeys - eid)*entryPairLeafNodeSize];
					//try to copy all entries after new entry
					memcpy(&temp, buffer + (eid*entryPairLeafNodeSize), (numOfKeys - eid)*entryPairLeafNodeSize);

					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &temp, (numOfKeys - eid)*entryPairLeafNodeSize);

					memcpy(buffer + (eid*entryPairLeafNodeSize), &rid, sizeof(RecordId));
					memcpy(buffer + (eid*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
				}
			}
			//if there is only one key
			else if (numOfKeys == 1)
			{
				if (temp_key < key)
				{
					memcpy(buffer + entryPairLeafNodeSize, &rid, sizeof(RecordId));
					memcpy(buffer + entryPairLeafNodeSize + sizeof(RecordId), &key, INTEGER_SIZE);
				}
				else if (temp_key >= key)
				{
					char temp[(numOfKeys - eid)*entryPairLeafNodeSize];
					//try to copy all entries after new entry
					memcpy(&temp, buffer + (eid*entryPairLeafNodeSize), (numOfKeys - eid)*entryPairLeafNodeSize);

					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &temp, (numOfKeys - eid)*entryPairLeafNodeSize);

					memcpy(buffer + (eid*entryPairLeafNodeSize), &rid, sizeof(RecordId));
					memcpy(buffer + (eid*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
				}
			}
			FLAG_ADDED_NEW_KEY = 1;
		}

		else if (temp_key > key)
		{
			if (numOfKeys > 1)
			{
				char temp[(numOfKeys - eid)*entryPairLeafNodeSize];
				//try to copy all entries after new entry
				memcpy(&temp, buffer + (eid*entryPairLeafNodeSize), (numOfKeys - eid)*entryPairLeafNodeSize);

				memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &temp, (numOfKeys - eid)*entryPairLeafNodeSize);

				memcpy(buffer + (eid*entryPairLeafNodeSize), &rid, sizeof(RecordId));
				memcpy(buffer + (eid*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
			}
			//if there is only one key
			else if (numOfKeys == 1)
			{

				char temp[(numOfKeys - eid)*entryPairLeafNodeSize];
				//try to copy all entries after new entry
				memcpy(&temp, buffer + (eid*entryPairLeafNodeSize), (numOfKeys - eid)*entryPairLeafNodeSize);

				memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &temp, (numOfKeys - eid)*entryPairLeafNodeSize);

				memcpy(buffer + (eid*entryPairLeafNodeSize), &rid, sizeof(RecordId));
				memcpy(buffer + (eid*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
			}
			FLAG_ADDED_NEW_KEY = 1;
		}

		
		
	}
	
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
                              BTLeafNode& sibling, int& siblingKey)	//called assuming that leaf node is not full
{ 
	memset(sibling.buffer, 0, PageFile::PAGE_SIZE);
	insert(key, rid);
	int maxNumKeys = getKeyCount();
	double median = ceil((double)maxNumKeys/2);
	
	for(int index = (int) median; index < maxNumKeys; index++)
	{		
		int new_key;
		RecordId new_rid;
		readEntry(index, new_key, new_rid);
		sibling.insert(new_key, new_rid);
		//new
		memset(buffer + index*entryPairLeafNodeSize, 0, entryPairLeafNodeSize);
		//end of new
		if (index == median)
		{
			siblingKey = new_key;
		}
	}
	FLAG_ADDED_NEW_KEY = 1;
	sibling.set_FLAG();
	/*
	int new_eid;
	if (locate(key, new_eid) != 0)
		sibling.insert(key,rid);
	else
		insert(key, rid);
		*/
	return 0;
}

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
	int index;
	for(index = 0; index < maxNumKeys; index++)
	{
		readEntry(index, retrieved_key, RID);
		if (retrieved_key >= searchKey)
		{
			eid = index;
			return 0;
		}
	}

	eid = index - 1;	//set eid to the index entry immediately after the largest index key 
						//that is smaller than searchKey
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
	//memcpy(&pid, buffer+PageFile::PAGE_SIZE-sizeof(PageId), sizeof(PageId));
	memcpy(&pid, buffer+PageFile::PAGE_SIZE-PAGE_ID_SIZE, PAGE_ID_SIZE);
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
		//memcpy(buffer+(PageFile::PAGE_SIZE-sizeof(PageId)),&pid,sizeof(PageId));
		memcpy(buffer+(PageFile::PAGE_SIZE-PAGE_ID_SIZE),&pid,PAGE_ID_SIZE);
		return 0;
	}
	return RC_INVALID_PID;
}

void BTLeafNode::print()
{ 
	int temp_key;
	//char counter[PageFile::PAGE_SIZE];
	//counter = buffer;
	int index = 0;

	std::cout << SSTR("Key Count: " << getKeyCount() << '\n');

	while (temp_key != 0 || index < MAX_KEYS_LEAF_NODE)
	{
		if (index == 0)
		{
			memcpy(&temp_key, buffer + sizeof(RecordId), INTEGER_SIZE);
		}
		else
		{
			memcpy(&temp_key, buffer + sizeof(RecordId) + (index*entryPairLeafNodeSize), INTEGER_SIZE);
		}
		
		if (temp_key == 0)
		{
			break;
		}
		//string temp = to_string(temp_key);
		//cout << temp << '\n';
		std::cout << SSTR("Key: " << temp_key << '\n');
		//counter = counter + PAGE_ID_SIZE;
		index++;
	}
}

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
	if (FLAG_ADDED_NEW_KEY == 1)	//a new key(s) was added
	{
		numOfKeys = 0;
		//int index = 0;
		int indexInBuffer = 0;
		int key_holder;
		char* char_key_holder = buffer;
		memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
		while(numOfKeys < MAX_KEYS_NON_LEAF_NODE && key_holder != 0)
		//while((indexInBuffer < PageFile::PAGE_SIZE - entryPairLeafNodeSize) && key_holder != 0)
		{
			numOfKeys++;
			indexInBuffer+=entryPairNonLeafNodeSize;
			char_key_holder += entryPairNonLeafNodeSize;
			if (numOfKeys < MAX_KEYS_NON_LEAF_NODE)
			{
				memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
			}
		}
		FLAG_ADDED_NEW_KEY = 0;
	}
	return numOfKeys; 
}


RC BTNonLeafNode::locate(int searchKey, int& eid)
{
	int maxNumKeys = getKeyCount();
	int retrieved_key;
	for (int index = 0; index < maxNumKeys; index++)
	{
		memcpy(&retrieved_key, buffer + (PAGE_ID_SIZE + index*(entryPairNonLeafNodeSize)), INTEGER_SIZE);
		if (retrieved_key >= searchKey)
		{
			eid = index;
			return 0;
		}
	}
	return RC_NO_SUCH_RECORD;	//failed to find the searchKey
}

/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{ 
	//there are no entries in the node
	if (getKeyCount() == 0)
	{
		memcpy(buffer, &pid, PAGE_ID_SIZE);
		memcpy(buffer + PAGE_ID_SIZE, &key, INTEGER_SIZE);
	}
	//there are max number of entries in the node
	else if (numOfKeys >= MAX_KEYS_NON_LEAF_NODE)
	{
		return -1;
	}
	//there are less than max number of entries in the node
	else
	{
		int eid;
		locate(key, eid);
		char temp[(numOfKeys - eid)*entryPairNonLeafNodeSize];
		//try to copy all entries after new entry
		memcpy(&temp, buffer + (PAGE_ID_SIZE + eid*entryPairNonLeafNodeSize), (numOfKeys - eid)*entryPairNonLeafNodeSize);

		memcpy(buffer + (PAGE_ID_SIZE + (eid + 1)*entryPairNonLeafNodeSize), &temp, (numOfKeys - eid)*entryPairNonLeafNodeSize);

		memcpy(buffer + (PAGE_ID_SIZE + eid*entryPairNonLeafNodeSize), &key, INTEGER_SIZE);
		memcpy(buffer + (PAGE_ID_SIZE + INTEGER_SIZE + eid*entryPairNonLeafNodeSize) + PAGE_ID_SIZE, &pid, PAGE_ID_SIZE);
	}
	FLAG_ADDED_NEW_KEY = 1;
	return 0;
}

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
{ 
	//memset(sibling.buffer, 0, PageFile::PAGE_SIZE);
	int maxNumKeys = getKeyCount();
	double median = ceil((double)maxNumKeys / 2);
	insert(key, pid);

	for (int index = (int)median; index < maxNumKeys; index++)
	{
		int new_key;
		PageId new_pid;
		readEntry(index, new_key, new_pid);
		sibling.insert(new_key, new_pid);
		memcpy(&new_key, buffer + (PAGE_ID_SIZE + index*(entryPairNonLeafNodeSize)), INTEGER_SIZE);
		memcpy(&new_pid, buffer + (PAGE_ID_SIZE + index*(entryPairNonLeafNodeSize)) + INTEGER_SIZE, PAGE_ID_SIZE);

		if (sibling.getKeyCount() > 0)
		{
			sibling.insert(new_key, new_pid);
		}

		else
		{
			PageId new_pid_root;
			memcpy(&new_pid_root, buffer + (index*(entryPairNonLeafNodeSize)), PAGE_ID_SIZE);
			sibling.initializeRoot(new_pid_root, new_key, new_pid);
		}
		//new - clear buffer
		memset(buffer + index*entryPairNonLeafNodeSize, 0, entryPairNonLeafNodeSize);
		//end of new
	}
	memcpy(buffer, &median, INTEGER_SIZE);
	FLAG_ADDED_NEW_KEY = 1;
	sibling.set_FLAG();
	/*
	int new_eid;
	if (locate(key, new_eid) != 0)
		sibling.insert(key, pid);
	else
		insert(key, pid);
		*/
	midKey = getKeyCount() - 1;
	memcpy(&key, buffer + PAGE_ID_SIZE + midKey*(entryPairNonLeafNodeSize), INTEGER_SIZE);
	midKey = key;
	
	return 0;
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{
	int key_holder;
	PageId left;
	PageId right;
	int numKeys = getKeyCount();
	int index = 0;
	while(index < numKeys)
	{
		memcpy(&left, buffer + (index*entryPairNonLeafNodeSize), PAGE_ID_SIZE);
		memcpy(&right, buffer + entryPairNonLeafNodeSize + INTEGER_SIZE + (index*entryPairNonLeafNodeSize) , PAGE_ID_SIZE);	//entry pair + key + all previous entry pairs
		memcpy(&key_holder, buffer + entryPairNonLeafNodeSize + (index*entryPairNonLeafNodeSize) , INTEGER_SIZE);	//entry pair + all previous entry pairs
		if (key_holder > searchKey)	//got child ptr
		{
			pid = left;
			return 0;
		}

		index++;
	}
	if (key_holder <= searchKey)	//we reached end of node
	{
			pid = right;
			return 0;
	}
	return RC_INVALID_PID;
}

/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{ 
	char* buffer_ptr = buffer;
	memcpy(buffer_ptr, &pid1, PAGE_ID_SIZE);
	buffer_ptr += PAGE_ID_SIZE;
	memcpy(buffer_ptr, &key, INTEGER_SIZE);
	buffer_ptr += INTEGER_SIZE;
	memcpy(buffer_ptr, &pid2, PAGE_ID_SIZE);
	FLAG_ADDED_NEW_KEY = 1;
	return 0;
}

void BTNonLeafNode::print()
{  }
