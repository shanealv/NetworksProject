#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <queue>
#include "../Shared/Packets.h"
#include "../Shared/FileService.h"
#include "worker.h"
#include <time.h>

using namespace std;

int socket_fd;
int file_fd;
long file_size;
long file_blocks;
pthread_t * workers;
pthread_mutex_t file_lock;
pthread_mutex_t queue_lock;
pthread_mutex_t network_lock;

queue<PacketRequest> * requestQueue = new queue<PacketRequest>();

PacketRequest::PacketRequest()
{
	PacketNum = -1;
}

PacketRequest::PacketRequest(int packetNum, sockaddr_in requestAddress)
{
	PacketNum = packetNum;
	memcpy(&RequestAddress, &requestAddress, sizeof(sockaddr_in));
}

PacketRequest::PacketRequest(const PacketRequest& other)
{
	if (this != &other)
	{
		PacketNum = other.PacketNum;
		memcpy(&RequestAddress, &other.RequestAddress, sizeof(sockaddr_in));
	}
}

PacketRequest& PacketRequest::operator=(const PacketRequest& other)
{
	if (this != &other)
	{
#ifdef DEBUG
		cout << "COPY ASSIGN " << other.PacketNum << endl;
#endif 
		PacketNum = other.PacketNum;
		memcpy(&RequestAddress, &other.RequestAddress, sizeof(sockaddr_in));
	}
	return *this;
}

bool InitThreads(int numThreads, const char * filename, int fd)
{
#ifdef DEBUG
	srand(time(NULL));
#endif 
#ifdef DEBUG
	cout << "Initializing " << numThreads << " threads using the file: " << filename << endl;
#endif 
	socket_fd = fd;
	if (numThreads < 1)
	{
		cerr << "Error: Invalid Number of Threads" << endl;
		return false;
	}
	
	workers = new pthread_t[numThreads];
	if (pthread_mutex_init(&file_lock, NULL)    != 0 ||
		pthread_mutex_init(&queue_lock, NULL)   != 0 ||
		pthread_mutex_init(&network_lock, NULL) != 0)
	{
		cerr << "Error: Mutex intialization failed" << endl;
		return false;
	}
	
	file_fd = open(filename, O_RDONLY, 0);
	file_size = (long) filesize(filename);
	file_blocks = GetNumChunks(file_size);
	cout << "Using file: " << filename << "   Size: " << file_size << " bytes   Chunks: " << file_blocks << endl;
	
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

void QueueRequest(PacketRequest request)
{
	pthread_mutex_lock(&queue_lock);
	requestQueue->push(request);
#ifdef DEBUG
	cout << "[Queued] Packet Number " << request.PacketNum << endl;
#endif
	pthread_mutex_unlock(&queue_lock);
}

bool DequeueRequest(PacketRequest & request)
{
	bool success = false;
	pthread_mutex_lock(&queue_lock);
	if (!requestQueue->empty())
	{
		request = requestQueue->front();
#ifdef DEBUG
		cout << "[Dequeuing] " << request.PacketNum << endl;
#endif
		requestQueue->pop();
#ifdef DEBUG
		cout << "[Dequeued]  " << request.PacketNum << endl;
#endif
		success = true;
	}
	pthread_mutex_unlock(&queue_lock);
	return success;
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
			if (request.PacketNum == -1)
			{
				packet.PacketNum = (int) file_size;
				SendPacket(&packet, request.RequestAddress);
				continue;
			}
			
			packet.PacketNum = request.PacketNum;
			pthread_mutex_lock(&file_lock);
			CopyChunk(file_fd, request.PacketNum, file_size, packet.Payload);
			pthread_mutex_unlock(&file_lock);
			SendPacket(&packet, request.RequestAddress);
		}
		usleep(500);
	}
}

void SendPacket (const ServerPacket * packet, sockaddr_in & addr)
{
	cout << "[Sent] Packet Number " << packet->PacketNum << endl;

#ifdef DEBUG
	int boo = rand() % 100;
	if (boo < 25)
	{
		cout << "BOO!!!!" << endl;
		cout << "BOO!!!!" << endl;
		cout << "BOO!!!!" << endl;
		cout << "BOO!!!!" << endl;
		return;
	}
#endif

	pthread_mutex_lock(&network_lock);
	int result = sendto(socket_fd, packet, sizeof(ServerPacket), 0, (sockaddr *)&addr, sizeof(sockaddr_in));
	pthread_mutex_unlock(&network_lock);
	if (result < 0)
	{
		cerr << "ERROR: failed to send packet";
	}
}






















