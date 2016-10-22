#include <iostream>
#include <fstream>
#include <limits>
#include <ios>
#include "FileService.h"

using namespace std;

streamsize filesize(const char* filename)
{
	ifstream file;
	file.open(filename, ios::in | ios::binary);
	file.ignore(numeric_limits<streamsize>::max());
	streamsize length = file.gcount();
	file.clear();
	file.seekg(0, ios_base::beg);
	return length;
}

char* ReadFile(const char * filename, int start, int end)
{
	char * buffer = new char[BUFFER_SIZE];
	long diff = end - start;
	if (diff > BUFFER_SIZE)
	{
		cerr << "Buffer isn't large enough to make read from bytes " << start << " to " << end << endl;
		return NULL;
	}
	ifstream inf(filename);
	if (!inf)
	{
		cerr << "Could not open " << filename << endl;
		return NULL;
	}

	inf.seekg(start, ios::beg);
	inf.read(buffer, diff);
	return buffer;
}

char* WriteFile(const char * filename, int start, int end, char buffer[])
{
	long diff = end - start;
	if (diff > BUFFER_SIZE)
	{
		cerr << "Buffer isn't large enough to make read from bytes " << start << " to " << end << endl;
		return NULL;
	}
	ofstream inf(filename);
	if (!inf)
	{
		cerr << "Could not open " << filename << endl;
		return NULL;
	}

	inf.seekp(start, ios::beg);
	inf.write(buffer, diff);
	return buffer;
}