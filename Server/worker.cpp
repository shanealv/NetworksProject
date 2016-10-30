#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <pthread.h>
#include <queue>
#include "../Shared/Packets.h"
#include "../Shared/FileService.h"
#include "worker.h"

using namespace std;

int socket_fd;
char * file_name;
long file_size;
long file_blocks;
pthread_t * workers;
pthread_mutex_t queue_lock;
pthread_mutex_t network_lock;

queue<PacketRequest> requestQueue;

bool InitThreads(int numThreads, char * filename, int fd)
{
	socket_fd = fd;
	if (numThreads < 1)
	{
		cerr << "Error: Invalid Number of Threads" << endl;
		return false;
	}
	
	workers = new pthread_t[numThreads];
	if (pthread_mutex_init(&queue_lock, NULL) != 0)
	{
		cerr << "Error: Mutex intialization failed" << endl;
		return false;
	}
	if (pthread_mutex_init(&network_lock, NULL) != 0)
	{
		cerr << "Error: Mutex intialization failed" << endl;
		return false;
	}
	
	file_name = filename;
	file_size = (long) filesize(file_name);
	file_blocks = GetNumChunks(file_size);
	
	for (int i = 0; i < numThreads; i++)
	{
		int returnCode = pthread_create(workers + (i-1), NULL, WaitForRequests, NULL);
		if (returnCode)
		{
			cerr << "Error: unable to create thread, return code: " << returnCode << endl;
			return false;
		}
	}
	return true;
}

void QueueRequest(PacketRequest & request)
{
	pthread_mutex_lock(&queue_lock);
	requestQueue.push(request);
	pthread_mutex_unlock(&queue_lock);
}

bool DequeueRequest(PacketRequest & request)
{
	pthread_mutex_lock(&queue_lock);
	if (requestQueue.empty())
		return false;

	request = requestQueue.front();
	requestQueue.pop();
	pthread_mutex_unlock(&queue_lock);
	return true;
}


void * WaitForRequests (void * arg)
{
	PacketRequest request;
	while (true)
	{
		bool success = DequeueRequest(request);
		if (success)
		{
			struct ServerPacket packet;
			packet.PacketNum = request.PacketNum;
			CopyChunk(file_name, request.PacketNum, file_size, packet.Payload);
			SendPacket(packet, request.requestAddress);
		}
	}
}

void SendPacket (const struct ServerPacket & packet, struct sockaddr_in & addr)
{
	pthread_mutex_lock(&network_lock);
	auto result = sendto(socket_fd, &packet, sizeof(ServerPacket), 0, (struct sockaddr *)&addr, sizeof(sockaddr_in));
	if (result < 0)
	{
		perror("sendto");
	}
	pthread_mutex_unlock(&network_lock);
}






















