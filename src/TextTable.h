// TextTable.h
#ifndef _texttable_h_
#define _texttable_h_

#include <map>
#include <string>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include "RetTable.h"

using namespace std;

typedef map<string, int> MAP;


static MAP instructionMap = { {"add", 0}, {"sub", 1}, {"mul", 2}, {"div", 3},{ "cmp", 4 },{ "and", 5 },{ "or", 6 },{ "not", 7 },{ "test", 8 },{ "ldr", 9 },{ "str", 10 },
										   { "je", 11 },{ "jne", 12 },{ "jge", 13 },{ "jg", 14 },{ "jle", 15 },{ "jl", 16 },{ "jo", 19 },{ "jno", 20 },{ "call", 21 },{ "ret", 22 },
										   { "iret", 22 } ,{ "jmp", 22 } ,{ "push", 23 } ,{ "pop", 24 } ,{ "mov", 25 } ,{ "in", 26 } ,{ "out", 27 } ,{ "shr", 28 } ,{ "shl", 29 },
										   { "int", 30 },{ "ldc", 31 } ,{ "ldch", 31 } ,{ "ldcl", 31 } };
static MAP registerMap = { {"r0", 0}, { "PC", 0 }, { "r1", 1 },{ "SP", 1 } ,{ "r2", 2 },{ "r3", 3 },{ "r4", 4 },{ "r5", 5 },{ "r6", 6 },{ "r7", 7 },{ "r8", 8 },{ "r9", 9 },
									    {"r10", 10},{ "r11", 11 },{ "r12", 12 },{ "r13", 13 },{ "r14", 14 },{ "r15", 15 },{ "psw", 16 },{ "pmt", 17 } };

static MAP directivesMap = { {".char", 0},{ ".wordh", 1 }, { ".long", 2 },{ ".align", 3 },{ ".skip", 4 },{ ".global", 5 },{ ".extern", 6 }, 
										 {".end", 10}, {".wordl", 11 } };

//{".text", 7}, {".data", 8}, {".bss", 9},

struct TableEntry
{
	int value;
	int numOfBytes;

	TableEntry(int val, int num)
	{
		value = val;
		numOfBytes = num;
	}
};

//template< typename T >
//string intToHex(T i, int numOfBytes)
//{
//	stringstream stream;
//	stream << setfill('0') << std::setw(numOfBytes * 2) << hex << i;
//	return stream.str();
//}

template< typename T >
string intToHex(T i, int numOfBytes)
{
	unsigned char bytes[4];
	//int n = 4;

	bytes[0] = (i >> 24) & 0xFF;
	bytes[1] = (i >> 16) & 0xFF;
	bytes[2] = (i>> 8) & 0xFF;
	bytes[3] = i & 0xFF;



	stringstream s;
	s << std::hex << std::setfill('0') << std::uppercase
		<< std::setw(2) << static_cast<int>(bytes[3]);
	if(numOfBytes<=4)
		for (int n = numOfBytes - 1; n > 0; n--)
			s << ' ' << std::setw(2) << static_cast<int>(bytes[n - 1]);
	else
		for (int n = numOfBytes - 1; n > 0; n--)
			s << ' ' << std::setw(2) << static_cast<int>(bytes[3]);

	//std::cout << "[" << s.str() << "]\n";

	return s.str();
}


class TextTable
{
private:
	string name;
	map<int, TableEntry*> table;
	RetTable* retTable;
	int countNumber;
	int locationCounter;
	//int addressInMemory; //ne treba tu, jer se to nalazi u tabeli simbola
	

	friend class FileManager;

public:
	TextTable(string name);
	~TextTable();

	int insertValue(int lc, int val, int numOfBytes=4);

	void printIntoFile(ofstream& outFile);
	//void setAddress(int adr);
	//int setInstruction(int count, string mnemonic);

};

#endif