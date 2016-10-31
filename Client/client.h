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

#define WINDOW_SIZE 25

using namespace std;

struct WindowSectionWrapper
{
	int PacketNum;
	char Payload[PAYLOAD_SIZE];
	int LoadFull;		//boolean: if buffer has useful information
};

int TotalPackets;
int CurrentWindowBase;
WindowSectionWrapper *WindowManager;
int FileSize;
char *FileName;


struct sockaddr_in ClientAddr, ServerAddr;
int FileDescriptor, PortNum;
socklen_t ServerSize = sizeof(ServerAddr);
int RecvLength;		/* # bytes in acknowledgement message */
char *Server;	/* change this to use a different server */

ClientPacket *SendBuffer;
ServerPacket *RecvBuffer;

void InitRequest();
void Request(int packetNumFirst, int packetNumLast);
void Receive();
void DumpWindow();
void InitSocket();

#endif