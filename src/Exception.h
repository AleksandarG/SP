// Exception.h
#ifndef _exception_h_
#define _exception_h_

#pragma once
#include <string>
#include <iostream>
#include "Line.h"

using namespace std;

class Exception
{
private:
	string message;
	Line* line = 0;
	int lineNumber = 0;
public:
	Exception(string s, int lineNum = 0, Line* l=0);

	~Exception();
	friend ostream& operator<<(ostream& it, const Exception& e);
};

#endif