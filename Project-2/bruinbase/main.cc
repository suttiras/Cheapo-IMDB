#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
#include <cstdio>
#include "PageFile.h"
#include <assert.h>
#include "BTreeIndex.h"
#include <assert.h>
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
using namespace std;

int main()
{
	// run the SQL engine taking user commands from standard input (console).
	SqlEngine::run(stdin);
	/*
	BTLeafNode* b1 = new BTLeafNode();
	b1->print();

	PageId p2 = 2;

	RecordId r2 =
	{
	p2,
	2,
	};

	int key2 = 2;
	b1->insert(key2, r2);

	b1->print();

	PageId p1 = 1;

	RecordId r1 =
	{
	p1,
	1,
	};

	int key1 = 1;

	b1->insert(key1, r1);

	b1->print();

	BTLeafNode* b2 = new BTLeafNode();

	PageId p4 = 4;

	RecordId r4 =
	{
	p4,
	4,
	};

	int key4 = 4;

	//b1->insert(key4, r4);

	//b1->print();

	PageId p3 = 3;

	RecordId r3 =
	{
	p3,
	3,
	};

	int siblingKeyb2;

	b1->insertAndSplit(3, r3, *b2, siblingKeyb2);
	b1->print();
	b2->print();
	int eid;
	b1->locate(4, eid);	//should be 1

	//int key3 = 3;

	//b1->insert(key3, r3);

	//b1->print();

	BTNonLeafNode* nb1 = new BTNonLeafNode();
	nb1->print();

	PageId np2 = 2;

	int nkey2 = 2;
	//nb1->insert(nkey2, np2);

	nb1->print();

	PageId np1 = 1;

	//nb1->insert(nkey2, np2);

	//nb1->insert(nkey3, np3);

	int nkey1 = 1;

	//nb1->print();

	nb1->insert(nkey1, np1);

	//nb1->print();

	int midKey;
	BTNonLeafNode* nb2 = new BTNonLeafNode();

	PageId np3 = 3;

	int nkey3 = 3;

	PageId np4 = 4;

	int nkey4 = 4;

	//nb1->insert(nkey4, np4);
	nb1->insert(nkey3, np3);
	nb1->insert(nkey4, np4);
	nb1->insertAndSplit(nkey2, np2, *nb2, midKey);	//midKey = 2

	//nb1->insertAndSplit(nkey3, np3, *nb2, midKey);	//midKey = 2

	BTNonLeafNode* nb4 = new BTNonLeafNode();
	nb4->insert(nkey1, np1);
	nb4->insert(nkey3, np3);
	nb4->insert(nkey4, np4);

	BTNonLeafNode* nb3 = new BTNonLeafNode();
	nb4->insertAndSplit(nkey2, np2, *nb3, midKey);	//case 1

	BTNonLeafNode* nb7 = new BTNonLeafNode();
	nb7->insert(nkey1, np1);
	nb7->insert(nkey2, np2);

	BTNonLeafNode* nb8 = new BTNonLeafNode();
	nb7->insertAndSplit(nkey3, np3, *nb8, midKey);	//case 2

	BTNonLeafNode* nb5 = new BTNonLeafNode();
	nb5->insert(nkey1, np1);
	nb5->insert(nkey2, np2);
	nb5->insert(nkey4, np4);

	BTNonLeafNode* nb6 = new BTNonLeafNode();
	nb5->insertAndSplit(nkey3, np3, *nb6, midKey);	//case 3

	nb1->print();
	nb2->print();
	*/

	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	//test BTleaf and NonLeaf

	/*
	cout << "TEST 1 ------------------------------" << endl;
	BTLeafNode leaf;
	//cout << "count(0): " << leaf.getKeyCount() << endl;

	RecordId rid;
	rid.pid = 1;
	rid.sid = 12;
	assert(leaf.insert(1,rid) == 0);

	rid.pid = 2;
	rid.sid = 13;
	assert(leaf.insert(2,rid) == 0);

	//cout << "max: " << leaf.maxKeyCount() << endl;

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
	assert(leaf4.locate(3,eid) != 0);	//made a mistake?

	assert(leaf4.insert(9,rid) == 0); //entry 2
	assert(leaf4.locate(3,eid) != 0);
	assert(eid == 0);
	assert(leaf4.insert(5,rid) == 0);	//entry 3
	assert(leaf4.locate(8,eid) != 0);
	assert(eid == 2);

	assert(leaf4.locate(6,eid) != 0);
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
	cout << "@leaf4 Eid 0: " << key << endl;
	assert(key == 4);
	assert(leaf4.readEntry(1,key,rid) == 0);

	leaf4split.readEntry(0,key,rid);
	cout << "leaf4split @Eid 0: " << key << endl;
	leaf4split.readEntry(1,key,rid);
	cout << "leaf4split @Eid 1: " << key << endl;
	//leaf4split.readEntry(2,key,rid);
	//cout << "@Eid 2: " << key << endl;
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

	//cout << "max: " << leaf5.maxKeyCount() << endl;


	assert(leaf5.locate(0,eid) != 0);
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
	assert(pid == 400);

	leaf5split.locateChildPtr(7,pid);
	cout << "@pidsplit 7: " << pid << endl;
	assert(pid == 400);

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





	//test BTreeIndex
	/*
	RecordId rid;
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

	//TESTING BELOW IS FOR BTreeNode

	/*
	cout << sizeof(RecordId) + sizeof(int) << endl; //size 12
	cout << sizeof(PageId) << endl; //size 4



	//Initialize new leaf node
	BTLeafNode poop;
	cout << "Initial key count: " << poop.getKeyCount() << endl;

	int zeroPls;


	//Try stuffing our node with (key, rid) pairs until node is full
	RecordId poopRid = { 1, 2 };
	for (int i = 0; i<86; i++)
	{
	zeroPls = poop.insert(5, poopRid);
	cout << "Am I zero? " << zeroPls << endl;
	}

	cout << "Final key count: " << poop.getKeyCount() << endl;



	//Test location by finding key with 5
	int eid = -1;
	poop.locate(5, eid);
	cout << "Found key=5 at eid: " << eid << endl;

	//Test location by finding key with 6 (doesn't exist - should be -1)
	int eid2 = -1;
	poop.locate(6, eid2);
	cout << "Found key=6 at eid2: " << eid2 << endl;

	//Test readEntry by reading in key=5's key/rid
	int key = -1;
	RecordId rid;
	rid.pid = -1;
	rid.sid = -1;
	poop.readEntry(0, key, rid);
	cout << "Entry at specified eid has key=" << key << " and pid=" << rid.pid << " and sid=" << rid.sid << endl;

	//Test readEntry for more indexes
	poop.readEntry(1, key, rid);
	cout << "Entry at specified eid has key=" << key << " and pid=" << rid.pid << " and sid=" << rid.sid << endl;
	poop.readEntry(85, key, rid);
	cout << "Entry at specified eid has key=" << key << " and pid=" << rid.pid << " and sid=" << rid.sid << endl;

	//Test next pointers
	PageId nextPid = poop.getNextNodePtr(); //should return an error
	cout << "The next (invalid) pid is: " << nextPid << endl;

	poop.setNextNodePtr(999);
	cout << "After setting it, the next pid is: " << poop.getNextNodePtr() << endl;
	//Test insertAndSplit by putting in a new (key, rid) pair
	BTLeafNode poop2;
	int poop2Key = -1;
	poop.insertAndSplit(100, ((RecordId){101, 102}), poop2, poop2Key);
	cout << "The first entry in poop2 has key (should be 5): " << poop2Key << endl;

	cout << "poop has numKeys " << poop.getKeyCount() << " and poop2 has numKeys " << poop2.getKeyCount() << endl;

	//Nothing should change if we don't meet the conditions to split
	zeroPls = poop.insertAndSplit(100, (RecordId){ 101, 102 }, poop2, poop2Key);
	cout << "poop has numKeys " << poop.getKeyCount() << " and poop2 has numKeys " << poop2.getKeyCount() << " (should be same)" << endl;
	cout << "zeroPls should not be zero pls: " << zeroPls << endl;

	cout << "--------------------------------------------------" << endl;
	//----------------------------------------------------------------------------------------------------

	BTLeafNode blah;
	BTLeafNode blah2;
	blah.insert(1, (RecordId){12,23});
	blah.insert(10, (RecordId){34,45});
	blah.insert(100, (RecordId){56,67});
	blah.insert(1000, (RecordId){78,89});

	//Do a quick test on locate
	int blahEid = -1;
	blah.locate(0, blahEid);
	cout << "0 belongs at eid: " << blahEid << endl;
	blah.locate(1, blahEid);
	cout << "1 belongs at eid: " << blahEid << endl;
	blah.locate(5, blahEid);
	cout << "5 belongs at eid: " << blahEid << endl;
	blah.locate(15, blahEid);
	cout << "15 belongs at eid: " << blahEid << endl;
	blah.locate(150, blahEid);
	cout << "150 belongs at eid: " << blahEid << endl;
	blah.locate(1500, blahEid);
	cout << "1500 belongs at eid: " << blahEid << endl;

	//Set up insertAndSplit on leaf nodes test
	int key2 = -1;
	RecordId rid2;
	rid2.pid = -1;
	rid2.sid = -1;

	blah.readEntry(0, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;

	blah.readEntry(1, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;

	blah.readEntry(2, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;

	blah.readEntry(3, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;
	//This should stop after we fill up the entire blah buffer (produces maybe 4 errors or so)
	for(int j=0; j<85; j++)
	blah.insert(101, (RecordId){0,0});

	int blah2Key = -1;
	blah.insertAndSplit(99, (RecordId){9000, 9001}, blah2, blah2Key);

	cout << "blah has numKeys " << blah.getKeyCount() << " and blah2 has numKeys " << blah2.getKeyCount() << endl;
	cout << "blah2's first entry is now key: " << blah2Key << endl;

	int eid3 = -1;
	blah.locate(99, eid3);

	cout << "blah contains key=99 at eid: " << eid3 << endl;

	blah.readEntry(0, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;

	blah.readEntry(1, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;

	blah.readEntry(2, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;

	blah.readEntry(3, key2, rid2);
	cout << "Entry at specified eid has key=" << key2 << " and pid=" << rid2.pid << " and sid=" << rid2.sid << endl;

	cout << "--------------------------------------------------" << endl;
	//----------------------------------------------------------------------------------------------------

	BTNonLeafNode root;

	//Initialize root and verify number of keys
	root.initializeRoot(0,99,1);
	cout << "root node has numKeys: " << root.getKeyCount() << endl;

	//Insert to root and verify number of keys
	root.insert(999, 2);
	cout << "After insert, root node has numKeys: " << root.getKeyCount() << endl;

	//Try to insertAndSplit (this should fail)
	BTNonLeafNode sibling;
	int median = -1;
	root.insertAndSplit(9999, 3, sibling, median);
	cout << "After insertAndSplit, root node has numKeys (should be same): " << root.getKeyCount() << endl;
	cout << "Median: " << median << endl;

	//Check child pointers
	PageId rootPid = -1;

	root.locateChildPtr(50, rootPid);
	cout << "50 has child pointer: " << rootPid << endl;

	root.locateChildPtr(500, rootPid);
	cout << "500 has child pointer: " << rootPid << endl;
	rootPid = -1;
	root.locateChildPtr(5000, rootPid);
	cout << "5000 has child pointer: " << rootPid << endl;

	for(int k=0; k<5; k++)
	root.insert(9,4);

	cout << "root node has numKeys: " << root.getKeyCount() << endl;

	for(int k=0; k<121; k++)
	root.insert(9999,5);

	cout << "root node has numKeys: " << root.getKeyCount() << endl;

	root.insertAndSplit(99999, 6, sibling, median);
	cout << "After insertAndSplit, root node has numKeys: " << root.getKeyCount() << endl;
	cout << "After insertAndSplit, sibling node has numKeys: " << sibling.getKeyCount() << endl;
	cout << "Median: " << median << endl;

	//Let's test for median more accurately
	BTNonLeafNode root2;
	root2.initializeRoot(0,2,1);
	for(int k=0; k<127; k++)
	root2.insert(2*(k+2),2);
	//Currently, root2 looks something like  (2, 4, 6, 8, ..., 126, 128, 130, ..., 252, 254)
	//Median should be 128 if we insert 127 [reason: adding 127 bumps 128 up into the middle key]
	//Median should be 129 if we insert 129 [reason: 129 itself becomes the middle most key]
	//Median should be 130 if we insert 131 [reason: adding 131 bumps 130 down into the middle key]

	median = -1;
	BTNonLeafNode sibling2;
	root2.insertAndSplit(131, 6, sibling2, median); //Replace the key to be inserted with the numbers above to test
	cout << "After insertAndSplit, root2 node has numKeys: " << root2.getKeyCount() << endl;
	cout << "After insertAndSplit, sibling2 node has numKeys: " << sibling2.getKeyCount() << endl;
	cout << "Median: " << median << endl;

	cout << "--------------------2C TESTING------------------------" << endl;
	//----------------------------------------------------------------------------------------------------
	//int maxEid = (PageFile::PAGE_SIZE-sizeof(PageId))/(sizeof(RecordId)+sizeof(int)); //This produces 85
	cout << sizeof(PageFile) << endl;
	cout << sizeof(PageId) << endl;
	cout << sizeof(int) << endl;
	PageFile pf;
	pf.open("test", 'w');
	cout << "pf.endPid() on initialization: " << pf.endPid() << endl;
	//check for endPid changes
	BTLeafNode thisLeaf;
	for(int i=0; i<85; i++)
	thisLeaf.insert(1, (RecordId) {1,1});
	cout << "thisLeaf has key count: " << thisLeaf.getKeyCount() << endl;

	cout << "pf.endPid() after insert: " << pf.endPid() << endl;

	//Try inserting leaf node
	//If succesful, write back into PageFile
	cout << "thisLeaf write: " << thisLeaf.write(1, pf) << endl;
	cout << "pf.endPid() after thisLeaf write: " << pf.endPid() << endl;
	//Try inserting leaf node via splitting
	BTLeafNode anotherLeaf;
	int anotherKey;
	thisLeaf.insertAndSplit(2, (RecordId) {2,2}, anotherLeaf, anotherKey);
	cout << "thisLeaf has key count: " << thisLeaf.getKeyCount() << endl;
	cout << "anotherLeaf has key count: " << anotherLeaf.getKeyCount() << endl;
	cout << "pf.endPid() after insert and split: " << pf.endPid() << endl;
	//Write new contents into thisLeaf and anotherLeaf
	//Notice that anotherLeaf starts writing at the end of the last pid
	cout << "anotherLeaf write: " << anotherLeaf.write(pf.endPid(), pf) << endl;
	cout << "pf.endPid() after anotherLeaf write: " << pf.endPid() << endl;
	thisLeaf.setNextNodePtr(pf.endPid());
	cout << "pf.endPid() after setting thisLeaf's next node ptr: " << pf.endPid() << endl;
	pf.close();
	pf.open("test", 'r');
	BTLeafNode readTest;
	cout << "readTest: " << readTest.read(1, pf) << endl;
	BTLeafNode readTest2;
	cout << "readTest2: " << readTest2.read(2, pf) << endl;
	cout << "readTest has key count: " << readTest.getKeyCount() << endl;
	cout << "readTest2 has key count: " << readTest2.getKeyCount() << endl;
	*/

	//----------------------------------------------------------------------------------------------------  


	/*
	SOME test cases for read and write:
	1) write a leaf and read it
	2) write a leaf with a next pointer and read it
	3) write a non-leaf with X keys and X id's and read it
	- I assume we will never use a non-leaf with X keys and X id's based on how we create the tree though, so this test may be unnecessary
	4) write a non-leaf with X keys and X + 1 id's and read it
	5) write a root and read it
	SOME test cases for insert:
	1) insert into an empty tree
	2) insert into a tree without resulting in overflow
	3) insert into a tree resulting in leaf overflow
	4) insert into a tree resulting in non-leaf overflow
	5) insert into a tree resulting in having to create a new root
	*/

	/*
	cout << "Testing read/write for LeafNodes" << endl;
	PageFile pfRW;
	pfRW.open("testPFRW.idx", 'w');
	PageFile pfRW2;
	pfRW2.open("testPFRW2.idx", 'w');
	BTLeafNode lRW, lRW2;
	BTNonLeafNode nlRW, nlRW2;
	lRW.insert(1, (RecordId) {1,1});
	lRW.insert(2, (RecordId) {2,2});
	lRW.insert(3, (RecordId) {3,3});
	lRW.insert(4, (RecordId) {4,4});
	cout << lRW.insert(5, (RecordId) {5,5}) << endl;
	cout << "pfRW.endPid() on initialization: " << pfRW.endPid() << endl;
	cout << "lRW has #keys: " << lRW.getKeyCount() << endl;
	cout << lRW.write(1, pfRW) << endl;
	cout << "pfRW.endPid() after write: " << pfRW.endPid() << endl;
	int key1=-100;
	RecordId rid1;
	char bufferTest[PageFile::PAGE_SIZE];
	cout << pfRW.read(1, bufferTest) << endl;
	//This should produce 111222333444555
	for(int i=0; i<5; i++)
	{
	if(i==0)
	{
	memcpy(&key1, bufferTest, sizeof(int));
	memcpy(&rid1, bufferTest+4, sizeof(RecordId));
	cout << "key1 is: " << key1 << endl;
	cout << "pid1 is: " << rid1.pid << endl;
	cout << "sid1 is: " << rid1.sid << endl;
	}
	else
	{
	memcpy(&key1, bufferTest+(12*i), sizeof(int));
	memcpy(&rid1, bufferTest+4+(12*i), sizeof(RecordId));
	cout << "key1 is: " << key1 << endl;
	cout << "pid1 is: " << rid1.pid << endl;
	cout << "sid1 is: " << rid1.sid << endl;
	}
	}

	cout << lRW2.read(1, pfRW) << endl;
	cout << "lRW2 has #keys: " << lRW2.getKeyCount() << endl;
	int eid1=-100;
	for(int i=0; i<5; i++)
	{
	lRW2.locate(i+1, eid1);
	cout << "eid1 is: " << eid1 << endl;
	}
	//repeat the same thing for non leaf nodes
	cout << "Testing read/write for NonLeafNodes" << endl;
	nlRW.insert(1, 1);
	nlRW.insert(2, 2);
	nlRW.insert(3, 3);
	nlRW.insert(4, 4);
	cout << nlRW.insert(5, 5) << endl;
	cout << "pfRW2.endPid() on initialization: " << pfRW2.endPid() << endl;
	cout << "nlRW has #keys: " << nlRW.getKeyCount() << endl;
	cout << nlRW.write(1, pfRW2) << endl;
	cout << "pfRW2.endPid() after write: " << pfRW2.endPid() << endl;
	cout << nlRW2.read(1, pfRW2) << endl;
	cout << "nlRW2 has #keys: " << nlRW2.getKeyCount() << endl;
	//clear the terminal
	//cout << string(50, '\n');
	*/


/*
	//Initialize new leaf node
	BTLeafNode poop;
	cout << "Initial key count: " << poop.getKeyCount() << endl;

	int zeroPls;

	//Try stuffing our node with (key, rid) pairs until node is full
	//RecordId poopRid;
	for (int i = 0; i<86; i++)
	{
		RecordId poopRid = { i, i + 1 };
		zeroPls = poop.insert(i + 100, poopRid);
		cout << "Am I zero? " << zeroPls << endl;
	}
	
	cout << "Final key count: " << poop.getKeyCount() << endl;
	
	poop.print();
	
	//Test insertAndSplit by putting in a new (key, rid) pair
	BTLeafNode poop2;
	int poop2Key = -1;
	poop.insertAndSplit(99, ((RecordId){100, 101}), poop2, poop2Key);
	*/
	/*
	cout << "Poop: ";
	poop.print();

	cout << "\n";
	*/
/*
	cout << "The first entry in poop2 has key (should be 5?): " << poop2Key << endl;

	cout << "poop has numKeys " << poop.getKeyCount() << " and poop2 has numKeys " << poop2.getKeyCount() << endl;

	//Nothing should change if we don't meet the conditions to split
	zeroPls = poop.insertAndSplit(98, (RecordId){ 99, 100 }, poop2, poop2Key);
	*/
	/*
	cout << "Poop: ";
	poop.print();

	cout << "\n";
	cout << "Poop2: ";
	poop2.print();
	cout << "\n";
	*/
/*
	cout << "poop has numKeys " << poop.getKeyCount() << " and poop2 has numKeys " << poop2.getKeyCount() << " (should be same)" << endl;
	cout << "zeroPls should not be zero pls: " << zeroPls << endl;
	*/
	/*
	poop.print();
	poop2.print();
	*/


/*
	int key;
	BTLeafNode leaf;
	BTLeafNode siblingLeaf;
	for(int i=1; i<100; i++)
	leaf.insert(i, (RecordId){i,i});

	leaf.print();
	leaf.insertAndSplit(100, (RecordId){111,111}, siblingLeaf, key);

	cout << "leaf's #keys" << leaf.getKeyCount() << endl;
	leaf.print();
	cout << "MEDIAN KEY: " << key << endl;
	siblingLeaf.print();
	
	BTNonLeafNode nonLeaf;
	BTNonLeafNode siblingNonLeaf;
	for(int i=1; i<135; i++)
	nonLeaf.insert(i, i);
	cout << "nonLeaf's #keys: " << nonLeaf.getKeyCount() << endl;
	nonLeaf.insertAndSplit(1111, 1111, siblingNonLeaf, key);

	cout << "KEY: " << key << endl;
	cout << "nonLeaf's #keys: " << nonLeaf.getKeyCount() << endl;
	nonLeaf.print();
	cout << "siblingNonLeaf's #keys: " << siblingNonLeaf.getKeyCount() << endl;
	siblingNonLeaf.print();
	*/


/*
	IndexCursor c;
	//test BTreeIndex
	BTreeIndex test;
	test.open("testIndex.idx", 'w');
	for (int i=0; i<200; i++)
	test.insert(i, (RecordId) {i, i});

	cout << "" << endl;
	cout << "did it work: " << test.locate(1, c) << endl;
	cout << "1: " << c.eid << " / " << c.pid << endl;
	cout << "did it work: " << test.locate(20, c) << endl;
	cout << "9: " << c.eid << " / " << c.pid << endl;
	cout << "did it work: " << test.locate(500, c) << endl;
	cout << "500: " << c.eid << " / " << c.pid << endl;

	//print out totals for testing purposes
	cout << "rootPid: " << test.getRootPid() << endl;
	cout << "treeHeight: " << test.getTreeHeight() << endl;

	//test.insert(2342, (RecordId) {1,0});

	test.close();
	BTreeIndex test2;
	test2.open("testIndex2.idx", 'w');
	cout << "test2 treeHeight: " << test2.getTreeHeight() << endl;
	test2.insert(272, (RecordId) {0,0});
	cout << "test2 treeHeight: " << test2.getTreeHeight() << endl;
	cout << "did it work: " << test2.locate(272, c) << endl;
	cout << "272: " << c.eid << " / " << c.pid << endl;
	test2.insert(2342, (RecordId) {0,1});
	cout << "did it work: " << test2.locate(2342, c) << endl;
	cout << "2342: " << c.eid << " / " << c.pid << endl;

	cout << "test2 treeHeight: " << test2.getTreeHeight() << endl;
	test2.close();
	*/
	

/*

	IndexCursor c;
	//test BTreeIndex
	BTreeIndex test;
	test.open("testIndex.idx", 'w');
	for (int i=1; i<200; i++)
	test.insert(i, (RecordId) {i, i});

	cout << "" << endl;

	//print out totals for testing purposes
	cout << "rootPid: " << test.getRootPid() << endl;
	cout << "treeHeight: " << test.getTreeHeight() << endl;

	//test.insert(2342, (RecordId) {1,0});

	test.close();
	
	*/
	return 0;
}
