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
 /*
int BTLeafNode::getKeyCount()
{
	//return numKeys;
	
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(RecordId) + sizeof(int);
	int count=0;
	char* temp = buffer;
	
	//Loop through all the indexes in the temp buffer; increment by 12 bytes to jump to next key
	//1008 is the largest possible index of the next inserted pair (since we already know we can fit another pair)
	int i;
	for(i=0; i<=1008; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey
		if(insideKey==0) //Once we hit key of 0, we break
			break;
		//Otherwise, increment count
		count++;
		
		temp += pairSize; //Jump temp over to the next key
	}
	
	return count;
}
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
/*
RC BTLeafNode::insert(int key, const RecordId& rid)
{
	//Save last 4 bytes (the pid) for reconstructing the inserted leaf
	PageId nextNodePtr = getNextNodePtr();
	
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(RecordId) + sizeof(int);
	
	//Return error if no more space in this node
	//Page has 1024 bytes, we need to store 12 bytes (key, rid)
	//That means we can fit 85 with 4 bytes left over for pid pointer to next leaf node
	//Check if adding one more (key, rid) pair will exceed the size limit of 85
	int numTotalPairs = (PageFile::PAGE_SIZE-sizeof(PageId))/pairSize;
	if(getKeyCount()+1 > numTotalPairs) //if(getKeyCount()+1 > 85)
	{
		//cout << "Cannot insert anymore: this node is full!" << endl;
		return RC_NODE_FULL;
	}
	
	//Now we must go through the buffer's sorted keys to see where the new key goes
	char* temp = buffer;
	
	//Loop through all the indexes in the temp buffer; increment by 12 bytes to jump to next key
	//1008 is the largest possible index of the next inserted pair (since we already know we can fit another pair)
	
	int i;
	for(i=0; i<1008; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey
		
		//Once the insideKey is null or key is smaller than some inside key, we stop
		if(insideKey==0 || !(key > insideKey))
			break;
		
		temp += pairSize; //Jump temp over to the next key
	}
	
	//At this point, variable i holds the index to insert the pair and temp is the buffer at that index
	char* newBuffer = (char*)malloc(PageFile::PAGE_SIZE);
	std::fill(newBuffer, newBuffer + PageFile::PAGE_SIZE, 0); //clear the buffer if necessary
	
	//Copy all values from buffer into newBuffer up until i
	memcpy(newBuffer, buffer, i);
	
	//Values to insert as new (key, rid) pair
	PageId pid = rid.pid;
	int sid = rid.sid;
	
	memcpy(newBuffer+i, &key, sizeof(int));
	memcpy(newBuffer+i+sizeof(int), &rid, sizeof(RecordId));
	
	//INCREMENTAL POINTER METHOD:
	//char* insertPos = newBuffer+i;
	//memcpy(insertPos, &pid, sizeof(PageId));
	//insertPos += sizeof(PageId);
	//memcpy(insertPos, &sid, sizeof(int));
	//insertPos += sizeof(int);
	//memcpy(insertPos, &key, sizeof(int));
	
	//Copy the rest of the values into newBuffer
	//Notice that we are neglecting nextNodePtr, so we'll manually copy that in
	memcpy(newBuffer+i+pairSize, buffer+i, getKeyCount()*pairSize - i);
	memcpy(newBuffer+PageFile::PAGE_SIZE-sizeof(PageId), &nextNodePtr, sizeof(PageId));
	
	//Copy newBuffer into buffer, then delete temporary newBuffer to prevent memory leak
	memcpy(buffer, newBuffer, PageFile::PAGE_SIZE);
	free(newBuffer);
	
	//Successfully inserted leaf node, so we increment number of keys
	numKeys++;
	
	return 0;
}
*/

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
                              BTLeafNode& sibling, int& siblingKey)
{
	//Save last 4 bytes (the pid) for reconstructing the inserted leaf
	PageId nextNodePtr = getNextNodePtr();
	
	int pairSize = sizeof(RecordId) + sizeof(int);
	int numTotalPairs = (PageFile::PAGE_SIZE-sizeof(PageId))/pairSize;
	
	//Only split if inserting will cause an overflow; otherwise, return error
	if(!(getKeyCount() >= numTotalPairs))
		return RC_INVALID_FILE_FORMAT;
	
	//If sibling node is not empty, return error
	if(sibling.getKeyCount()!=0)
		return RC_INVALID_ATTRIBUTE;
	
	//Clear sibling buffer just in case
	std::fill(sibling.buffer, sibling.buffer + PageFile::PAGE_SIZE, 0); //clear the buffer if necessary
	
	//Calculate keys to remain in the first half
	int numHalfKeys = ((int)((getKeyCount()+1)/2));
	
	//Find the index at which to split the node's buffer
	int halfIndex = numHalfKeys*pairSize;
	
	//Copy everything on the right side of halfIndex into sibling's buffer (ignore the pid)
	memcpy(sibling.buffer, buffer+halfIndex, PageFile::PAGE_SIZE-sizeof(PageId)-halfIndex);
	
	//Update sibling's number of keys and set pid to current node's pid ptr
	sibling.numKeys = getKeyCount() - numHalfKeys;
	sibling.setNextNodePtr(getNextNodePtr());
	
	//Clear the second half of current buffer except for pid; update number of keys
	std::fill(buffer+halfIndex, buffer + PageFile::PAGE_SIZE - sizeof(PageId), 0); 
	numKeys = numHalfKeys;
	
	//Check which buffer to insert new (key, rid) into
	int firstHalfKey;
	memcpy(&firstHalfKey, sibling.buffer, sizeof(int));
	
	//Insert pair and increment number of keys
	if(key>=firstHalfKey) //If our key belongs in the second buffer (since it's sorted)...
	{
		sibling.insert(key, rid);
	}
	else //Otherwise, place it in the first half
	{
		insert(key, rid);
	}
	
	//Copy over sibling's first key and rid
	memcpy(&siblingKey, sibling.buffer, sizeof(int));
	
	RecordId siblingRid;
	siblingRid.pid = -1;
	siblingRid.sid = -1;
	memcpy(&siblingRid, sibling.buffer+sizeof(int), sizeof(RecordId));
	
	//Remember not to touch the next node pointer
	//Since we use it later, changing this will destroy the index tree's leaf node mapping
	
	return 0;
}
/*
 * Find the entry whose key value is larger than or equal to searchKey
 * and output the eid (entry number) whose key value >= searchKey.
 * Remeber that all keys inside a B+tree node should be kept sorted.
 * @param searchKey[IN] the key to search for
 * @param eid[OUT] the entry number that contains a key larger than or equalty to searchKey
 * @return 0 if successful. Return an error code if there is an error.
 *//*
RC BTLeafNode::locate(int searchKey, int& eid)
{
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(RecordId) + sizeof(int);
	
	char* temp = buffer;
	
	//Loop through all the indexes in the temp buffer; increment by 12 bytes to jump to next key	
	int i;
	for(i=0; i<getKeyCount()*pairSize; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey
		
		//Once insideKey is larger than or equal to searchKey
		if(insideKey >= searchKey)
		{
			//Set eid to the current byte index divided by size of a pair entry
			//This effectively produces eid
			eid = i/pairSize;
			return 0;
		}
		
		temp += pairSize; //Jump temp over to the next key
	}
	
	//If we get here, all of the keys inside the buffer were less than searchKey
	eid = getKeyCount();
	return 0;
}*/

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
 /*
RC BTLeafNode::readEntry(int eid, int& key, RecordId& rid)
{
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(RecordId) + sizeof(int);
	
	//If eid is out of bounds (negative or more than the number of keys we have), return error
	if(eid >= getKeyCount() || eid < 0)
		return RC_NO_SUCH_RECORD; //Not sure which error to return...RC_INVALID_CURSOR?
	
	//This is the position in bytes of the entry
	int bytePos = eid*pairSize;
	
	char* temp = buffer;
	
	//Copy the data into parameters
	memcpy(&key, temp+bytePos, sizeof(int));
	memcpy(&rid, temp+bytePos+sizeof(int), sizeof(RecordId));
	
	return 0;
}*/

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
 /*
PageId BTLeafNode::getNextNodePtr()
{
	//Initialize a PageId; assume there's no next node by default
	PageId pid = 0; 
	char* temp = buffer;
	
	//Find the last PageId section of the buffer and copy data over to pid
	memcpy(&pid, temp+PageFile::PAGE_SIZE-sizeof(PageId), sizeof(PageId));
	
	return pid;
}
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
 /*
RC BTLeafNode::setNextNodePtr(PageId pid)
{
	//If pid is invalid, return error
	if(pid < 0)
		return RC_INVALID_PID;
	
	char* temp = buffer;
	
	//Otherwise, copy the parameter pid into our buffer
	memcpy(temp+PageFile::PAGE_SIZE-sizeof(PageId), &pid, sizeof(PageId));
	
	return 0;
}*/

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
/*
BTNonLeafNode::BTNonLeafNode()
{
	numKeys=0;
	std::fill(buffer, buffer + PageFile::PAGE_SIZE, 0); //clear the buffer if necessary
}*/

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
 /*
RC BTNonLeafNode::read(PageId pid, const PageFile& pf)
{
	//Use PageFile to read from selected page into buffer
	return pf.read(pid, buffer);
}*/
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
 *//*
int BTNonLeafNode::getKeyCount()
{
	//return numKeys;
	
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(PageId) + sizeof(int);
	int numTotalPairs = (PageFile::PAGE_SIZE-sizeof(PageId))/pairSize; //127
	int count=0;
	//Now we must go through the buffer's sorted keys to see where the new key goes
	//For nonleaf nodes only, remember to skip the first 8 bytes (4 bytes pid, 4 bytes empty)
	char* temp = buffer+8;
	
	//Loop through all the indexes in the temp buffer; increment by 8 bytes to jump to next key
	//1016 is the largest possible index of the next inserted pair (since we already know we can fit another pair)
	//For nonleaf nodes only, remember that we start the (key,pid) entries at index 8
	int i;
	for(i=8; i<=1016; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey
		if(insideKey==0) //Once we hit key of 0, we break
			break;
		//Otherwise, increment count
		count++;

		temp += pairSize; //Jump temp over to the next key
	}
	
	return count;
}*/

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
	//Nonleaf nodes have pairs of integer keys and PageIds, with another PageId at the front
	int pairSize = sizeof(PageId) + sizeof(int);
	int numTotalPairs = (PageFile::PAGE_SIZE-sizeof(PageId))/pairSize; //127
	
	//Return error if no more space in this node
	//Page has 1024 bytes, we need to store 8 bytes (key, pid)
	//That means we can fit 127 with 8 bytes left over; the first 4 will be used for pid
	//Check if adding one more (key, pid) pair will exceed the size limit of 127
	if(getKeyCount()+1 > numTotalPairs)
	{
		//cout << "Cannot insert anymore: this node is full!" << endl;
		return RC_NODE_FULL;
	}

	//Now we must go through the buffer's sorted keys to see where the new key goes
	//For nonleaf nodes only, remember to skip the first 8 bytes (4 bytes pid, 4 bytes empty)
	char* temp = buffer+8;
	
	//Loop through all the indexes in the temp buffer; increment by 8 bytes to jump to next key
	//1016 is the largest possible index of the next inserted pair (since we already know we can fit another pair)
	//For nonleaf nodes only, remember that we start the (key,pid) entries at index 8
	int i;
	for(i=8; i<1016; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey
		
		//Once the insideKey is null or key is smaller than some inside key, we stop
		if(insideKey==0 || !(key > insideKey))
			break;
		
		temp += pairSize; //Jump temp over to the next key
	}
	
	//At this point, variable i holds the index to insert the pair and temp is the buffer at that index
	char* newBuffer = (char*)malloc(PageFile::PAGE_SIZE);
	std::fill(newBuffer, newBuffer + PageFile::PAGE_SIZE, 0); //clear the buffer if necessary
	
	//Copy all values from buffer into newBuffer up until i
	memcpy(newBuffer, buffer, i);
	
	//Copy key and pid into newBuffer
	memcpy(newBuffer+i, &key, sizeof(int));
	memcpy(newBuffer+i+sizeof(int), &pid, sizeof(PageId));
	
	//Copy the rest of the values into newBuffer
	//For nonleaf nodes only, remember that we must add in 8 bytes extra
	//Otherwise we would be counting the initial (pid, empty) as a key
	memcpy(newBuffer+i+pairSize, buffer+i, getKeyCount()*pairSize - i + 8);
	
	
	//Copy newBuffer into buffer, then delete temporary newBuffer to prevent memory leak
	memcpy(buffer, newBuffer, PageFile::PAGE_SIZE);
	free(newBuffer);
	
	//Successfully inserted leaf node, so we increment number of keys
	numKeys++;	
	return 0;
}



RC BTNonLeafNode::locate(int searchKey, int& eid)
{
	int maxNumKeys = getKeyCount();
	int retrieved_key;
	int index;
	for (index = 0; index < maxNumKeys; index++)
	{
		memcpy(&retrieved_key, buffer + (PAGE_ID_SIZE + INTEGER_SIZE + index*(entryPairNonLeafNodeSize)), INTEGER_SIZE);
		//if (retrieved_key >= searchKey)
		if (retrieved_key == searchKey)
		{
			eid = index;
			return 0;
		}
		if (retrieved_key > searchKey)
		{
			eid = index;
			return RC_NO_SUCH_RECORD;
		}
	}
	//eid = index - 1;
	eid = index;
	return RC_NO_SUCH_RECORD;	//failed to find the searchKey
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
			memcpy(&entryKey, buffer + entryPairLeafNodeSize + (eid*entryPairNonLeafNodeSize), INTEGER_SIZE);
			key = entryKey;
			PageId entryId;
			memcpy(&entryId, buffer + entryPairLeafNodeSize + (eid*entryPairNonLeafNodeSize) + INTEGER_SIZE, sizeof(PageId));
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
	//Nonleaf nodes have pairs of integer keys and PageIds, with another PageId at the front
	int pairSize = sizeof(PageId) + sizeof(int);
	int numTotalPairs = (PageFile::PAGE_SIZE-sizeof(PageId))/pairSize; //127
	
	//Only split if inserting will cause an overflow; otherwise, return error
	if(!(getKeyCount() >= numTotalPairs))
		return RC_INVALID_FILE_FORMAT;
	
	//If sibling node is not empty, return error
	if(sibling.getKeyCount()!=0)
		return RC_INVALID_ATTRIBUTE;

	//Clear sibling buffer just in case
	std::fill(sibling.buffer, sibling.buffer + PageFile::PAGE_SIZE, 0); //clear the buffer if necessary

	//Calculate keys to remain in the first half
	int numHalfKeys = ((int)((getKeyCount()+1)/2));

	//Find the index at which to split the node's buffer
	//For nonleaf nodes only, remember to add an offset of 8 for initial pid
	int halfIndex = numHalfKeys*pairSize + 8;
	
	//REMOVING THE MEDIAN KEY
	
	//Find the last key of the first half and the first key of the second half
	int key1 = -1;
	int key2 = -1;
	
	memcpy(&key1, buffer+halfIndex-8, sizeof(int));
	memcpy(&key2, buffer+halfIndex, sizeof(int));
	
	if(key < key1) //key1 is the median key to be removed
	{
		//Copy everything on the right side of halfIndex into sibling's buffer (ignore the pid)
		//For nonleaf nodes only, remember to add an offset of 8 for initial pid
		memcpy(sibling.buffer+8, buffer+halfIndex, PageFile::PAGE_SIZE-halfIndex);
		//Update sibling's number of keys
		sibling.numKeys = getKeyCount() - numHalfKeys;
		
		//Copy down the median key before getting rid of it in buffer
		memcpy(&midKey, buffer+halfIndex-8, sizeof(int));
		
		//Also set the sibling pid from buffer before getting rid of it
		memcpy(sibling.buffer, buffer+halfIndex-4, sizeof(PageId));
		
		//Clear the second half of current buffer; update number of keys
		//Also clear the last key of the first half, since this is the median key
		std::fill(buffer+halfIndex-8, buffer + PageFile::PAGE_SIZE, 0); 
		numKeys = numHalfKeys - 1;
		
		//Insert the (key, pid) pair into buffer, since it's key is smaller than the median
		insert(key, pid);		
	}
	else if(key > key2) //key2 is the median key to be removed
	{
		//Copy everything on the right side of halfIndex EXCEPT FOR THE FIRST KEY into sibling's buffer (ignore the pid)
		//The first key on the right side here is the median key, which will be removed
		//For nonleaf nodes only, remember to add an offset of 8 for initial pid
		memcpy(sibling.buffer+8, buffer+halfIndex+8, PageFile::PAGE_SIZE-halfIndex-8);
		//Update sibling's number of keys
		sibling.numKeys = getKeyCount() - numHalfKeys - 1;
		
		//Copy down the median key before getting rid of it in buffer
		memcpy(&midKey, buffer+halfIndex, sizeof(int));
		
		//Also set the sibling pid from buffer before getting rid of it
		memcpy(sibling.buffer, buffer+halfIndex+4, sizeof(PageId));
		
		//Clear the second half of current buffer; update number of keys
		std::fill(buffer+halfIndex, buffer + PageFile::PAGE_SIZE, 0); 
		numKeys = numHalfKeys;
		
		//Insert the (key, pid) pair into sibling, since it's key is larger than the median
		sibling.insert(key, pid);
		
	}
	else //key is the median key to be removed
	{
		//Copy everything on the right side of halfIndex into sibling's buffer (ignore the pid)
		//For nonleaf nodes only, remember to add an offset of 8 for initial pid
		memcpy(sibling.buffer+8, buffer+halfIndex, PageFile::PAGE_SIZE-halfIndex);
		//Update sibling's number of keys
		sibling.numKeys = getKeyCount() - numHalfKeys;
		
		//Clear the second half of current buffer; update number of keys
		std::fill(buffer+halfIndex, buffer + PageFile::PAGE_SIZE, 0); 
		numKeys = numHalfKeys;
		
		//The key we're inserting IS the median key, so we stop the insertion process and return
		midKey = key;
		
		//Set the sibling pid from the median key parameter
		memcpy(sibling.buffer, &pid, sizeof(PageId));
	}

	//If we reach this, then we have somehow split the node and inserted the (key, pid) pair
	return 0;
}

/*
 * Given the searchKey, find the child-node pointer to follow and
 * output it in pid.
 * @param searchKey[IN] the searchKey that is being looked up.
 * @param pid[OUT] the pointer to the child node to follow.
 * @return 0 if successful. Return an error code if there is an error.
 */
 /*
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
{
	//This is the size in bytes of an entry pair
	int pairSize = sizeof(PageId) + sizeof(int);
	
	//Skip the first 8 offset bytes, since there's no key there
	char* temp = buffer+8;	
	
	//Loop through all the indexes in the temp buffer; increment by 8 bytes to jump to next key	
	int i;	
	for(i=8; i<getKeyCount()*pairSize+8; i+=pairSize)
	{
		int insideKey;
		memcpy(&insideKey, temp, sizeof(int)); //Save the current key inside buffer as insideKey
				
		if(i==8 && insideKey > searchKey) //If searchKey is less than first key, we need to return initial pid
		{
			//A special check is necessarily since the initial pid is in a different buffer position from the rest
			memcpy(&pid, buffer, sizeof(PageId));
			return 0;
		}
		else if(insideKey > searchKey)
		{
			//Set pid to be the left pid (that is, the pid on the small side of insideKey)
			memcpy(&pid, temp-4, sizeof(PageId));
			return 0;
		}
		
		//Otherwise, searchKey is greater than or equal to insideKey, so we keep checking
		temp += pairSize; //Jump temp over to the next key
		
	}
	
	//If we get here, searchKey was greater than all instances of insideKey
	//Copy over the last, right-most pid before returning
	//Remember that temp is now on the next non-existent node's position, so we need to decrement by 4 bytes
	memcpy(&pid, temp-4, sizeof(PageId));
	return 0;
}*/
/*
RC BTNonLeafNode::locateChildPtr(int searchKey, PageId& pid)
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
		memcpy(&left, buffer + INTEGER_SIZE + (index*entryPairNonLeafNodeSize), PAGE_ID_SIZE);	//entry pair + key + all previous entry pairs
		memcpy(&right, buffer + entryPairNonLeafNodeSize + (index*entryPairNonLeafNodeSize), INTEGER_SIZE);	//entry pair + all previous entry pairs
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
*/
/*
 * Initialize the root node with (pid1, key, pid2).
 * @param pid1[IN] the first PageId to insert
 * @param key[IN] the key that should be inserted between the two PageIds
 * @param pid2[IN] the PageId to insert behind the key
 * @return 0 if successful. Return an error code if there is an error.
 */
 /*
RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)
{
	RC error;

	std::fill(buffer, buffer + PageFile::PAGE_SIZE, 0); //clear the buffer if necessary
	
	//This time, don't skip the first 8 offset bytes
	//We're actually initializing it to something explicitly
	char* temp = buffer;
	
	//Copy over the initial pid into buffer
	memcpy(temp, &pid1, sizeof(PageId));
	
	//Copy the first pair into buffer
	//memcpy(temp+8, &key, sizeof(int));
	//memcpy(temp+12, &pid2, sizeof(PageId));
	error = insert(key, pid2);
	
	if(error!=0)
		return error;
	
	//Set number of (key, pid) pairs to 1
	//Only need this if we dont use insert to set (key, pid2) pair
	//numKeys = 1;
	
	return 0;
}*/

RC BTNonLeafNode::initializeRoot(PageId pid1, int key, PageId pid2)	//works!
{ 
	char* buffer_ptr = buffer;
	memcpy(buffer_ptr, &pid1, PAGE_ID_SIZE);
	buffer_ptr += (PAGE_ID_SIZE+INTEGER_SIZE);
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
