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

	PageId p4 = 4;

	RecordId r4 =
	{
		p4,
		4,
	};

	int key4 = 4;

	b1->insert(key4, r4);

	b1->print();

	PageId p3 = 3;

	RecordId r3 =
	{
		p3,
		3,
	};

	int key3 = 3;

	b1->insert(key3, r3);

	b1->print();

  return 0;
}
