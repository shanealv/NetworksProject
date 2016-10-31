CC=g++ -std=gnu++0x
#CFLAGS = -g -DDEBUG
SHARED = ../Shared/
# uncomment this for SunOS
# LIBS = -lsocket -lnsl
LIBS = -pthread

all: server.o worker.o FileService.o
	$(CC) $(LIBS) $(CFLAGS) server.o worker.o FileService.o -o server 

server.o: server.cpp 
	$(CC) $(LIBS) $(CFLAGS) -c server.cpp
	
worker.o: worker.cpp
	$(CC) $(CFLAGS) -c worker.cpp

FileService.o: $(SHARED)FileService.cpp
	$(CC) $(CFLAGS) -c $(SHARED)FileService.cpp

clean:
	rm -f server *.o 
	
rebuild: clean all

debug: rebuild
	./server
