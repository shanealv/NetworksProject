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

using namespace std;

struct WindowSectionWrapper
{
	int PacketNum;
	char Payload[PAYLOAD_SIZE];
	int LoadFull;	//boolean
};

int TotalPackets;
int WindowSize;
int CurrentWindowBase;
WindowSectionWrapper *WindowManager;


struct sockaddr_in myaddr, remaddr;
int fd, portno;
socklen_t slen = sizeof(remaddr);
int recvlen;		/* # bytes in acknowledgement message */
char *server;	/* change this to use a different server */

ClientPacket *sendBuff;
ServerPacket *recvBuff;

int InitRequest();
void Request(int packetNumFirst, int packetNumLast);
void Receive();
void WriteToFile();
void InitFile(int size);
void InitSocket();

#endif