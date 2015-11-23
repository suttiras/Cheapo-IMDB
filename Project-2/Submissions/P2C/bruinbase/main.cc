#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeNode.h"
#include <cstdio>
#include "PageFile.h"
#include <assert.h>
#include "BTreeIndex.h"
#include <assert.h>
#include <iostream>
using namespace std;

int main()
{
  // run the SQL engine taking user commands from standard input (console).
  SqlEngine::run(stdin);
	
  return 0;
}
