// Line.h
#ifndef _line_h_
#define _line_h_


#include <string>
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

class Line
{
private:
	string label;
	string mnemonic;
	vector<string> operands;
	//friend class FileManager;

public:
	Line();
	friend istream& operator>>(istream& istr, Line& l);
	friend ostream& operator<<(ostream& ostr, Line& l);
	int isEnd() const;
	bool existLabel() const;
	bool existMnemonic() const;
	bool existOperands() const;

	string& getLabel();
	string& getMnemonic();
	vector<string>& getOperands();

	bool isDirective() const;
	~Line();
};

#endif