CC=g++ -std=gnu++0x
CFLAGS = -g 
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: client testserver

client: client.o 
	$(CC) -o client client.o $(LIBS)

testserver: testserver.o 
	$(CC) -o testserver testserver.o $(LIBS)

clean:
	rm -f client testserver client.o testserver.o 
