// SymTable.cpp

#include "SymTable.h"

int SymTableEntry::locationCounter = 0;




SymTableEntry::SymTableEntry(string name, int section, unsigned int value, bool def, int rwe, char vis,  int sizeOfSection, int address )
{
	this->name = name;
	this->section = section;
	this->value = value;
	this->visibility = vis;
	this->count = SymTableEntry::locationCounter++;
	this->defined = def;
	this->sizeOfSection = sizeOfSection;
	this->addressInMemory = address;
	this->rwe = rwe;
}

SymTableEntry::~SymTableEntry()
{
	for (list<ForwardRefs*>::iterator it = fLink.begin(); it != fLink.end(); ++it)
		delete *it;
	fLink.clear();
}

string SymTableEntry::getName() const
{
	return name;
}

int SymTableEntry::getCount() const
{
	return count;
}

void SymTableEntry::setValue(int val)
{
	value = val;
}

int SymTableEntry::getValue()
{
	return value;
}

void SymTableEntry::setVisibility(char v)
{
	visibility = v;
}

bool SymTableEntry::isGlobal()
{
	return (visibility == 'g' ? true : false);
		
}

void SymTableEntry::setDefined(bool d)
{
	defined = d;
}

void SymTableEntry::setSizeOfSection(int s)
{
	sizeOfSection = s;
}

void SymTableEntry::setAddressInMemory(int a)
{
	addressInMemory = a;
}

void SymTableEntry::setRWE(int r)
{
	rwe = r;
}

void SymTableEntry::addFLink(int p, int s, int t)
{
	ForwardRefs* fr = new ForwardRefs(p, s, t);
	fLink.push_back(fr);
}

bool SymTableEntry::isDefined()
{
	return (defined ? true : false);
}

int SymTableEntry::getSectionNum()
{
	return section;
}

void SymTableEntry::setSectionNum(int num)
{
	section = num;
}

void SymTableEntry::printIntoFile(ofstream & outFile)
{
	outFile << setw(15) << left << name << setw(10) << left << section << setw(10) << left << value << setw(10) << left << visibility << setw(10) << left << count;
	if (count == section)
	{
		outFile << setw(10) << left << sizeOfSection << setw(10) << left << addressInMemory;
		outFile <<  ((rwe & 4)==4 ?  'r' : '-');
		outFile << ((rwe & 2)==2 ? 'w' : '-');
		outFile << ((rwe & 1)==1 ? 'e' : '-');
		
	}
		
	outFile << '\n';
}
