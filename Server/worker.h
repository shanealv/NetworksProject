#ifndef Worker_H
#define Worker_H

#include <pthread.h>

typedef struct {
	int PacketNum;
	struct sockaddr_in requestAddress;
} PacketRequest;

bool InitThreads(int numThreads, int fd);

void QueueRequest(PacketRequest request);
bool DequeueRequest(PacketRequest & request);

void * WaitForRequests (void * arg);
void SendPacket (const struct ServerPacket & packet, struct sockaddr_in & addr);

#endif

