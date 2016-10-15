#ifndef PACKETS
#define PACKETS

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

struct ClientPacket
{
	struct sockaddr *ServerAddr;
	struct sockaddr *ReturnAddr;
	int PacketNum;
};

struct ServerPacket
{
	struct sockaddr *ReturnAddr;
	const  unsigned char Payload[1024];
	int PacketNum;
};

#endif