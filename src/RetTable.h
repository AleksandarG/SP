// RetTable.h
#ifndef _rettable_h_
#define _rettable_h_


#include <list>
//#include "TextTable.h"
#include <map>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


struct retTableEntry
{
	int offset;
	int typeOfRelocation;
	int value;

	retTableEntry(int offset, int typeOfRelocation, int value)
	{
		this->offset = offset;
		this->typeOfRelocation = typeOfRelocation;
		this->value = value;
	}
};

const string typeOfRel[] = {"00", "R_ABS_32", "R_PC_21", "R_ABS_16H", "R_ABS_16L", "R_PC_24", "R_ABS_16H_5", "R_ABS_16L_5"};

class RetTable
{
	list<retTableEntry*> table;

public:
	RetTable();

	void insertValue(int offset, int typeOfRelocation, int value);

	void printIntoFile(ofstream& outFile);
	~RetTable();
};

#endif