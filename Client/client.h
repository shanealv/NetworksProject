#ifndef CLIENT
#define CLIENT

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>
#include "../Shared/Packets.h"
#include "../Shared/FileService.h"

#define BUFLEN 2048

using namespace std;

int TotalPackets;
int WindowSize;
struct sockaddr_in myaddr, remaddr;
int fd, portno;
socklen_t slen = sizeof(remaddr);
char buf[BUFLEN];	/* message buffer */
int recvlen;		/* # bytes in acknowledgement message */
char *server;	/* change this to use a different server */

ClientPacket *sendBuff;
ServerPacket *recvBuff;

int InitRequest();
void Request(int packetNumFirst, int packetNumLast);
void Receive();
void WriteToFile();
void InitFile(int size);

#endif

//int socket(int domain, int type, int protocol);
//int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
//ssize_t read(int fd, void *buf, size_t count);
//ssize_t write(int fd, const void *buf, size_t count);
