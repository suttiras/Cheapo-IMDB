#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
#include <cstdio>
#include "PageFile.h"

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  //SqlEngine::run(stdin);


	//leaf node test cases
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

	PageId p0 = 0;

	RecordId r0 =
	{
		p0,
		0,
	};

	int key0 = 0;

	b1->insert(key0, r0);

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
	*/

	//int key3 = 3;

	//b1->insert(key3, r3);

	//b1->print();
	
	BTNonLeafNode* nb1 = new BTNonLeafNode();
	//nb1->print();

	PageId np2 = 2;

	int nkey2 = 2;
	//nb1->insert(nkey2, np2);

	//nb1->print();
	
	PageId np1 = 0;

	//nb1->insert(nkey2, np2);

	//nb1->insert(nkey3, np3);
	
	int nkey1 = 0;

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

	//nb1->print();
	//nb2->print();
	
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
	nb7->print();
	nb8->print();
	
	BTNonLeafNode* nb5 = new BTNonLeafNode();
	nb5->insert(nkey1, np1);
	nb5->insert(nkey2, np2);
	nb5->insert(nkey4, np4);
	
	BTNonLeafNode* nb6 = new BTNonLeafNode();
	nb5->insertAndSplit(nkey3, np3, *nb6, midKey);	//case 3

	nb5->print();
	nb6->print();
	
  return 0;
}
