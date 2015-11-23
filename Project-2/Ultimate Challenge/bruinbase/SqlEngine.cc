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
  int    count = 0;
  int    diff;

  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
	fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
	return rc;
  }
  
	SelCond temp;
	bool hasCond = false;
	bool hasValueCond = false;
	bool usesIndex = false;
	
	bool condGE = false;
	bool condLE = false;
	int max = -1;
	int min = -1;
	int eqVal = -1;
	
	int condIndex = -1;
	
	bool valueConflict = false;
	std::string valEq = "";

	for(int i=0; i<cond.size(); i++)
	{
		temp = cond[i];
		int tempVal = atoi(temp.value);

		if(temp.attr==1 && temp.comp!=SelCond::NE)
		{
			hasCond = true;
			
			if(temp.comp==SelCond::EQ)
			{
				eqVal = tempVal;
				condIndex = i;
				break;
			}
			else if(temp.comp==SelCond::GE)
			{
				if(tempVal > min || min==-1)
				{
					condGE = true;
					min = tempVal;
				}
			}
			else if(temp.comp==SelCond::GT)
			{
				if(tempVal >= min || min==-1)
				{
					condGE = false;
					min = tempVal;
				}
			}
			else if(temp.comp==SelCond::LE)
			{
				if(tempVal < max || max==-1)
				{
					condLE = true;
					max = tempVal;
				}
			}
			else if(temp.comp==SelCond::LT)
			{
				if(tempVal <= max || max==-1)
				{
					condLE = false;
					max = tempVal;
				}
			}
		}
		else if(temp.attr==2)
		{
			hasValueCond = true;
			
			if(temp.comp==SelCond::EQ)
			{
				if(valEq=="" || strcmp(value.c_str(), cond[i].value)==0)
					valEq=tempVal;
				else
					valueConflict = true;
			}
		}
	}
	
	if(valueConflict || (max!=-1 && min!=-1 && max<min))
		goto end_select_early;

	if(max!=-1 && min!=-1 && !condGE && !condLE && max==min)
		goto end_select_early;

  if(tree.open(table + ".idx", 'r')!=0 || (!hasCond && attr!=4))
  {
	  rid.pid = rid.sid = 0;
	  count = 0;
	  while (rid < rf.endRid()) {
		if ((rc = rf.read(rid, key, value)) < 0) {
		  fprintf(stderr, "Error: while reading a tuple from table %s\n", table.c_str());
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
	usesIndex = true;
	
	if(eqVal!=-1) 
		tree.locate(eqVal, c);
	else if(min!=-1 && !condGE)
		tree.locate(min+1, c);
	else if(min!=-1 && condGE)
		tree.locate(min, c);
	else
		tree.locate(0, c);
	
	while(tree.readForward(c, key, rid)==0)
	{
		if(!hasValueCond && attr==4)
		{
			if(eqVal!=-1 && key!=eqVal)
				goto end_select_early;
			
			if(max!=-1)
			{
				if(condLE && key>max)
					goto end_select_early;
				else if(!condLE && key>=max)
					goto end_select_early;
			}
			
			if(min!=-1)
			{
				if(condGE && key<min)
					goto end_select_early;
				else if(!condGE && key<=min)
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
  
  if(usesIndex)
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
