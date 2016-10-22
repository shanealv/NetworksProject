CC=g++
CFLAGS=-std=gnu++0x

fileservicetest: FileServiceMain.o FileService.o 
	$(CC) $(CFLAGS) -o fstest FileServiceMain.o FileService.o 

FileServiceMain.o: FileServiceMain.cpp FileService.h
	$(CC) $(CFLAGS) -c FileServiceMain.cpp

FileService.o: FileService.cpp FileService.h
	$(CC) $(CFLAGS) -c FileService.cpp

