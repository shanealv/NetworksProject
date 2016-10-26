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

#define PORTNUM 8081
#define BUFSIZE 2048

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
	char buf[BUFSIZE];	/* receive buffer */
	ClientPacket *rcvBuff = (ClientPacket *)malloc(sizeof(ClientPacket));
	ServerPacket *sendBuff = (ServerPacket *)malloc(sizeof(ServerPacket));
	ServerPacket newSend;


	/* create a UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
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
	for (;;) {
		printf("waiting on port %d\n", PORTNUM);
		recvlen = recvfrom(fd, rcvBuff, BUFSIZE, 0, (struct sockaddr *)&remaddr, &addrlen);
		if (recvlen > 0) {
			//buf[recvlen] = 0;
			printf("received message: \"%d\" (%d bytes)\n", rcvBuff->PacketNum, recvlen);
		}
		else
			printf("uh oh - something went wrong!\n");
		sprintf(buf, "ack %d", msgcnt++);
		printf("sending response \"%s\"\n", buf);
		
		//Create Server Packet
		newSend.PacketNum = rcvBuff->PacketNum;
		memcpy(sendBuff, &(newSend), sizeof(ServerPacket));
		
		if (sendto(fd, sendBuff, sizeof(ServerPacket), 0, (struct sockaddr *)&remaddr, addrlen) < 0)
			perror("sendto");
	}
	/* never exits */
}