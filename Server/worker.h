#ifndef Worker_H
#define Worker_H

#include <pthread.h>

struct PacketRequest{
	int PacketNum;
	sockaddr_in RequestAddress;
	
	PacketRequest();
	PacketRequest(int packetNum, sockaddr_in requestAddress);
	PacketRequest(const PacketRequest& other);
	PacketRequest& operator=(const PacketRequest& other);
};

bool InitThreads(int numThreads, const char * filename, int fd);

void QueueRequest(PacketRequest request);
bool DequeueRequest(PacketRequest & request);

void * WaitForRequests (void * arg);
void SendPacket (const ServerPacket * packet, sockaddr_in & addr);

#endif

