#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "../Shared/Packets.h"
#include "../Shared/FileService.h"

#define PORTNUM 8082

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

	/* now loop, receiving data and printing what we received */
	while(1)
	{
		printf("waiting on port %d\n", PORTNUM);
		recvlen = recvfrom(fd, rcvBuff, sizeof(ClientPacket), 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0)
			printf("received request for packet %d\n", rcvBuff->PacketNum);
		else
			printf("uh oh - something went wrong!\n");
		
		//Create Server Packet
		if(rcvBuff->PacketNum == -1)
			newSend.PacketNum = 8192;
		else
		{
			newSend.PacketNum = rcvBuff->PacketNum;
			for(int i = 0; i < PAYLOAD_SIZE; i++)
				newSend.Payload[i] = 'c';
		}
		
		memcpy(sendBuff, &(newSend), sizeof(ServerPacket));
		printf("sending SeverPacket with chunk %d\n", sendBuff->PacketNum);
		
		if (sendto(fd, sendBuff, sizeof(ServerPacket), 0, (struct sockaddr *)&remaddr, addrlen) < 0)
			perror("sendto");
	}
	/* never exits */
}