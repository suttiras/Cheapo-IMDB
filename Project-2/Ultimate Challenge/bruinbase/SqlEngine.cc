/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

extern FILE* sqlin;
int sqlparse(void);


RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  sqlin = commandline;
  sqlparse(); 
  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;
  RecordId   rid; 
  IndexCursor c;
  BTreeIndex tree;
	
  RC     rc;
  int    key;     
  string value;
  int    diff;
  int    count = 0;
  
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
	fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
	return rc;
  }
  
	SelCond temp;
	bool hasCond = false;
	bool isGE = false;
	bool isLE = false;
	bool valueCondExists = false;
	bool reqIndex = false;
	int maximum = -1;
	int minimum = -1;
	int equalValue = -1;
	
	int condIndex = -1;
	
	bool valueConflictExists = false;
	std::string valueStorage = "";

	for(int i=0; i<cond.size(); i++)
	{
		temp = cond[i];
		int tempVal = atoi(temp.value);

		if(temp.attr == 1 && temp.comp != SelCond::NE)
		{
			hasCond = true;
			
			if(temp.comp == SelCond::EQ)
			{
				equalValue = tempVal;
				condIndex = i;
				break;
			}
			else if(temp.comp == SelCond::GE)
			{
				if(tempVal > minimum || minimum == -1)
				{
					isGE = true;
					minimum = tempVal;
				}
			}
			else if(temp.comp == SelCond::GT)
			{
				if(tempVal >= minimum || minimum == -1)
				{
					isGE = false;
					minimum = tempVal;
				}
			}
			else if(temp.comp == SelCond::LE)
			{
				if(tempVal < maximum || maximum == -1)
				{
					isLE = true;
					maximum = tempVal;
				}
			}
			else if(temp.comp == SelCond::LT)
			{
				if(tempVal <= maximum || maximum == -1)
				{
					isLE = false;
					maximum = tempVal;
				}
			}
		}
		else if(temp.attr == 2)
		{
			valueCondExists = true;
			
			if(temp.comp == SelCond::EQ)
			{
				if(valueStorage=="" || strcmp(value.c_str(), cond[i].value)==0)
					valueStorage=tempVal;
				else
					valueConflictExists = true;
			}
		}
	}
	
	if(valueConflictExists || (maximum!=-1 && minimum!=-1 && maximum<minimum))
		goto end_select_early;

	if(maximum!=-1 && minimum!=-1 && !isGE && !isLE && maximum==minimum)
		goto end_select_early;

  if(tree.open(table + ".idx", 'r')!=0 || (!hasCond && attr!=4))
  {
	  rid.pid = rid.sid = 0;
	  count = 0;
	  while (rid < rf.endRid()) {
		if ((rc = rf.read(rid, key, value)) < 0) {
		  fprintf(stderr, "Error: reading a tuple from table %s\n", table.c_str());
		  goto exit_select;
		}

		for (unsigned i = 0; i < cond.size(); i++) {
		  switch (cond[i].attr) {
		  case 1:
		diff = key - atoi(cond[i].value);
		break;
		  case 2:
		diff = strcmp(value.c_str(), cond[i].value);
		break;
		  }
		  switch (cond[i].comp) {
			  case SelCond::EQ:
				if (diff != 0) goto next_tuple;
				break;
			  case SelCond::NE:
				if (diff == 0) goto next_tuple;
				break;
			  case SelCond::GT:
				if (diff <= 0) goto next_tuple;
				break;
			  case SelCond::LT:
				if (diff >= 0) goto next_tuple;
				break;
			  case SelCond::GE:
				if (diff < 0) goto next_tuple;
				break;
			  case SelCond::LE:
				if (diff > 0) goto next_tuple;
				break;
		  }
		}
		count++;

		switch (attr) {
			case 1: 
			  fprintf(stdout, "%d\n", key);
			  break;
			case 2: 
			  fprintf(stdout, "%s\n", value.c_str());
			  break;
			case 3: 
			  fprintf(stdout, "%d '%s'\n", key, value.c_str());
			  break;
		}
		next_tuple:
		++rid;
	  }
  }
  else
  {
	count = 0;
	rid.pid = rid.sid = 0;
	reqIndex = true;
	
	if(equalValue!=-1) 
		tree.locate(equalValue, c);
	else if(minimum!=-1 && !isGE)
		tree.locate(minimum+1, c);
	else if(minimum!=-1 && isGE)
		tree.locate(minimum, c);
	else
		tree.locate(0, c);
	
	while(tree.readForward(c, key, rid)==0)
	{
		if(!valueCondExists && attr==4)
		{
			if(equalValue!=-1 && key!=equalValue)
				goto end_select_early;
			
			if(maximum!=-1)
			{
				if(isLE && key>maximum)
					goto end_select_early;
				else if(!isLE && key>=maximum)
					goto end_select_early;
			}
			
			if(minimum!=-1)
			{
				if(isGE && key<minimum)
					goto end_select_early;
				else if(!isGE && key<=minimum)
					goto end_select_early;
			}
			
			count++;
			continue;
		}
	
		if ((rc = rf.read(rid, key, value)) < 0) {
		  fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
		  goto exit_select;
		}

		for (unsigned i = 0; i < cond.size(); i++)
		{
			switch (cond[i].attr)
			{
				case 1:
					diff = key - atoi(cond[i].value);
					break;
				case 2:
					diff = strcmp(value.c_str(), cond[i].value);
					break;
			}

			switch (cond[i].comp)
			{
				case SelCond::EQ:
					if (diff != 0)
					{
						if(cond[i].attr==1)
							goto end_select_early;
						goto continue_while;
					}
					break;
				case SelCond::NE:
					if (diff == 0) goto continue_while;
					break;
				case SelCond::GT:
					if (diff <= 0) goto continue_while;
					break;
				case SelCond::LT:
					if (diff >= 0)
					{
						if(cond[i].attr==1)
							goto end_select_early;
						goto continue_while;
					}
					break;
				case SelCond::GE:
					if (diff < 0) goto continue_while;
					break;
				case SelCond::LE:
					if (diff > 0)
					{
						if(cond[i].attr==1)
							goto end_select_early;
						goto continue_while;
					}
					break;
			}
		}
		count++;

		switch (attr)
		{
			case 1:
			  fprintf(stdout, "%d\n", key);
			  break;
			case 2:
			  fprintf(stdout, "%s\n", value.c_str());
			  break;
			case 3: 
			  fprintf(stdout, "%d '%s'\n", key, value.c_str());
			  break;
		}
		
		continue_while:
		cout << "";
	}
  }
  
  end_select_early:
  
  if (attr == 4) {
    fprintf(stdout, "%d\n", count);
  }
  rc = 0;
  exit_select:
  
  if(reqIndex)
	tree.close();
	
  rf.close();
  return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
  RecordFile rf; 
  RecordId   rid;
  RC     rc;
  BTreeIndex tree;
  string line;
  int    key;
  string value;
  ifstream tableData(loadfile.c_str());

  if(!tableData.is_open())
	fprintf(stderr, "Error: loadfile %s cannot be opened\n", loadfile.c_str());
  rc = rf.open(table + ".tbl", 'w');
 
  if(index)
  {
	tree.open(table + ".idx", 'w');

	  while(getline(tableData, line))
	  {
		parseLoadLine(line, key, value);
		if(rf.append(key, value, rid)!=0)
			return RC_FILE_WRITE_FAILED;
		if(tree.insert(key, rid)!=0)
			return RC_FILE_WRITE_FAILED;
	  }
	tree.close();
  }
  else
  {
	  while(getline(tableData, line))
	  {
		parseLoadLine(line, key, value);
		rc = rf.append(key, value, rid);	
	  }
  }
  rf.close();
  tableData.close();
  
  return rc;
}

RC SqlEngine::parseLoadLine(const string& line, int& key, string& value)
{
    const char *s;
    char        c;
    string::size_type loc;
    
    // ignore beginning white spaces
    c = *(s = line.c_str());
    while (c == ' ' || c == '\t') { c = *++s; }

    // get the integer key value
    key = atoi(s);

    // look for comma
    s = strchr(s, ',');
    if (s == NULL) { return RC_INVALID_FILE_FORMAT; }

    // ignore white spaces
    do { c = *++s; } while (c == ' ' || c == '\t');
    
    // if there is nothing left, set the value to empty string
    if (c == 0) { 
        value.erase();
        return 0;
    }

    // is the value field delimited by ' or "?
    if (c == '\'' || c == '"') {
        s++;
    } else {
        c = '\n';
    }

    // get the value string
    value.assign(s);
    loc = value.find(c, 0);
    if (loc != string::npos) { value.erase(loc); }

    return 0;
}
