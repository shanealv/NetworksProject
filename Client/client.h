#ifndef CLIENT
#define CLIENT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int TotalPackets;
int WindowSize;
int SockFD, PortNum, N;
struct sockaddr_in Serv_Addr;
struct hostent *Server;
struct in_addr ip;

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
