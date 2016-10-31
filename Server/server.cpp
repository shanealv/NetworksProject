#include <cstdlib>
#include <cstring>
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

int main(int argc, char *argv[])
{
	sockaddr_in myaddr;	/* our address */
	socklen_t addrlen = sizeof(sockaddr_in);		/* length of addresses */
	int fd;					/* our socket */

	/* create a UDP socket */
	if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		cerr << "cannot create socket" << endl;
		return 0;
	}

	/* bind the socket to any valid IP address and a specific port */
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(PORTNUM);

	if (bind(fd, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
	{
		cerr << "bind failed" << endl;
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
		
		ClientPacket data;
		PacketRequest request;
		int length = recvfrom(fd, &data, sizeof(ClientPacket), 0, (sockaddr *)&request.RequestAddress, &addrlen);
		if (length <= 0)
		{
			cerr << "Uh oh - something went wrong!" << endl;
			continue;
		}

		cout << "Received request for packet " << data.PacketNum << endl;
		request.PacketNum = data.PacketNum;
		QueueRequest(request);
		usleep(200); // give time for the other threads to work
	}
	/* never exits */
}