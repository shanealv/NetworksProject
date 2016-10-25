CC=g++ -std=gnu++0x
CFLAGS = -g 
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: client server

client: client.o 
	$(CC) -o client client.o $(LIBS)

server: server.o 
	$(CC) -o server server.o $(LIBS)

clean:
	rm -f client server client.o server.o 
