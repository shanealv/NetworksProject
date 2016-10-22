#include "fileService.h"
#include <iostream>
#include <fstream>
#include <ios>

using namespace std;

int main(int argc, char * argv[])
{
	char file[]= "lorem.txt";
	streamsize fileSize = filesize(file);
	cout << fileSize << endl;
}