#ifndef Worker_H
#define Worker_H

#include <pthread.h>

struct PacketRequest{
	int PacketNum;
	struct sockaddr_in RequestAddress;
	
	PacketRequest() {}
	
	PacketRequest(int packetNum, struct sockaddr_in requestAddress)
	{
		PacketNum = packetNum;
		memcpy(&RequestAddress, &requestAddress, sizeof(sockaddr_in)); ;
	}
	
	PacketRequest(const PacketRequest& other)
	{
		PacketNum = other.PacketNum;
		memcpy(&RequestAddress, &other.RequestAddress, sizeof(sockaddr_in)); ;
	}
	
	PacketRequest& operator=(const PacketRequest& other)
	{
		PacketNum = other.PacketNum;
		memcpy(&RequestAddress, &other.RequestAddress, sizeof(sockaddr_in)); ;
	}
};

bool InitThreads(int numThreads, const char * filename, int fd);

void QueueRequest(const PacketRequest & request);
bool DequeueRequest(PacketRequest & request);

void * WaitForRequests (void * arg);
void SendPacket (const struct ServerPacket * packet, struct sockaddr_in & addr);

#endif

