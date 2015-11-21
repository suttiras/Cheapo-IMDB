/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */
 
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
#include "BTreeIndex.h"
#include <assert.h>
#include <iostream>
using namespace std;


int main()
{
	// run the SQL engine taking user commands from standard input (console).
	SqlEngine::run(stdin);

	//test BTleaf and NonLeaf
/*	cout << "TEST 1 ------------------------------" << endl; 
	BTLeafNode leaf;  
	//cout << "count(0): " << leaf.getKeyCount() << endl;    

	RecordId rid;
	rid.pid = 1;
	rid.sid = 12;
	assert(leaf.insert(1,rid) == 0);

	rid.pid = 2;
	rid.sid = 13;
	assert(leaf.insert(2,rid) == 0);
	
	cout << "max: " << leaf.maxKeyCount() << endl;
	
	rid.pid = 3;
	rid.sid = 14;
	assert(leaf.insert(0,rid) == 0);
	assert(leaf.insert(1,rid) == 0);
	assert(leaf.insert(2,rid) == 0);
	assert(leaf.insert(3,rid) == 0);
	assert(leaf.insert(4,rid) == 0);
	assert(leaf.insert(5,rid) == 0);

	int key;
	leaf.readEntry(6,key,rid);
	//cout << "@Eid 6: " << key << endl;
	leaf.readEntry(7,key,rid);
	//cout << "@Eid 7: " << key << endl;

	cout << "TEST 2 ------------------------------" << endl;

	BTLeafNode leaf2;
	rid.pid = 1;
	rid.sid = 12;
	assert(leaf2.insert(5,rid) == 0);

	rid.pid = 2;
	rid.sid = 13;
	assert(leaf2.insert(4,rid) == 0);

	rid.pid = 10;
	rid.sid = 14;
	assert(leaf2.insert(3,rid) == 0);

	leaf2.readEntry(0,key,rid);
	//cout << "@Eid 0: " << key << endl;
	leaf2.readEntry(1,key,rid);
	//cout << "@Eid 1: " << key << endl;
	leaf2.readEntry(2,key,rid);
	//cout << "@Eid 2: " << key << endl;

	assert(leaf2.insert(0,rid) == 0);
	assert(leaf2.insert(1,rid) == 0);
	assert(leaf2.insert(2,rid) == 0);
	assert(leaf2.insert(3,rid) == 0);
	assert(leaf2.insert(4,rid) == 0);
	assert(leaf2.insert(5,rid) == 0);
	assert(leaf2.insert(6,rid) == 0);
	assert(leaf2.insert(7,rid) == 0);
	
	leaf2.readEntry(8,key,rid);
	//cout << "@Eid 8: " << key << endl;
	leaf2.readEntry(9,key,rid);
	//cout << "@Eid 9: " << key << endl;  


	cout << "TEST 3 ------------------------------" << endl;

	BTLeafNode leaf3;
	rid.pid = 10;
	rid.sid = 13;
	int eid = -1;
	// Locate in empty node
	assert(leaf3.locate(1, eid) != 0); //should fail
	
	assert(leaf3.insert(1,rid) == 0);
	assert(leaf3.locate(1, eid) == 0); //should work now
	//cout << "Locate 1: " << eid << endl;	//should give 0 still
	
	assert(leaf3.locate(2, eid) != 0); //should fail
	//cout << "Locate 2: " << eid << endl;
	assert(eid != 0); //should be one

	assert(leaf3.insert(2,rid) == 0);
	assert(leaf3.locate(2, eid) == 0);
	assert(leaf3.insert(3,rid) == 0);
	assert(leaf3.locate(3, eid) == 0);
	assert(leaf3.locate(2, eid) == 0);
	assert(eid == 1);
	
	//next nodeptr test
	assert(leaf3.setNextNodePtr(100) == 0);
	assert(leaf3.getNextNodePtr() == 100);
		
	cout << "TEST 4 ------------------------------" << endl;
	BTLeafNode leaf4;
	assert(leaf4.insert(4,rid) == 0); //entry 1
	assert(leaf4.locate(3,eid) == 0);
	
	assert(leaf4.insert(9,rid) == 0); //entry 2
	assert(leaf4.locate(3,eid) == 0);
	assert(eid == 0);
	assert(leaf4.insert(5,rid) == 0);	//entry 3
	assert(leaf4.locate(8,eid) == 0);
	assert(eid == 2);

	assert(leaf4.locate(6,eid) == 0);
	assert(eid == 2);
	
	assert(leaf4.locate(9,eid) == 0);
	assert(eid == 2);
	
	assert(leaf4.locate(10,eid) != 0);
	assert(eid == 3);

	//4 5 9
	BTLeafNode leaf4split;
	int sibkey;
	leaf4.insertAndSplit(100,rid,leaf4split,sibkey);
	cout << "sibling key: " << sibkey << endl;


	cout << "newcount leaf4: " << leaf4.getKeyCount() << endl;
	cout << "newcount leaf4split: " << leaf4split.getKeyCount() << endl;	
	
	leaf4.readEntry(0,key,rid);
	cout << "@Eid 0: " << key << endl;
	assert(key == 4);
	assert(leaf4.readEntry(1,key,rid) != 0);
	
	leaf4split.readEntry(0,key,rid);
	cout << "@Eid 0: " << key << endl;
	leaf4split.readEntry(1,key,rid);
	cout << "@Eid 1: " << key << endl;  
	leaf4split.readEntry(2,key,rid);
	cout << "@Eid 2: " << key << endl;  
	assert(leaf4split.readEntry(4,key,rid) != 0);
	assert(key == 100);
	cout << "TEST 5 ------------------------------" << endl; 
	BTNonLeafNode leaf5;  
	//cout << "count(0): " << leaf5.getKeyCount() << endl;    


	PageId pid = 100;
	PageId pid2 = 200;
	key = 1;
	assert(leaf5.initializeRoot(pid,key,pid2) == 0);
	//cout << "count(1): " << leaf5.getKeyCount() << endl;  
	
	pid = -1;
	assert(leaf5.locateChildPtr(0,pid) == 0);
	//cout << "locateptr 0: " << pid << endl;
	assert(pid == 100);
	
	assert(leaf5.locateChildPtr(2,pid) == 0);
	//cout << "locateptr 2: " << pid << endl;	
	assert(pid == 200);
		

	assert(leaf5.insert(4,300) == 0);

	assert(leaf5.locateChildPtr(3,pid) == 0);
	//cout << "locateptr 3: " << pid << endl;	
	assert(pid == 200);
	
	cout << "max: " << leaf5.maxKeyCount() << endl;
	

	assert(leaf5.locate(0,eid) == 0);
	assert(eid == 0);
	
	assert(leaf5.locate(10,eid) != 0);
	assert(eid == 2);

	//100 0 200 4 300 10 400
	assert(leaf5.insert(10,400) == 0);
	
	BTNonLeafNode leaf5split; 
	int midkey;
	leaf5.insertAndSplit(100,1000,leaf5split,midkey);
	cout << "mid key: " << midkey << endl;


	cout << "newcount leaf5: " << leaf5.getKeyCount() << endl;
	cout << "newcount leaf5split: " << leaf5split.getKeyCount() << endl;	
	
	leaf5.locateChildPtr(-1,pid);
	cout << "@pid -1: " << pid << endl;
	assert(pid == 100);
	
	leaf5.locateChildPtr(1,pid);
	cout << "@pid 1: " << pid << endl;
	assert(pid == 200);
	
	leaf5split.locateChildPtr(1,pid);
	cout << "@pidsplit 1: " << pid << endl;
	assert(pid == 200);
	
	leaf5split.locateChildPtr(7,pid);
	cout << "@pidsplit 7: " << pid << endl;
	assert(pid == 300);	

	leaf5split.locateChildPtr(60,pid);
	cout << "@pidsplit 60: " << pid << endl;
	assert(pid == 400);
*/





//test BTreeIndex
/*	RecordId rid;
	rid.pid = 10;
	rid.sid = 12;
	IndexCursor cursor;
	
	
	BTreeIndex index = BTreeIndex();
	assert(index.open("testingindex",'w') == 0);
	for(int i = 1; i <= 86; i++){
		assert(index.insert(i,rid) != -1);
		assert(index.locate(i,cursor) == 0);
		cout << "cursor pid: " << cursor.pid << " | cursor eid: " << cursor.eid << endl << "------" << endl;
	}
	assert(index.close() == 0);
	return 0;
*/

}
