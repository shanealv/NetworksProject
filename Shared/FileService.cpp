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
	file.close();
	return length;
}

void AllocateFile(const char* filename, long size)
{
	ofstream ofs(filename, ios::binary | ios::out);
	ofs.seekp(size - 1);
	ofs.write("", 1);
	ofs.close();
}

long GetNumChunks(long fileSize)
{
	int numChunks = fileSize / BUFFER_SIZE;
	if (fileSize % BUFFER_SIZE > 0)
	{
		numChunks++;
	}
	return numChunks;
}

void ReadFile(const char * filename, int start, int end, char buffer[])
{
	long diff = end - start;
	if (diff > BUFFER_SIZE)
	{
		cerr << "Buffer isn't large enough to make read from bytes " << start << " to " << end << endl;
		return;
	}
	ifstream inf(filename, ios::in | ios::binary);
	if (!inf)
	{
		cerr << "Could not open " << filename << endl;
		return;
	}

	inf.seekg(start, ios::beg);
	inf.read(buffer, diff);
	inf.close();
}

void WriteFile(const char * filename, int start, int end, char buffer[])
{
	long diff = end - start;
	if (diff > BUFFER_SIZE)
	{
		cerr << "Buffer isn't large enough to make read from bytes " << start << " to " << end << endl;
	}
	fstream outf(filename);
	if (!outf)
	{
		cerr << "Could not open " << filename << endl;
	}

	outf.seekp(start, ios::beg);
	outf.write(buffer, diff);
	outf.close();
}

void CopyChunk(const char * filename, int chunkNumber, long fileSize, char buffer[])
{
	int start = chunkNumber * BUFFER_SIZE;
	int end = start + BUFFER_SIZE;
	if (end > fileSize)
	{
		end = fileSize;
	}
	ReadFile(filename, start, end, buffer);
}

void SaveChunk(const char * filename, int chunkNumber, long fileSize, char buffer[])
{
	int start = chunkNumber * BUFFER_SIZE;
	int end = start + BUFFER_SIZE;
	if (end > fileSize)
	{
		end = fileSize;
	}
	WriteFile(filename, start, end, buffer);
}
