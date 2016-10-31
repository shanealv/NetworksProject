//my IP: 192.168.0.21
//school IP: 172.24.102.8

#include "client.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
	{
		cout << "usage %s: [hostname, port, file name (no spaces)]\n" << argv[0] << endl;
		exit(0);
    }
	
	//Initialize variables
	CurrentWindowBase = 0;
	TotalPackets = 0;
	SendBuffer = (ClientPacket *)malloc(sizeof(ClientPacket));
	RecvBuffer = (ServerPacket *)malloc(sizeof(ServerPacket));
	WindowManager = (WindowSectionWrapper *)malloc(WINDOW_SIZE * sizeof(WindowSectionWrapper));
	
	PortNum = atoi(argv[2]);
	Server = (char *)malloc(strlen(argv[1]));
    Server[0] = '\0';
	strcat(Server, argv[1]);
	
	FileName = (char *)malloc(strlen(argv[3]));
    FileName[0] = '\0';
	strcat(FileName, argv[3]);
	
	//Create client's socket
	InitSocket();
	
	//Make initial request
	InitRequest();
	
	//Receive file
	while(1)
	{
		Request(CurrentWindowBase, CurrentWindowBase + WINDOW_SIZE);
		Receive();
		DumpWindow();
		
		if(TotalPackets <= CurrentWindowBase + 1)
		{
			cout << "File transfer complete!" << endl;
			close(FileDescriptor);
			break;
		}
	}
	
	close(FileDescriptor);

	return 0;
}

//Request the -1th chunk. ASSUMED: the size of the file is in ServerPacket.PacketNum
void InitRequest()
{
	Request(-1,-1);
	RecvLength = recvfrom(FileDescriptor, RecvBuffer, sizeof(ServerPacket), MSG_WAITALL, (struct sockaddr *)&ServerAddr, &ServerSize);
	
	FileSize = RecvBuffer->PacketNum;
	cout << "Size of the file is " << FileSize << endl;
	AllocateFile(FileName,RecvBuffer->PacketNum);
	
	TotalPackets = (int)GetNumChunks(FileSize) - 1;

	for(int i = 0; i < WINDOW_SIZE; i++)
	{
		WindowManager[i].PacketNum = i;
		WindowManager[i].LoadFull = 0;
	}
}

void Request(int packetNumFirst, int packetNumLast)
{
	ClientPacket requestPacket;
	
	//Don't request packets beyond the total number of packets
	if(packetNumLast > TotalPackets)
		packetNumLast = TotalPackets;

	/* Send requests */
	for(int i=packetNumFirst; i <= packetNumLast; i++)
	{
		//Do not request duplicate packets
		for(int j = 0; j < WINDOW_SIZE; j++)
			if( (WindowManager[j].PacketNum == i && !(WindowManager[j].LoadFull))
				|| i == -1)
			{
				cout << "Requesting packet "<< i << " from " << Server << ":" << PortNum << endl;
				
				requestPacket.PacketNum = i;
				
				memcpy(SendBuffer, &(requestPacket), sizeof(ClientPacket));
				if (sendto(FileDescriptor, SendBuffer, sizeof(ClientPacket), 0, (struct sockaddr *)&ServerAddr, ServerSize)==-1)
				{
					perror("sendto");
					exit(1);
				}
				
				break;
			}
		
		usleep(6000);
	}
	
}

void Receive()
{
	//Request all data out of OS input buffer
	do
	{
		RecvLength = recvfrom(FileDescriptor, RecvBuffer, sizeof(ServerPacket), MSG_DONTWAIT, (struct sockaddr *)&ServerAddr, &ServerSize);
		if (RecvLength >= 0)	//If a ServerPacket was recieved
		{
			//Add the data from the recieved ServerPacket into the WindowManager
			for(int i = 0; i < WINDOW_SIZE; i++)
			{
				//Check to make sure the packet is in the current window or if
				//the packet has already been recieved
				if(WindowManager[i].PacketNum == RecvBuffer->PacketNum && !(WindowManager[i].LoadFull))
				{
					memcpy(WindowManager[i].Payload, &(RecvBuffer->Payload), PAYLOAD_SIZE);
					WindowManager[i].LoadFull = 1;
					
					cout << "received packet "<< RecvBuffer->PacketNum
						<< " w/ payload size " << RecvLength - 4 << endl;
					break;
				}
			}
			
		}
	} while(RecvLength > 0);
}

void DumpWindow()
{
	//Write all buffers in he window to the file, moving the window when neccessary
	for(int i = 0; i < WINDOW_SIZE; i++)
	{
		if(WindowManager[i].PacketNum == CurrentWindowBase && WindowManager[i].LoadFull)
		{
			SaveChunk(FileName, WindowManager[i].PacketNum, FileSize, WindowManager[i].Payload);
			
			cout << "Chunk #" << WindowManager[i].PacketNum << " written to file" << endl;
			
			if(TotalPackets == CurrentWindowBase + 1)
			{
				cout << "done" << endl;
				return;
			}
			
			WindowManager[i].PacketNum = CurrentWindowBase + WINDOW_SIZE;
			WindowManager[i].LoadFull = 0;
			CurrentWindowBase++;
		}
	}
}

void InitSocket()
{
	/* create a socket */
	if ((FileDescriptor=socket(AF_INET, SOCK_DGRAM, 0))==-1)
		cout << "socket created" << endl;

	/* bind it to all local addresses and pick any port number */
	memset((char *)&ClientAddr, 0, sizeof(ClientAddr));
	ClientAddr.sin_family = AF_INET;
	ClientAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	ClientAddr.sin_port = htons(0);
	
	if(bind(FileDescriptor, (struct sockaddr *)&ClientAddr, sizeof(ClientAddr)) < 0)
	{
		perror("bind failed");
		exit(1);
	}       

	/* now define ServerAddr, the address to whom we want to send messages */
	/* For convenience, the host address is expressed as a numeric IP address */
	/* that we will convert to a binary format via inet_aton */
	memset((char *) &ServerAddr, 0, sizeof(ServerAddr));
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(PortNum);
	if (inet_aton(Server, &ServerAddr.sin_addr)==0)
	{
		fprintf(stderr, "inet_aton() failed\n");
		exit(1);
	}
}