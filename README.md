# NetworksProject

### How to compile ###
use the 'make' command in the top directory. To remove all executables and .o files use the 'make clean' command

### How to run Server ###
After compiling, run the 'server' executable with the arguments [name of file to serve, port number to listen on], respectively. The server will then start listening on the specified port.

### How to run Client ###
After compiling, run the 'client' executable with the arguments [IP address of the server, the port nuber the server is listening on, the filename which you want to save the retrieved file under], respectively. Please note the filename must not have any spaces. If the server is listening at the specified address and port, the file will begin downloading in the same directory as the executable.


### Implementation notes ###
# Communication Logic
Our system is completely Client driven. The Server's only job is to respond to any immediate requests it recieves.

The Server does not keep any record of its clients and does not send or recieve any acks. It will respond to the Client that specifies a packet number in a request. The Server will then send a chunk of the file being served that corresponds to that number. If the packet number is -1, the server will send the size of the file in a packet to the Client who made the request.

The Client first requests packet -1. It should recieve the size of the file being served. It then preallocates a file of that size, calculates how many chunks the file will be served in, and start requesting those chunks in packets. The client is in charge of making sure it writes the chunks in order, ignores duplicate chunks, and doesn't request the a packet it already received. The Client doesn't send or receive ACKs. If it doesn't receive a packet before it checks for received data, it will request that packet again.

# Thread pool in Server
To implement concurrency of file serving, the server has a thread pool of worker threads that are constantly trying to read jobs from a queue. The queue is protected by a mutex. Jobs specify a particular chunk of the file being served. Each worker thread will copy that chunk into a packet and send it to the client that requested it.

# Pipelining and selective repeat in Client
The Client will request all packets in a window at once. The window at any time has a base n and bound of n + 24 (25 packets in total). Once all packets have been requested, the client will then recieve the packets from its input buffer. It will write the packet that corresponds to the window base to the file being received, advance the window base, and repeat. Once the entire window has been written or the packet corresponding to the window base is not found, the client requests packets for the new window. The client will not request any packets it has recieved and will ignore any duplicate packets it recieves.