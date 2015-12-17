// SymTable.h
#ifndef _symtable_h_
#define _symtable_h_

#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct ForwardRefs	
{
	int patch;
	int section;
	int typeOfRelocation;
	//ForwardRefs* next;

	ForwardRefs(int p, int s, int t)
	{
		patch = p;
		section = s;
		typeOfRelocation = t;
		//next = NULL;
	}
};

class SymTableEntry
{
private:
	

	string name;
	int section;
	unsigned int value;
	char visibility;
	unsigned int count;
	bool defined;
	list<ForwardRefs*> fLink;
	//samo ako je u pitanju sekcija, inace 0
	int sizeOfSection; 
	int addressInMemory;
	int rwe;
	//ForwardRefs* fLink;
	friend class FileManager;
public:
	SymTableEntry(string name, int section, unsigned int value, bool def = false, int rwe = 0, char vis='l', int sizeOfSection=0, int address=0);
	~SymTableEntry();

	string getName() const;
	int getCount() const;
	void setValue(int val);
	int getValue();
	void setVisibility(char v);
	bool isGlobal();
	void setDefined(bool d);
	void setSizeOfSection(int s);
	void setAddressInMemory(int a);
	void setRWE(int r);
	void addFLink(int p, int s, int t);
	bool isDefined();
	int getSectionNum();
	void setSectionNum(int num);



	void printIntoFile(ofstream& outFile);
	static int locationCounter;
};

#endif