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
	
	//Initialize variables
	CurrentWindowBase = 0;
	WindowSize = 10;
	sendBuff = (ClientPacket *)malloc(sizeof(ClientPacket));
	recvBuff = (ServerPacket *)malloc(sizeof(ServerPacket));
	WindowManager = (WindowSectionWrapper *)malloc(WindowSize * sizeof(WindowSectionWrapper));
	
	portno = atoi(argv[2]);
	server = (char *)malloc(strlen(argv[1]));
    server[0] = '\0';
	strcat(server, argv[1]);
	
	//Create client's socket
	InitSocket();
	
	//This will be in InitRequest
	for(int i = 0; i < WindowSize; i++)
	{
		WindowManager[i].PacketNum = i;
		WindowManager[i].LoadFull = 0;
	}
	
	Request(CurrentWindowBase, CurrentWindowBase + WindowSize);
	Receive();
	close(fd);

	return 0;
}


void Request(int packetNumFirst, int packetNumLast)
{
	ClientPacket requestPacket;

	/* now let's send the messages */
	for(int i=packetNumFirst; i <= packetNumLast; i++)
	{
		cout << "Sending packet "<< i << " to " << server << " port " << portno << endl;
		
		requestPacket.PacketNum = i;
		
		//sprintf(buf, "This is packet %d", i);
		memcpy(sendBuff, &(requestPacket), sizeof(ClientPacket));
		if (sendto(fd, sendBuff, sizeof(ClientPacket), 0, (struct sockaddr *)&remaddr, slen)==-1)
		{
			perror("sendto");
			exit(1);
		}
	}
}

void Receive()
{
	//Request all data out of OS input buffer
	int tempBase = CurrentWindowBase;
	
	do
	{
		recvlen = recvfrom(fd, recvBuff, sizeof(ServerPacket), MSG_DONTWAIT, (struct sockaddr *)&remaddr, &slen);
		if (recvlen >= 0)	//If a ServerPacket was recieved
		{
			//Add the data from the recieved ServerPacket into the WindowManager
			for(int i = tempBase; i < tempBase + WindowSize; i++)
			{
				//Check to make sure the packet is in the current window or if
				//the packet has already been recieved
				if(WindowManager[i].PacketNum == recvBuff->PacketNum && !(WindowManager[i].LoadFull))
				{
					memcpy(WindowManager[i].Payload, &(recvBuff->Payload), PAYLOAD_SIZE);
					WindowManager[i].LoadFull = 1;
					
					cout << "received ack for: \""<< recvBuff->PacketNum <<"\"" << endl;
					cout << "\t payload size: " << recvlen << endl;
					break;
				}
			}
			
		}
	} while(recvlen > 0);
}

void InitSocket()
{
	/* create a socket */
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
		exit(1);
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
}

