//my IP: 192.168.0.21
// 172.24.102.8

#include "client.h"
//using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 3)
	{
       cout << "usage %s: [hostname, port]\n" << argv[0] << endl;
       exit(0);
    }
	
	sendBuff = (ClientPacket *)malloc(sizeof(ClientPacket));
	recvBuff = (ServerPacket *)malloc(sizeof(ServerPacket));
	portno = atoi(argv[2]);
	server = (char *)malloc(strlen(argv[1]));
    server[0] = '\0';
	strcat(server, argv[1]);
	
	
	Request(7, 19);

	return 0;
}


void Request(int packetNumFirst, int packetNumLast)
{
	/* create a socket */
	ClientPacket cPacket;

	if ((fd=socket(AF_INET, SOCK_DGRAM, 0))==-1)
		cout << "socket created" << endl;

	/* bind it to all local addresses and pick any port number */
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(0);
	
	if(bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		perror("bind failed");
		return;
	}       

	/* now define remaddr, the address to whom we want to send messages */
	/* For convenience, the host address is expressed as a numeric IP address */
	/* that we will convert to a binary format via inet_aton */
	memset((char *) &remaddr, 0, sizeof(remaddr));
	remaddr.sin_family = AF_INET;
	remaddr.sin_port = htons(portno);
	if (inet_aton(server, &remaddr.sin_addr)==0)
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}

	/* now let's send the messages */
	for (int i=packetNumFirst; i <= packetNumLast; i++)
	{
		cout << "Sending packet "<< i << " to " << server << " port " << portno << endl;
		
		cPacket.PacketNum = i;
		
		//sprintf(buf, "This is packet %d", i);
		memcpy(sendBuff, &(cPacket), sizeof(ClientPacket));
		if (sendto(fd, sendBuff, sizeof(ClientPacket), 0, (struct sockaddr *)&remaddr, slen)==-1)
		{
			perror("sendto");
			exit(1);
		}
		/* now receive an acknowledgement from the server */
		recvlen = recvfrom(fd, recvBuff, sizeof(ServerPacket), 0, (struct sockaddr *)&remaddr, &slen);
		if (recvlen >= 0)
		{
				//buf[recvlen] = 0;	/* expect a printable string - terminate it */
				cout << "received ack for: \""<< recvBuff->PacketNum <<"\"" << endl;
		}
	}
	close(fd);

}


