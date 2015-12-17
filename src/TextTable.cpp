// TextTable.cpp

#include "TextTable.h"

using namespace std;



TextTable::TextTable(string name)
{
	this->name.assign(name);
	//svaka text, il data sekcija imaju svoju tabelu relokacija
	retTable = new RetTable();

}


TextTable::~TextTable()
{
	delete retTable;
	for (map<int, TableEntry*>::iterator it = table.begin(); it != table.end(); ++it)
		delete it->second;
	table.clear();
}

int TextTable::insertValue(int lc, int val, int numOfBytes)
{
	TableEntry* newEntry = new TableEntry(val, numOfBytes);
	table.insert({ lc, newEntry });
	return 0;
}

void TextTable::printIntoFile(ofstream & outFile)
{
	//prvo ispise ret table
	outFile << "#ret" <<  name << '\n';
	retTable->printIntoFile(outFile);
	//pa onda text table
	outFile << "#" << setw(20) << left << name << '\n';
	int size = 0;
	for (map<int, TableEntry*>::iterator it = table.begin(); it != table.end(); ++it)
	{
		string s = intToHex(it->second->value, it->second->numOfBytes);
		//size += s.size();
		if ((size + s.size() )% 80 > 69)
		{
			outFile << '\n';
			size = 0;
		}
		size += s.size();
		outFile << s<<" ";
		
	}
	outFile << '\n';
}

//void TextTable::setAddress(int adr)
//{
//	addressInMemory = adr;
//}

//int TextTable::setInstruction(int count, string mnemonic)
//{
//	int tmp = instructionMap.find(mnemonic)->second;
//	tmp <<= 27;
//	map<int, TableEntry*>::iterator it = table.find(count);
//	it->second = tmp;
//	return 0;
//}
