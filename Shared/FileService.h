#ifndef FileService_h
#define FileService_h
#define	BUFFER_SIZE 1024
#include <ios>

std::streamsize filesize(const char* filename);
char* ReadFile(const char * filename, int start, int stop);
char* WriteFile(const char * filename, int start, int stop, char buffer[]);

#endif // !FileService_h
