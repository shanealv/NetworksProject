#ifndef FileService_h
#define FileService_h
#define	BUFFER_SIZE 1024
#include <ios>

std::streamsize filesize(const char* filename);
void AllocateFile(const char* filename, long size);
long GetNumChunks(long fileSize);

char* ReadFile(const char * filename, int start, int stop);
void WriteFile(const char * filename, int start, int stop, char buffer[]);

char* CopyChunk(const char * filename, int chunkNumber, long fileSize);
void SaveChunk(const char * filename, int chunkNumber, long fileSize, char buffer[]);

#endif // !FileService_h
