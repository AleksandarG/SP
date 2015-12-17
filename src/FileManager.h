// FileManager.h
#ifndef _filemanager_h_
#define _filemanager_h_


#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <vector>
#include <list>
#include "SymTable.h"
#include "TextTable.h"
#include "Line.h"
#include "Exception.h"

using namespace std;






class FileManager
{
	list<SymTableEntry*> symTable;
	list<TextTable*> textTables;
	

	int currentSection;
	TextTable* curSection;
	int currentLocationCounter;
	
	bool newSection;

	bool isNumeric(const string& in);

public:
	FileManager();
	
	void openFile(char* fName);
	void printFile(string s);
	SymTableEntry* findSymTable( const string symbol);
	SymTableEntry* findSymTableByCount(int count);
	TextTable* findTextTable(int count);
	void backPatch();
	~FileManager();
};

#endif