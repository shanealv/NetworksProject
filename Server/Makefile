CC=g++ -std=gnu++0x
CFLAGS = -g 
# uncomment this for SunOS
# LIBS = -lsocket -lnsl

all: server

server: server.o 
	$(CC) -o server server.o $(LIBS)

clean:
	rm -f server server.o 
