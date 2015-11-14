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

	//RecordId r1 = 
	//b1->insert()

  return 0;
}
