#ifndef FileService_h
#define FileService_h
#define	BUFFER_SIZE = 1024

streamsize filesize(const char* filename);
unsigned char[] ReadFile(const char * filename, int start, int stop);
unsigned char[] WriteFile(const char * filename, int start, int stop, unsigned char buffer[]);

#endif // !FileService_h
