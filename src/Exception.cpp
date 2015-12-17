// Exception.cpp
#include "Exception.h"





Exception::Exception(string s, int lineNum, Line* l)
{
	message.assign(s);
	lineNumber = lineNum;
	line = l;
}

ostream& operator<<(ostream& it, const Exception& e)
{
	it << e.message;
	if (e.lineNumber != 0)
		it << endl <<"####   LINE "<< e.lineNumber << ": " << *e.line << "     ####"<< endl;
	return it;
}
Exception::~Exception()
{
}
