CC=g++ -std=gnu++0x
CFLAGS = -g 
SHARED = ../Shared/
# uncomment this for SunOS
# LIBS = -lsocket -lnsl
LIBS = -pthread

all: server.o worker.o FileService.o
	$(CC) $(LIBS) server.o worker.o FileService.o -o server 

server.o: server.cpp 
	$(CC) $(LIBS) -c server.cpp
	
worker.o: worker.cpp
	$(CC) -c worker.cpp

FileService.o: $(SHARED)FileService.cpp
	$(CC) -c $(SHARED)FileService.cpp

clean:
	rm -f server *.o 
