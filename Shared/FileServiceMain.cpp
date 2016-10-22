#include "fileService.h"
#include <iostream>
#include <fstream>
#include <ios>
#include <string>

using namespace std;

int main(int argc, char * argv[])
{
	char infile[] = "lorem.txt";
	char outfile[] = "out.txt";
	long fileSize = (long) filesize(infile);
	long numChunks = GetNumChunks(fileSize);
	cout << fileSize << endl;
	cout << numChunks << endl;
	AllocateFile(outfile, fileSize);
	for (int i = 0; i < numChunks; i++)
	{
		char * buffer = CopyChunk(infile, i, fileSize);
		SaveChunk(outfile, i, fileSize, buffer);
	}
	long outSize = (long)filesize(outfile);
	cout << outSize << endl;
}