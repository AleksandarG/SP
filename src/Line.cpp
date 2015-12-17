// Line.cpp

#include "Line.h"
#include <algorithm>

using namespace std;


Line::Line()
{
}

istream& operator>>(istream& istr, Line& l)
{
	string line;

	if (getline(istr, line))
	{
		stringstream iss(line);
		string tmp;
		if(line.find(':')!=string::npos)
			if(getline(iss, tmp, ':'))
				l.label.assign(tmp);
		//if (tmp.length()!=0)
		//	l.label.assign(tmp);
			//else return str;

		iss >> ws;
		if (getline(iss, tmp, ' '))
		{
			size_t pos1 = tmp.find("#");
			
			if (pos1 != string::npos)
			{
				size_t pos2 = tmp.find(" ");
				size_t pos3 = tmp.find("\t");
				tmp = tmp.substr(0, min(pos1, min(pos2, pos3)));
			}
				
			l.mnemonic.assign(tmp);
		}
			
		//if (tmp.length() != 0)
			
		else 
			return istr;

		iss >> ws;
		

			while (getline(iss, tmp, ','))
			{
				size_t pos1 = tmp.find("#");
				
				if (pos1 != string::npos)
				{
					size_t pos2 = tmp.find(" ");
					size_t pos3 = tmp.find("\t");
					tmp = tmp.substr(0, min(pos1, min(pos2, pos3)));
				}
					
				
					
				l.operands.push_back(tmp);
				iss >> ws;
				//if(!getline(iss, tmp, ','))
				//	break;


			}
		
	}


	return istr;
}

ostream& operator<<(ostream& ostr, Line& l)
{
	if (l.label.compare(""))
		ostr << l.label;
	if (l.mnemonic.compare(""))
		ostr << "   " << l.mnemonic;
	for (vector<string>::iterator it = l.operands.begin(); it != l.operands.end(); )
	{
		ostr << " " << *it;
		it++;
		if (it != l.operands.end())
			ostr << ",";
	}
	return ostr;
}

int Line::isEnd() const
{
	return mnemonic.compare(".end");
}

bool Line::existLabel() const
{
	return label.size() > 0 ? true : false;
}

bool Line::existMnemonic() const
{
	return mnemonic.size() > 0 ? true : false;
}

bool Line::existOperands() const
{
	return operands.size()>0 ? true: false;
}

string & Line::getLabel()
{
	return label;
}

string & Line::getMnemonic()
{
	return mnemonic;
}

vector<string>& Line::getOperands()
{
	return operands;
}

bool Line::isDirective() const
{
	return (mnemonic.find(".") != string::npos) ? true : false;
}

Line::~Line()
{
}
