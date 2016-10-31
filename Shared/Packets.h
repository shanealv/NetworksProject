#ifndef Packets_H
#define Packets_H

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>

#define PAYLOAD_SIZE 1024

struct ClientPacket
{
	int PacketNum;
};

struct ServerPacket
{
	char Payload[PAYLOAD_SIZE];
	int PacketNum;
};

#endif
