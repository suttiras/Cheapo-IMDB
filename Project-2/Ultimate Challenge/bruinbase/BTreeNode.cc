#include "BTreeNode.h"
#include "RecordFile.h"
#include "PageFile.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <string>
#include <sstream>
#include <stdlib.h>  
using namespace std;

int entryPairLeafNodeSize = sizeof(RecordId) + sizeof(int);
int entryPairNonLeafNodeSize = sizeof(PageId) + sizeof(int);
int INTEGER_SIZE = sizeof(int);
int MAX_KEYS_LEAF_NODE = floor((PageFile::PAGE_SIZE - sizeof(PageId))/entryPairLeafNodeSize);
int MAX_KEYS_NON_LEAF_NODE = floor((PageFile::PAGE_SIZE - sizeof(PageId))/entryPairNonLeafNodeSize);
int PAGE_ID_SIZE = sizeof(PageId);

#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

//Leaf node constructor
BTLeafNode::BTLeafNode()	//works!
{
	memset(buffer, '\0', PageFile::PAGE_SIZE);
	numOfKeys = 0;
	FLAG_ADDED_NEW_KEY = 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::read(PageId pid, const PageFile& pf)	//works!
{ 
	memset(buffer, '\0', PageFile::PAGE_SIZE);
	RC rc = pf.read(pid, buffer);
	return rc;
}
    
/*
 * Write the content of the node to the page pid in the PageFile pf.
 * @param pid[IN] the PageId to write to
 * @param pf[IN] PageFile to write to
 * @return 0 if successful. Return an error code if there is an error.
 */
RC BTLeafNode::write(PageId pid, PageFile& pf)	//works!
{ 
	RC rc = pf.write(pid, buffer);
	return rc; 
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */

int BTLeafNode::getKeyCount()	//doesn't work for when key == 0	//works!
{ 
	int previousNumOfKeys = numOfKeys;
	//if (FLAG_ADDED_NEW_KEY == 1)	//a new key(s) was added
	//{
		numOfKeys = 0;
		//int index = 0;
		int indexInBuffer = 0;
		int key_holder;
		int FLAG_UNTIL_POSITIVE = 0;
		char* char_key_holder = buffer;
		char check_null_key_holder;
		memcpy(&key_holder, char_key_holder, INTEGER_SIZE);

		//memcpy(&check_null_key_holder, char_key_holder, INTEGER_SIZE);
		//if (FLAG_ADDED_ZERO == 1 && previousNumOfKeys == 0)
		/*
		if (previousNumOfKeys == 0)
		{
			numOfKeys = 1;
		}
		//else if (FLAG_ADDED_ZERO == 1 && previousNumOfKeys == 1)
		if (previousNumOfKeys == 1)
		{
			numOfKeys = 2;
		}
		else
		{
		*/
			//while (numOfKeys < MAX_KEYS_LEAF_NODE && key_holder != '\0' || (FLAG_ADDED_ZERO == 1 && FLAG_UNTIL_POSITIVE == 0 && previousNumOfKeys > 1))
				//while((indexInBuffer < PageFile::PAGE_SIZE - entryPairLeafNodeSize) && key_holder != 0)
		while (numOfKeys < MAX_KEYS_LEAF_NODE && key_holder != '\0')
		{
			/*
			if (numOfKeys > previousNumOfKeys)
			{
			break;
			}
			*/
			numOfKeys++;
			indexInBuffer += entryPairLeafNodeSize;
			char_key_holder += entryPairLeafNodeSize;
			if (numOfKeys < MAX_KEYS_LEAF_NODE)
			{
				memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
				//memcpy(&key_holder, char_key_holder - INTEGER_SIZE, INTEGER_SIZE);
				/*
				if (key_holder > 0)
				{
				FLAG_UNTIL_POSITIVE = 1;	//reached a positive number
				}
				*/
				if (key_holder == 0)
				{
					break;
				}
			}
			//}
			//FLAG_ADDED_NEW_KEY = 0;
			//}

		}
	
	return numOfKeys; 
}

RC BTLeafNode::insertPid(PageId pid)
{
	numOfKeys = getKeyCount();
	memcpy(buffer + (numOfKeys * entryPairLeafNodeSize), &pid, PAGE_ID_SIZE);
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
	/*
	if (key == 0)
	{
	FLAG_ADDED_ZERO = 1;
	}
	*/
	//there are no entries in the node
	if (numOfKeys == 0)
	{
		//memcpy(buffer, &rid, sizeof(RecordId));
		memcpy(buffer, &key, INTEGER_SIZE);
		memcpy(buffer + INTEGER_SIZE, &rid, sizeof(RecordId));
		FLAG_ADDED_NEW_KEY = 1;
	}
	//there are max number of entries in the node
	else if (numOfKeys >= MAX_KEYS_LEAF_NODE)
	{
		//return -1;
		return RC_NODE_FULL;
	}
	//there are less than max number of entries in the node
	else
	{
		int eid;
		RC rc = locate(key, eid);
		int temp_key = 0;
		RecordId temp_rid;
		readEntry(eid, temp_key, temp_rid);
		if (temp_key < key)
		{
			if (numOfKeys > 1)
			{
				//if key is the max key
				if (eid == numOfKeys - 1)
					//if (eid >= numOfKeys)
				{
					//memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &rid, sizeof(RecordId));
					//memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
					
					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &key, INTEGER_SIZE);
					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize) + INTEGER_SIZE, &rid, sizeof(RecordId));
				}
				else
				{
					char temp[(numOfKeys - eid)*entryPairLeafNodeSize];
					//try to copy all entries after new entry
					memcpy(&temp, buffer + (eid*entryPairLeafNodeSize), (numOfKeys - eid)*entryPairLeafNodeSize);

					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &temp, (numOfKeys - eid)*entryPairLeafNodeSize);

					//memcpy(buffer + (eid*entryPairLeafNodeSize), &rid, sizeof(RecordId));
					//memcpy(buffer + (eid*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
					memcpy(buffer + (eid*entryPairLeafNodeSize), &key, INTEGER_SIZE);
					memcpy(buffer + (eid*entryPairLeafNodeSize) + INTEGER_SIZE, &rid, sizeof(RecordId));
				}
			}
			//if there is only one key
			else if (numOfKeys == 1)
			{
				if (temp_key < key)
				{
					//memcpy(buffer + entryPairLeafNodeSize, &rid, sizeof(RecordId));
					//memcpy(buffer + entryPairLeafNodeSize + sizeof(RecordId), &key, INTEGER_SIZE);
					memcpy(buffer + entryPairLeafNodeSize, &key, INTEGER_SIZE);
					memcpy(buffer + entryPairLeafNodeSize + INTEGER_SIZE, &rid, sizeof(RecordId));
				}
				else if (temp_key >= key)
				{
					char temp[(numOfKeys - eid)*entryPairLeafNodeSize];
					//try to copy all entries after new entry
					memcpy(&temp, buffer + (eid*entryPairLeafNodeSize), (numOfKeys - eid)*entryPairLeafNodeSize);

					memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &temp, (numOfKeys - eid)*entryPairLeafNodeSize);

					//memcpy(buffer + (eid*entryPairLeafNodeSize), &rid, sizeof(RecordId));
					//memcpy(buffer + (eid*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
					memcpy(buffer + (eid*entryPairLeafNodeSize), &key, INTEGER_SIZE);
					memcpy(buffer + (eid*entryPairLeafNodeSize) + INTEGER_SIZE, &rid, sizeof(RecordId));
				}
			}
			FLAG_ADDED_NEW_KEY = 1;
			//new
			//numOfKeys++;
		}

		else if (temp_key > key)
		{
			char temp[(numOfKeys - eid)*entryPairLeafNodeSize];
			//try to copy all entries after new entry
			memcpy(&temp, buffer + (eid*entryPairLeafNodeSize), (numOfKeys - eid)*entryPairLeafNodeSize);

			memcpy(buffer + ((eid + 1)*entryPairLeafNodeSize), &temp, (numOfKeys - eid)*entryPairLeafNodeSize);

			//memcpy(buffer + (eid*entryPairLeafNodeSize), &rid, sizeof(RecordId));
			//memcpy(buffer + (eid*entryPairLeafNodeSize) + sizeof(RecordId), &key, INTEGER_SIZE);
			memcpy(buffer + (eid*entryPairLeafNodeSize), &key, INTEGER_SIZE);
			memcpy(buffer + (eid*entryPairLeafNodeSize) + INTEGER_SIZE, &rid, sizeof(RecordId));
			FLAG_ADDED_NEW_KEY = 1;
			//numOfKeys++;
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
	//insert(key, rid);
	int maxNumKeys = getKeyCount();
	double median = ceil((double)maxNumKeys / 2);

	/*
	if (FLAG_ADDED_ZERO == 1)
	{
	sibling.set_ZERO_FLAG();
	}
	*/

	PageId pointerToSiblingNode;
	PageId last_pid;

	int medianKey;

	//memcpy(&last_pid, buffer + (maxNumKeys*entryPairLeafNodeSize), PAGE_ID_SIZE);
	sibling.setNextNodePtr(getNextNodePtr());

	for (int index = (int)median; index < maxNumKeys; index++)
	{
		int new_key;
		RecordId new_rid;
		readEntry(index, new_key, new_rid);
		if (index == median)
		{
			medianKey = new_key;
		}

		//to keep the pid of the sibling node
		/*
		if (index == median)
		{
		pointerToSiblingNode = new_rid.pid;
		}
		*/

		sibling.insert(new_key, new_rid);
		//new
		//memset(buffer + index*entryPairLeafNodeSize, '\0', entryPairLeafNodeSize);
		//end of new
		if (index == median)
		{
			siblingKey = new_key;
		}
	}
	/*
	sibling.print();
	cout << "\n";
	*/
	for (int index = (int)median; index < maxNumKeys; index++)
	{
		memset(buffer + index*entryPairLeafNodeSize, '\0', entryPairLeafNodeSize);
	}

	if (key >= medianKey)
	{
		sibling.insert(key, rid);
	}
	else
	{
		insert(key, rid);
	}

	FLAG_ADDED_NEW_KEY = 1;
	sibling.set_FLAG();

	//to set the end page file to point to the sibling node
	/*
	int currentNumKeys = getKeyCount();
	memcpy(buffer + currentNumKeys*entryPairLeafNodeSize, &pointerToSiblingNode, PAGE_ID_SIZE);
	sibling.insertPid(last_pid);
	*/
	/*
	int new_eid;
	if (locate(key, new_eid) != 0)
	sibling.insert(key,rid);
	else
	insert(key, rid);
	*/
	return 0;
	//return RC_NO_SEARCH_RECORD;
}

/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 */

RC BTLeafNode::locate(int searchKey, int& eid)	//works!
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
		else if (retrieved_key > searchKey)	//couldn't find search key
		{
			eid = index;
			return 0;
			//return RC_NO_SUCH_RECORD;
		}
	}

	//eid = index - 1;	//set eid to the index entry immediately after the largest index key 
						//that is smaller than searchKey
	eid = index;
	return 0;
	//return RC_NO_SUCH_RECORD;	//failed to find the searchKey
}

/*
 * Read the (key, rid) pair from the eid entry.
 * @param eid[IN] the entry number to read the (key, rid) pair from
 * @param key[OUT] the key from the entry
 * @param rid[OUT] the RecordId from the entry
 * @return 0 if successful. Return an error code if there is an error.
 */

RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)	//works!
{
	if (eid < 0 || eid >= getKeyCount())
	{
		return -1;
	}
	else
	{
		//if (eid != 0)
		//{
			RecordId entryId;
			memcpy(&entryId, buffer + (eid*entryPairLeafNodeSize) + INTEGER_SIZE, sizeof(RecordId));
			rid = entryId;
			int entryKey;
			//memcpy(&entryKey, buffer + entryPairLeafNodeSize, INTEGER_SIZE);
			memcpy(&entryKey, buffer + (eid*entryPairLeafNodeSize), INTEGER_SIZE);
			key = entryKey;
		//}
		/*
		else
		{
			RecordId entryId;
			memcpy(&entryId, buffer, sizeof(RecordId));
			rid = entryId;
			int entryKey;
			//memcpy(&entryKey, buffer + entryPairLeafNodeSize, INTEGER_SIZE);
			memcpy(&entryKey, buffer + 2*INTEGER_SIZE, INTEGER_SIZE);
			key = entryKey;
		}*/
	}

	return 0;
}

/*
 * Return the pid of the next slibling node.
 * @return the PageId of the next sibling node 
 */

PageId BTLeafNode::getNextNodePtr()	//works!
{
	PageId pid;
	//memcpy(&pid, buffer+PageFile::PAGE_SIZE-sizeof(PageId), sizeof(PageId));
	memcpy(&pid, buffer+PageFile::PAGE_SIZE-PAGE_ID_SIZE, PAGE_ID_SIZE);
	return pid;
}

/*
 * Set the pid of the next slibling node.
 * @param pid[IN] the PageId of the next sibling node 
 * @return 0 if successful. Return an error code if there is an error.
 */

RC BTLeafNode::setNextNodePtr(PageId pid)	//works!
{ 
	if (pid >= 0)
	{
		//memcpy(buffer+(PageFile::PAGE_SIZE-sizeof(PageId)),&pid,sizeof(PageId));
		memcpy(buffer+(PageFile::PAGE_SIZE-PAGE_ID_SIZE),&pid,PAGE_ID_SIZE);
		return 0;
	}
	return RC_INVALID_PID;
}

/*
 * Print the keys of the node to cout
 */
void BTLeafNode::print()
{
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(RecordId) + sizeof(int);
	
	char* temp = buffer;
	
	for(int i=0; i<getKeyCount()*pairSize; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey
		
		cout << insideKey << " ";
		
		temp += pairSize; //Jump temp over to the next key
	}
	
	cout << "" << endl;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------


//Nonleaf node constructor


BTNonLeafNode::BTNonLeafNode()	//works!
{
	memset(buffer, 0, PageFile::PAGE_SIZE);
	numOfKeys = 0;
	FLAG_ADDED_NEW_KEY = 0;
	FLAG_KEY_BEFORE_PID = 0;
}

/*
 * Read the content of the node from the page pid in the PageFile pf.
 * @param pid[IN] the PageId to read
 * @param pf[IN] PageFile to read from
 * @return 0 if successful. Return an error code if there is an error.
 */

RC BTNonLeafNode::read(PageId pid, const PageFile& pf)	//works!
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
RC BTNonLeafNode::write(PageId pid, PageFile& pf)	//works!
{ 
	RC rc = pf.write(pid, buffer);
	return rc;
}

/*
 * Return the number of keys stored in the node.
 * @return the number of keys in the node
 */

int BTNonLeafNode::getKeyCount()	//works!
{ 
	int previousNumOfKeys = numOfKeys;
	//if (FLAG_ADDED_NEW_KEY == 1)	//a new key(s) was added
	//{
		numOfKeys = 0;
		//int index = 0;
		int indexInBuffer = 0;
		int key_holder;
		int FLAG_UNTIL_POSITIVE = 0;
		char* char_key_holder = buffer + PAGE_ID_SIZE + INTEGER_SIZE;
		char check_null_key_holder;
		int previous_key_holder;
		memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
		previous_key_holder = key_holder + 1;
		//memcpy(&check_null_key_holder, char_key_holder, INTEGER_SIZE);
		/*
		if (FLAG_ADDED_ZERO == 1 && previousNumOfKeys == 0)
		{
		numOfKeys = 1;
		}
		else if (FLAG_ADDED_ZERO == 1 && previousNumOfKeys == 1)
		{
		numOfKeys = 2;
		}
		else
		{
		*/

		//while (numOfKeys < MAX_KEYS_NON_LEAF_NODE && key_holder != '\0' || (FLAG_ADDED_ZERO == 1 && FLAG_UNTIL_POSITIVE == 0 && previousNumOfKeys > 1))
		//while((indexInBuffer < PageFile::PAGE_SIZE - entryPairLeafNodeSize) && key_holder != 0)
		while (numOfKeys < MAX_KEYS_NON_LEAF_NODE && key_holder != '\0')
		{
			/*
			if (numOfKeys > previousNumOfKeys)
			{
				break;
			}
			*/
			/*
			if (previous_key_holder == key_holder)
			{
				break;
			}
			*/
			numOfKeys++;
			indexInBuffer += entryPairNonLeafNodeSize;
			char_key_holder += entryPairNonLeafNodeSize;
			if (numOfKeys < MAX_KEYS_NON_LEAF_NODE)
			{
				//memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
				previous_key_holder = key_holder;
				memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
				/*
				if (key_holder > 0)
				{
				FLAG_UNTIL_POSITIVE = 1;	//reached a positive number
				}
				*/

				//new
				if (key_holder == 0)
				{
					break;
				}
				//
			}
		//}
		FLAG_ADDED_NEW_KEY = 0;
		//}

	}

	return numOfKeys;


	/*
	if (FLAG_ADDED_NEW_KEY == 1)	//a new key(s) was added
	{
		numOfKeys = 0;
		//int index = 0;
		int indexInBuffer = 0;
		int key_holder;
		char* char_key_holder = buffer + PAGE_ID_SIZE;
		//memcpy(&key_holder, char_key_holder, INTEGER_SIZE);
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
	*/
}

/*
 * Insert a (key, pid) pair to the node.
 * @param key[IN] the key to insert
 * @param pid[IN] the PageId to insert
 * @return 0 if successful. Return an error code if the node is full.
 */
RC BTNonLeafNode::insert(int key, PageId pid)
{
	numOfKeys = getKeyCount();
	/*

	if (FLAG_KEY_BEFORE_PID == 0)
	{
	//there are no entries in the node
	if (numOfKeys == 0)
	{
	memcpy(buffer, &pid, PAGE_ID_SIZE);
	memcpy(buffer + PAGE_ID_SIZE, &key, INTEGER_SIZE);
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
	PageId temp_pid;
	readEntry(eid, temp_key, temp_pid);
	if (temp_key < key)
	{
	if (numOfKeys > 1)
	{
	//if key is the max key
	if (eid == numOfKeys - 1)
	{
	memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize), &pid, PAGE_ID_SIZE);
	memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize) + PAGE_ID_SIZE, &key, INTEGER_SIZE);
	}
	else
	{
	char temp[(numOfKeys - eid)*entryPairNonLeafNodeSize];
	//try to copy all entries after new entry
	memcpy(&temp, buffer + (eid*entryPairNonLeafNodeSize), (numOfKeys - eid)*entryPairNonLeafNodeSize);

	memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize), &temp, (numOfKeys - eid)*entryPairNonLeafNodeSize);

	memcpy(buffer + (eid*entryPairNonLeafNodeSize), &key, INTEGER_SIZE);
	memcpy(buffer + (INTEGER_SIZE + eid*entryPairNonLeafNodeSize), &pid, PAGE_ID_SIZE);
	}
	}
	//if there is only one key
	else if (numOfKeys == 1)
	{
	if (temp_key < key)
	{
	memcpy(buffer + entryPairNonLeafNodeSize, &pid, PAGE_ID_SIZE);
	memcpy(buffer + entryPairNonLeafNodeSize + PAGE_ID_SIZE, &key, INTEGER_SIZE);
	}
	else if (temp_key >= key)
	{
	char temp[(numOfKeys - eid)*entryPairNonLeafNodeSize];
	//try to copy all entries after new entry
	memcpy(&temp, buffer + (eid*entryPairNonLeafNodeSize), (numOfKeys - eid)*entryPairNonLeafNodeSize);

	memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize), &temp, (numOfKeys - eid)*entryPairNonLeafNodeSize);

	memcpy(buffer + (eid*entryPairNonLeafNodeSize), &key, INTEGER_SIZE);
	memcpy(buffer + (INTEGER_SIZE + eid*entryPairNonLeafNodeSize), &pid, PAGE_ID_SIZE);
	}
	}
	FLAG_ADDED_NEW_KEY = 1;
	}

	else if (temp_key > key)
	{
	char temp[(numOfKeys - eid)*entryPairNonLeafNodeSize];
	//try to copy all entries after new entry
	memcpy(&temp, buffer + (eid*entryPairNonLeafNodeSize), (numOfKeys - eid)*entryPairNonLeafNodeSize);

	memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize), &temp, (numOfKeys - eid)*entryPairNonLeafNodeSize);

	memcpy(buffer + (eid*entryPairNonLeafNodeSize), &key, INTEGER_SIZE);
	memcpy(buffer + (INTEGER_SIZE + eid*entryPairNonLeafNodeSize), &pid, PAGE_ID_SIZE);
	FLAG_ADDED_NEW_KEY = 1;
	}



	}

	}
	*/
	//if flag is set
	//else
	//{

	if (key == 0)
	{
		FLAG_ADDED_ZERO = 1;
	}

	//there are no entries in the node
	if (numOfKeys == 0)
	{
		//memcpy(buffer + PAGE_ID_SIZE, &key, INTEGER_SIZE);
		//memcpy(buffer + PAGE_ID_SIZE + INTEGER_SIZE, &pid, PAGE_ID_SIZE);
		memcpy(buffer + (2 * PAGE_ID_SIZE), &key, INTEGER_SIZE);
		memcpy(buffer + (2 * PAGE_ID_SIZE) + INTEGER_SIZE, &pid, PAGE_ID_SIZE);

		FLAG_ADDED_NEW_KEY = 1;
	}
	//there are max number of entries in the node
	else if (numOfKeys >= MAX_KEYS_NON_LEAF_NODE)
	{
		//return -1;
		return RC_NODE_FULL;
	}
	//there are less than max number of entries in the node
	else
	{
		int eid;
		RC rc = locate(key, eid);
		int temp_key;
		PageId temp_pid;
		readEntry(eid, temp_key, temp_pid);
		if (temp_key < key)
		{
			if (numOfKeys > 1)
			{
				//if key is the max key
				if (eid == numOfKeys - 1)
				{
					memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &key, INTEGER_SIZE);
					memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE) + INTEGER_SIZE, &pid, PAGE_ID_SIZE);
				}
				else
				{
					char temp[(numOfKeys - eid)*entryPairNonLeafNodeSize + (2 * PAGE_ID_SIZE)];
					//try to copy all entries after new entry
					memcpy(&temp, buffer + (eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), (numOfKeys - eid)*entryPairNonLeafNodeSize);

					memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &temp, (numOfKeys - eid)*entryPairNonLeafNodeSize);

					memcpy(buffer + (eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &key, INTEGER_SIZE);
					memcpy(buffer + (INTEGER_SIZE + eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &pid, PAGE_ID_SIZE);
				}
			}
			//if there is only one key
			else if (numOfKeys == 1)
			{
				if (temp_key < key)
				{
					memcpy(buffer + entryPairNonLeafNodeSize + (2 * PAGE_ID_SIZE), &key, INTEGER_SIZE);
					memcpy(buffer + entryPairNonLeafNodeSize + (2 * PAGE_ID_SIZE) + INTEGER_SIZE, &pid, PAGE_ID_SIZE);

				}
				else if (temp_key >= key)
				{
					char temp[(numOfKeys - eid)*entryPairNonLeafNodeSize + (2 * PAGE_ID_SIZE)];
					//try to copy all entries after new entry
					memcpy(&temp, buffer + (eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), (numOfKeys - eid)*entryPairNonLeafNodeSize);

					memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &temp, (numOfKeys - eid)*entryPairNonLeafNodeSize);

					memcpy(buffer + (eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &key, INTEGER_SIZE);
					memcpy(buffer + (INTEGER_SIZE + eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &pid, PAGE_ID_SIZE);
				}
			}
			FLAG_ADDED_NEW_KEY = 1;
		}

		else if (temp_key > key)
		{
			char temp[(numOfKeys - eid)*entryPairNonLeafNodeSize + (2 * PAGE_ID_SIZE)];
			//try to copy all entries after new entry
			memcpy(&temp, buffer + (eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), (numOfKeys - eid)*entryPairNonLeafNodeSize);

			memcpy(buffer + ((eid + 1)*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &temp, (numOfKeys - eid)*entryPairNonLeafNodeSize);

			memcpy(buffer + (eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &key, INTEGER_SIZE);
			memcpy(buffer + (INTEGER_SIZE + eid*entryPairNonLeafNodeSize) + (2 * PAGE_ID_SIZE), &pid, PAGE_ID_SIZE);

			FLAG_ADDED_NEW_KEY = 1;
		}



	}
	//}


	return 0;





	//there are no entries in the node
	/*
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
	*/
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
	memset(sibling.buffer, 0, PageFile::PAGE_SIZE);
	int maxNumKeys = getKeyCount();
	double median = ceil((double)maxNumKeys / 2);

	if (FLAG_ADDED_ZERO == 1)
	{
		sibling.set_ZERO_FLAG();
	}
	
	int half = median * entryPairNonLeafNodeSize + entryPairNonLeafNodeSize;
	//int half = median * entryPairNonLeafNodeSize;
	
	int first_half_key;
	int second_half_key;
	
	memcpy(&first_half_key, buffer + half - entryPairNonLeafNodeSize, INTEGER_SIZE);
	//memcpy(&first_half_key, buffer + half - INTEGER_SIZE, INTEGER_SIZE);
	memcpy(&second_half_key, buffer + half, INTEGER_SIZE);
	
	if(key < first_half_key)
	{
		memcpy(sibling.buffer+entryPairNonLeafNodeSize, buffer + half, PageFile::PAGE_SIZE-half);
		//memcpy(sibling.buffer+PAGE_ID_SIZE, buffer + half, PageFile::PAGE_SIZE-half);
		//memcpy(sibling.buffer, buffer + half, PageFile::PAGE_SIZE-half);
		
		sibling.set_FLAG();
		memcpy(&midKey, buffer+half, INTEGER_SIZE);
		//memcpy(&midKey, buffer+half-INTEGER_SIZE, INTEGER_SIZE);
		memcpy(sibling.buffer, buffer+half-4, PAGE_ID_SIZE);
		//memcpy(sibling.buffer, buffer+half-8, PAGE_ID_SIZE);
		memset(buffer+half-entryPairNonLeafNodeSize, 0, PageFile::PAGE_SIZE - half+entryPairNonLeafNodeSize);
		//memset(buffer+half, 0, buffer+PageFile::PAGE_SIZE);	//std::fill(buffer+halfIndex, buffer + PageFile::PAGE_SIZE, 0);
		
		set_FLAG();
		//memcpy(buffer+half-INTEGER_SIZE, &key, INTEGER_SIZE);
		//memcpy(buffer+half, &pid, PAGE_ID_SIZE);
		insert(key, pid);
	}
	else if (key > second_half_key)
	{
		memcpy(sibling.buffer+entryPairNonLeafNodeSize, buffer + half+entryPairNonLeafNodeSize, PageFile::PAGE_SIZE-half - entryPairNonLeafNodeSize);
		//memcpy(sibling.buffer+PAGE_ID_SIZE, buffer + half+entryPairNonLeafNodeSize, PageFile::PAGE_SIZE-half - entryPairNonLeafNodeSize);
		//memcpy(sibling.buffer+PAGE_ID_SIZE, buffer + half+entryPairNonLeafNodeSize+PAGE_ID_SIZE, PageFile::PAGE_SIZE-half - entryPairNonLeafNodeSize-PAGE_ID_SIZE);
		sibling.set_FLAG();
		//memcpy(&midKey, buffer+half+entryPairNonLeafNodeSize, INTEGER_SIZE);
		memcpy(&midKey, buffer+half, INTEGER_SIZE);
		//memcpy(&midKey, buffer+half+PAGE_ID_SIZE, INTEGER_SIZE);
		//memcpy(sibling.buffer, buffer+half+4, PAGE_ID_SIZE);
		//memcpy(sibling.buffer, buffer+half, PAGE_ID_SIZE);
		memcpy(sibling.buffer, buffer+half+INTEGER_SIZE, PAGE_ID_SIZE);
		//memcpy(sibling.buffer, buffer+half+entryPairNonLeafNodeSize, PAGE_ID_SIZE);
		//memset(buffer+half+PAGE_ID_SIZE, 0, PageFile::PAGE_SIZE - half-PAGE_ID_SIZE);
		memset(buffer+half, 0, PageFile::PAGE_SIZE - half);
		set_FLAG();
		sibling.insert(key, pid);
	}
	else
	{
		memcpy(sibling.buffer+entryPairNonLeafNodeSize, buffer + half, PageFile::PAGE_SIZE-half);
		//memcpy(sibling.buffer+PAGE_ID_SIZE, buffer + half, PageFile::PAGE_SIZE-half-PAGE_ID_SIZE);
		//memcpy(sibling.buffer+PAGE_ID_SIZE, buffer + half+PAGE_ID_SIZE, PageFile::PAGE_SIZE-half-PAGE_ID_SIZE);
		sibling.set_FLAG();

		memset(buffer+half, 0, PageFile::PAGE_SIZE - half);
		set_FLAG();
		midKey = key;
		memcpy(sibling.buffer, &pid, PAGE_ID_SIZE);
	}
	FLAG_KEY_BEFORE_PID = 1;
	/*
	insert(key, pid);
	int maxNumKeys = getKeyCount();
	double median = ceil((double)maxNumKeys / 2);

	for (int index = (int)median; index < maxNumKeys; index++)
	{
		int new_key;
		PageId new_pid;
		//readEntry(index, new_key, new_pid);
		//sibling.insert(new_key, new_pid);
		//memcpy(&new_key, buffer + (PAGE_ID_SIZE + index*(entryPairNonLeafNodeSize)), INTEGER_SIZE);
		//memcpy(&new_pid, buffer + (PAGE_ID_SIZE + index*(entryPairNonLeafNodeSize)) + INTEGER_SIZE, PAGE_ID_SIZE);
		memcpy(&new_key, buffer + (index*(entryPairNonLeafNodeSize)), INTEGER_SIZE);
		memcpy(&new_pid, buffer + (index*(entryPairNonLeafNodeSize)) + INTEGER_SIZE, PAGE_ID_SIZE);

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
	
	set_FLAG();
	sibling.set_FLAG();
	
	midKey = getKeyCount() - 1;
	memcpy(&key, buffer + PAGE_ID_SIZE + midKey*(entryPairNonLeafNodeSize), INTEGER_SIZE);
	midKey = key;
	*/
	return 0;
}

RC BTNonLeafNode::readEntry(int eid, int& key, PageId& pid)
{
	if (eid < 0 || eid >= getKeyCount())
	{
		return -1;
	}
	else
	{
		//if(FLAG_KEY_BEFORE_PID == 1)
		//{
		int entryKey;
		memcpy(&entryKey, buffer + (2 * PAGE_ID_SIZE) + (eid*entryPairNonLeafNodeSize), INTEGER_SIZE);
		key = entryKey;
		PageId entryId;
		memcpy(&entryId, buffer + (2 * PAGE_ID_SIZE) + (eid*entryPairNonLeafNodeSize) + INTEGER_SIZE, sizeof(PageId));
		pid = entryId;
		/*}
		else
		{
		int entryKey;
		memcpy(&entryKey, buffer + PAGE_ID_SIZE + (eid*entryPairNonLeafNodeSize) + INTEGER_SIZE, INTEGER_SIZE);
		key = entryKey;
		PageId entryId;
		memcpy(&entryId, buffer + PAGE_ID_SIZE + (eid*entryPairNonLeafNodeSize), sizeof(PageId));
		pid = entryId;
		}*/
	}

	return 0;
}

RC BTNonLeafNode::locate(int searchKey, int& eid)
{
	int maxNumKeys = getKeyCount();
	int retrieved_key;
	int index;
	for (index = 0; index < maxNumKeys; index++)
	{
		memcpy(&retrieved_key, buffer + ((2 * PAGE_ID_SIZE) + index*(entryPairNonLeafNodeSize)), INTEGER_SIZE);
		//if (retrieved_key >= searchKey)
		if (retrieved_key == searchKey)
		{
			eid = index;
			return 0;
		}
		if (retrieved_key > searchKey)
		{
			eid = index;
			return 0;
		}
	}
	//eid = index - 1;
	eid = index;
	return 0;	//failed to find the searchKey
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */

RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)	//works!
{
	int key_holder;
	PageId left;
	PageId right;
	int numKeys = getKeyCount();
	int index = 0;
	while(index < numKeys)
	{
		memcpy(&key_holder, buffer + ((index + 1)*entryPairNonLeafNodeSize), PAGE_ID_SIZE);
		//memcpy(&right, buffer + entryPairNonLeafNodeSize + INTEGER_SIZE + (index*entryPairNonLeafNodeSize) , PAGE_ID_SIZE);	//entry pair + key + all previous entry pairs
		if (index == 0)
		{
			memcpy(&left, buffer, PAGE_ID_SIZE);
		}
		else
			memcpy(&left, buffer + ((index+1)*entryPairNonLeafNodeSize)- INTEGER_SIZE , PAGE_ID_SIZE);	//entry pair + key + all previous entry pairs
		memcpy(&right, buffer + INTEGER_SIZE + ((index+1)*entryPairNonLeafNodeSize), INTEGER_SIZE);	//entry pair + all previous entry pairs
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
	return 0;
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
	buffer_ptr += (2*INTEGER_SIZE);
	memcpy(buffer_ptr, &key, INTEGER_SIZE);
	buffer_ptr += INTEGER_SIZE;
	memcpy(buffer_ptr, &pid2, PAGE_ID_SIZE);
	FLAG_ADDED_NEW_KEY = 1;
	return 0;
}

/*
 * Print the keys of the node to cout
 */
void BTNonLeafNode::print()
{
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(PageId) + sizeof(int);
	
	//Skip the first 8 offset bytes, since there's no key there
	char* temp = buffer+8;
	
	for(int i=8; i<getKeyCount()*pairSize+8; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey

		cout << insideKey << " ";
		
		//Otherwise, searchKey is greater than or equal to insideKey, so we keep checking
		temp += pairSize; //Jump temp over to the next key
	}
	
	cout << "" << endl;	
}
