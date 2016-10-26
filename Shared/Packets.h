#ifndef PACKETS
#define PACKETS

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

struct ClientPacket
{
//	char *ServerAddr;
//	char *ReturnAddr;
	int PacketNum;
};

struct ServerPacket
{
	//char *ReturnAddr;
	//char Payload[1024];
	int PacketNum;
};

#endif

//Printing readable IP from struct sockaddr_in
//	char *ip = inet_ntoa(myaddr.sin_addr);
//	cout << ip << endl;