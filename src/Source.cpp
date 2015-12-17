// source.cpp

#include "Line.h"
#include "FileManager.h"
#include <fstream>

using namespace std;

int main(int argc, char* argv[])
{
	

	FileManager *fm = new FileManager();
	try 
	{
		fm->openFile(argv[1]);
		fm->backPatch();
		fm->printFile(argv[2]);
		delete fm;
		cout << "fajl uspesno preveden!\n";
	}
	catch (Exception e)
	{
		cout << e;
	}
	return 0;
}