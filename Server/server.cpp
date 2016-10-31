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
#include "../Shared/Packets.h"
#include "../Shared/FileService.h"
#include "worker.h"

#define PORTNUM 8082

using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
	struct sockaddr_in myaddr;	/* our address */
	struct sockaddr_in remaddr;	/* remote address */
	socklen_t addrlen = sizeof(remaddr);		/* length of addresses */
	int recvlen;			/* # bytes received */
	int fd;				/* our socket */
	int msgcnt = 0;			/* count # of messages we received */
	ClientPacket *rcvBuff = (ClientPacket *)malloc(sizeof(ClientPacket));
	ServerPacket *sendBuff = (ServerPacket *)malloc(sizeof(ServerPacket));
	ServerPacket newSend;


	/* create a UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("cannot create socket\n");
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(PORTNUM);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("bind failed");
		return 0;
	}
	
	bool success = InitThreads (4, "skeleton.gif", fd);
	if (!success)
	{
		cout << "failed to intialize server workers" << endl;
		return 0;
	}
	
	/* now loop, receiving data and printing what we received */
	while(true)
	{
		cout << "Waiting on port " << PORTNUM << "..." << endl;
		recvlen = recvfrom(fd, rcvBuff, sizeof(ClientPacket), 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0)
			cout << "Received request for packet " << rcvBuff->PacketNum << endl;
		else
		{
			cerr << "Uh oh - something went wrong!" << endl;
			continue;
		}
		
		PacketRequest request(rcvBuff->PacketNum, remaddr);
		
		QueueRequest(request);
		usleep(5000); // give time for the other threads to work
	}
	/* never exits */
}