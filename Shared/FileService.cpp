#include <iostream>
#include <fstream>
#include <limits>
#include <ios>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <fcntl.h>
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
	return numChunks - 1;
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

bool CopyChunk(int fd, int chunkNumber, long fileSize, char buffer[])
{
	
	int offset = (long) chunkNumber * BUFFER_SIZE;
	if (offset > fileSize) 
		return false;
	int size = (offset + size >= fileSize) ? fileSize - offset: BUFFER_SIZE;

#ifdef DEBUG
	cout << "[Reading] address " << offset << " to " << offset + size << endl;
#endif

	pread(fd, buffer, size, offset);
	return true;
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
