#include "client.h"
#include <err.h>
#include <iostream>

using namespace std;

//*************** Main Method ***************//
int main(int argc, char *argv[])
{
	//Check for input
	if (argc != 4)
	{
		fprintf(stderr,"Neccessary arguments: IP address, port, file name\n");
		exit(0);
	}
	
	//** Initialize variables used for socket communication **//
	//IP Address
	if (!inet_aton(argv[1], &ip))
        errx(1, "can't parse IP address %s", ipstr);
	if ((Server = gethostbyaddr((const void *)&ip, sizeof ip, AF_INET)) == NULL)
        errx(1, "no name associated with %s", ipstr);
	
	
	//Port Number
	PortNum = atoi(argv[2]);
	
}