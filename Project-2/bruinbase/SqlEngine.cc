/**
 * Copyright (C) 2008 by The Regents of the University of California
 * Redistribution of this file is permitted under the terms of the GNU
 * Public License (GPL).
 *
 * @author Junghoo "John" Cho <cho AT cs.ucla.edu>
 * @date 3/24/2008
 */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include "Bruinbase.h"
#include "SqlEngine.h"
#include "BTreeIndex.h"
#include <string>

using namespace std;

// external functions and variables for load file and sql command parsing 
extern FILE* sqlin;
int sqlparse(void);

int EQ = SelCond::EQ;
int NE = SelCond::NE;
int LT = SelCond::LT;
int GT = SelCond::GT;
int LE = SelCond::LE;
int GE = SelCond::GE;

RC SqlEngine::run(FILE* commandline)
{
  fprintf(stdout, "Bruinbase> ");

  // set the command line input and start parsing user input
  sqlin = commandline;
  sqlparse();  // sqlparse() is defined in SqlParser.tab.c generated from
               // SqlParser.y by bison (bison is GNU equivalent of yacc)

  return 0;
}

RC SqlEngine::select(int attr, const string& table, const vector<SelCond>& cond)
{
  RecordFile rf;   // RecordFile containing the table
  RecordId   rid;  // record cursor for table scanning
  IndexCursor cursor;
  BTreeIndex btree;

  RC     rc;
  int    key;     
  string value;
  int    count;
  int    diff;
  int locate_key = 0;

  // open the table file
  if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
    fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
    return rc;
  }

  // scan the table file from the beginning
  rid.pid = rid.sid = 0;
  count = 0;
  //while (rid < rf.endRid()) {
    // read the tuple
    //if ((rc = rf.read(rid, key, value)) < 0) {
	/*
	if ((rc = rf.open(table + ".tbl", 'r')) < 0) {
      fprintf(stderr, "Error in select: while reading a tuple from table %s\n. Table does not exist.\n", table.c_str());
      goto exit_select;
	  //return rc;
    }
*/
	
    // check the conditions on the tuple
	
	if(btree.open(table+".idx", 'r') == 0)
	{
		for (unsigned i = 0; i < cond.size(); i++) {
			if(cond[i].attr != 1)
				continue;
			else if (cond[i].comp == EQ)
			{
				locate_key = atoi(cond[i].value);
				break;
			}
			else if (cond[i].comp == GE)
			{
				int temp = atoi(cond[i].value);
				if(locate_key == 0)
				{
					locate_key = temp;
					continue;
				}
				if(temp > locate_key)
				{
					locate_key = temp;
				}
			}
			else if (cond[i].comp == GT)
			{
				int temp = atoi(cond[i].value);
				if(locate_key == 0)
				{
					locate_key = temp;
					continue;
				}
				temp += 1;
				if(temp > locate_key)
				{
					locate_key = temp;
				}
			}
		}	//end of for loop
		btree.locate(locate_key, cursor);
	
		while(btree.readForward(cursor, key, rid) == 0)
		{
			if(attr != 4)
			{
				if ((rc = rf.read(rid, key, value)) < 0) {
					fprintf(stderr, "Error while readForward: while reading a tuple from table %s\n. Table does not exist.\n", table.c_str());
					goto exit_select;
				}
			}
			
			for (unsigned i = 0; i < cond.size(); i++) {
				// compute the difference between the tuple value and the condition value
				switch (cond[i].attr) {
					case 1:
						diff = key - atoi(cond[i].value);
					break;
					case 2:
						diff = strcmp(value.c_str(), cond[i].value);
					break;
				}

				// skip the tuple if any condition is not met
				switch (cond[i].comp) {
					case SelCond::EQ:
						if (diff != 0) 
						{
						//goto next_tuple;
							if(cond[i].attr == 1)
							{
								goto exit_select;
							}
							else
								continue;
						}
					break;
					case SelCond::NE:
						if (diff == 0) //goto next_tuple;
							continue;
					break;
					case SelCond::GT:
						if (diff <= 0) //goto next_tuple;
							continue;
					break;
					case SelCond::LT:
						if (diff >= 0) //goto next_tuple;
						{
							if(cond[i].attr == 1)
							{
								goto exit_select;
							}
							else
								continue;
						}
					break;
					case SelCond::GE:
						if (diff < 0) //goto next_tuple;
							continue;
					break;
					case SelCond::LE:
					if (diff > 0) //goto next_tuple;
					{
						if(cond[i].attr == 1)
							{
								goto exit_select;
							}
							else
								continue;
					}
					break;
				}
			}

			// the condition is met for the tuple. 
			// increase matching tuple counter
			count++;

			// print the tuple 
			switch (attr) {
				case 1:  // SELECT key
					fprintf(stdout, "%d\n", key);
				break;
				case 2:  // SELECT value
					fprintf(stdout, "%s\n", value.c_str());
				break;
				case 3:  // SELECT *
					fprintf(stdout, "%d '%s'\n", key, value.c_str());
				break;
			}

			// move to the next tuple
			//next_tuple:
			//++rid;
		}//end of while loop
			
		}//end of if statement
		
		else	//no index available
		{
			while (rid < rf.endRid()) {
				// read the tuple
				if ((rc = rf.read(rid, key, value)) < 0) {
					fprintf(stderr, "Error while reading with no index available: while reading a tuple from table %s\n", table.c_str());
					goto exit_select;
				}

				// check the conditions on the tuple
				for (unsigned i = 0; i < cond.size(); i++) {
					// compute the difference between the tuple value and the condition value
					switch (cond[i].attr) {
						case 1:
							diff = key - atoi(cond[i].value);
						break;
						case 2:
							diff = strcmp(value.c_str(), cond[i].value);
						break;
					}

					// skip the tuple if any condition is not met
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

				// the condition is met for the tuple. 
				// increase matching tuple counter
				count++;

				// print the tuple 
				switch (attr) {
					case 1:  // SELECT key
						fprintf(stdout, "%d\n", key);
					break;
					case 2:  // SELECT value
						fprintf(stdout, "%s\n", value.c_str());
					break;
					case 3:  // SELECT *
						fprintf(stdout, "%d '%s'\n", key, value.c_str());
					break;
				}

				// move to the next tuple
				next_tuple:
				++rid;
			}
		}	//end of else statement
  // print matching tuple count if "select count(*)"
  if (attr == 4) {
    fprintf(stdout, "%d\n", count);
  }
  rc = 0;
/////////////////////////////////////////////////////////
  // close the table file and return
  exit_select:
  rf.close();
  return rc;
}

RC SqlEngine::load(const string& table, const string& loadfile, bool index)
{
	RecordFile rf;
	RC     rc;
	RecordId   rid;
	std::ifstream ifs;
	string line;
	fstream import_file;
	
	import_file.open(loadfile.c_str(), fstream::in);
	
	if(!import_file || !import_file.is_open())
	{
		fprintf(stderr, "Error: failed to open/read file: %s\n", loadfile.c_str());
		return RC_FILE_READ_FAILED;
	}

	if ((rc = rf.open((table + ".tbl").c_str(), 'w')) < 0) {
		fprintf(stderr, "Error: table %s does not exist\n", table.c_str());
		return rc;
	}
	
	BTreeIndex btree;

	int    key;     
	string value;
	if(index)
	{
		if(btree.open(table + ".idx", 'w') < 0)
			return RC_FILE_READ_FAILED;
	}
	
	ifs.open(loadfile.c_str(), std::ifstream::in);

	while (getline(import_file, line))
	{
		if ((rc = parseLoadLine(line, key, value)) < 0) {
			fprintf(stderr, "Error: cannot parse loadfile.\n");
			return rc;
		}
		if ((rf.append(key, value, rid)) < 0) {
			fprintf(stderr, "Error: cannot append the file's data to table.\n");
			return rc;
		}
		else if (index)
			btree.insert(key, rid);
	}
	if(index)
		btree.close();
	import_file.close();
	return 0;
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
