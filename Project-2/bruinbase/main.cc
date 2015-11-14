#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
#include <cstdio>
#include "PageFile.h"

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  //SqlEngine::run(stdin);

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
	
	PageId p3 = 3;

	RecordId r3 =
	{
		p3,
		3,
	};
	
	int siblingKeyb2;
	
	b1->insertAndSplit(3, r3, *b2, siblingKeyb2);

  return 0;
}
