CC=g++ -std=gnu++0x
CFLAGS = -g 
SHARED=../Shared
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: client

client: client.o FileService.o
	$(CC) -o client client.o FileService.o $(LIBS)

FileService.o: $(SHARED)/FileService.cpp $(SHARED)/FileService.h
	$(CC) -c $(SHARED)/FileService.cpp 

clean:
	rm -f client *.o
