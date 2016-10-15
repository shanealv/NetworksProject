#include "client.h"

//*************** Main Method ***************//
int main(int argc, char *argv[])
{
	//Check for input
	if (argc != 4)
	{
		fprintf(stderr,"Neccessary arguments: IP address, port, file name\n");
		exit(0);
	}
	
	//Initialize variables used for socket communication
	PortNum = atoi(argv[2]);
	
}