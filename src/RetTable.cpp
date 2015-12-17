// RetTable.cpp

#include "RetTable.h"

using namespace std;


RetTable::RetTable()
{
}


void RetTable::insertValue(int offset, int typeOfRelocation, int value)
{
	retTableEntry* r = new retTableEntry(offset, typeOfRelocation, value);
	table.push_back(r);
}

void RetTable::printIntoFile(ofstream & outFile)
{
	outFile << setw(15) << left << "#ofset" << setw(16) << left << "tip" << setw(10) << left << "vr." << '\n';
	for (list<retTableEntry*>::iterator it = table.begin();it != table.end(); ++it)
	{
		outFile << setw(15) << left << (*it)->offset << setw(16) << left << typeOfRel[(*it)->typeOfRelocation ] << setw(10) << left << (*it)->value << '\n';
	}
}

RetTable::~RetTable()
{
	for (list<retTableEntry*>::iterator it = table.begin(); it != table.end(); ++it)
		delete *it;
	table.clear();
}
