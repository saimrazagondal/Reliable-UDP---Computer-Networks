#include <stdio.h>
#include <stdlib.h>

// Including Time function, sockets, htons... file stat librairies.
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>

// Including File function and bzero libraries.
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

// Total size of Buffer.
#define PACKETBUFFER 471

/* Function declaration. */
// Function for receiving duration.
int duration (struct timeval *start, struct timeval *stop, struct timeval *delta);
// Functiong to creating server socket and binding it.
int createServerSocket (int port);

// Creating server and client sockets.
struct sockaddr_in sock_serv, client;

// Packet structure.
struct packets
{
	int sequenceNumber;
	char data[PACKETBUFFER];
};

// Main function.
int main (int argc, char**argv)
{
    // Creating variables which will recieve the packets.
	int file, serverSocket;	 // file is used for file input.
 	struct packets * packet_recv0 = malloc(sizeof(struct packets));
   	struct packets * packet_recv1 = malloc(sizeof(struct packets));
	struct packets * packet_recv2 = malloc(sizeof(struct packets));
	struct packets * packet_recv3 = malloc(sizeof(struct packets));
	struct packets * packet_recv4 = malloc(sizeof(struct packets));
	struct packets * packet_seq[100000];

	// Long date type variables.
	off_t count = 0;                               // Variable to store transferred bytes.
	off_t packet0,packet1,packet2,packet3,packet4; // Packet numbers. 
	char filename[256];							   // File to be transferred.
	int ack_send0, ack_send1, ack_send2, ack_send3, ack_send4=-1; // Acks for received packets.
    unsigned int size = sizeof(struct sockaddr_in);  // Size of address.		
	int frame_id = 0;  // ?
	
    // Variable for the date.
	time_t intps;    // ?
	struct tm* tmi;  // ?
    
	// Preventing invalid input. Prompting user enter valid arguements.
	if (argc != 2)
	{
		printf("Error usage : %s <port_serv>\n",argv[0]);
		return EXIT_FAILURE;
	}

    // Socket creation.
    serverSocket = createServerSocket(atoi(argv[1]));
    
	intps = time(NULL);         // ?
	tmi = localtime(&intps);    // ?

	// Function which initializes the arguement passed to it to 0.
	bzero(filename,256);       

	// Time displayed
	sprintf(filename,"client.%d.%d.%d.%d.%d.%d",tmi->tm_mday,tmi->tm_mon+1,1900+tmi->tm_year,tmi->tm_hour,tmi->tm_min,tmi->tm_sec);

	// Displaying progress.
	printf("Creating the output file : %s\n",filename);
    
	// Opening file.
	if((file = open(filename,O_CREAT|O_WRONLY|O_TRUNC,0600)) == -1)
	{
		perror("open fail");
		return EXIT_FAILURE;
	};
    

	// Receiving file.
	packet0 = 1;  
	int write_var=0;
	while(packet0)
	{
		// Format of recvfrom = <serversocket>, <buffer>, <buffer size>, <flags>, <client address>, <size of client>.
		packet0 = recvfrom(serverSocket, packet_recv0, sizeof(*packet_recv0), 0, (struct sockaddr *)&client, &size);
		// Reveiving ack for packet 0.
		ack_send0 = packet_recv0->sequenceNumber;
		// Storing sequence number of receiving packet to sequence number array.
		packet_seq[packet_recv0->sequenceNumber] = packet_recv0;
		// Sending ack of packet 0 to the client.
		sendto(serverSocket, &ack_send0, 4, 0, (struct sockaddr*)&client, size);

		// Displaying progress.
		printf("Packet Recieved %d\n", packet_recv0->sequenceNumber);

		// Displaying error if packet is not read.
		if(packet0 == -1)
		{
			perror("read fails");
			return EXIT_FAILURE;
		};

		// Format of recvfrom = <serversocket>, <buffer>, <buffer size>, <flags>, <client address>, <size of client>.
		packet1 = recvfrom(serverSocket, packet_recv1, sizeof(*packet_recv1), 0, (struct sockaddr *)&client, &size);
		// Reveiving ack for packet 1.
		ack_send1 = packet_recv1->sequenceNumber;
		// Storing sequence number of receiving packet to sequence number array.
		packet_seq[packet_recv1->sequenceNumber] = packet_recv1;
		// Sending ack of packet 1 to the client.
		sendto(serverSocket, &ack_send1, 4, 0, (struct sockaddr*)&client, size);

		// Displaying progress.
		printf("Packet Recieved %d\n", ack_send1);

		// Displaying error if packet is not read.
		if(packet1 == -1)
		{
			perror("read fails");
			return EXIT_FAILURE;
		};

		// Format of recvfrom = <serversocket>, <buffer>, <buffer size>, <flags>, <client address>, <size of client>.
		packet2 = recvfrom(serverSocket, packet_recv2, sizeof(*packet_recv2), 0, (struct sockaddr *)&client, &size);
		// Reveiving ack for packet 2.
		ack_send2 = packet_recv2->sequenceNumber;
		// Storing sequence number of receiving packet to sequence number array.
		packet_seq[packet_recv2->sequenceNumber] = packet_recv2;
		// Sending ack of packet 2 to the client.
		sendto(serverSocket, &ack_send2, 4, 0, (struct sockaddr*)&client, size);

		// Displaying progress.
		printf("Packet Recieved %d\n", ack_send2);

		// Displaying error if packet is not read.
		if(packet2 == -1)
		{
			perror("read fails");
			return EXIT_FAILURE;
		};

		// Format of recvfrom = <serversocket>, <buffer>, <buffer size>, <flags>, <client address>, <size of client>.
		packet3 = recvfrom(serverSocket, packet_recv3, sizeof(*packet_recv3), 0, (struct sockaddr *)&client, &size);
		// Reveiving ack for packet 3.
		ack_send3 = packet_recv3->sequenceNumber;
		// Storing sequence number of receiving packet to sequence number array.
		sendto(serverSocket, &ack_send3, 4, 0, (struct sockaddr*)&client, size);
		// Sending ack of packet 3 to the client.
		packet_seq[packet_recv3->sequenceNumber] = packet_recv3;

		// Displaying progress.
		printf("Packet Recieved %d\n",ack_send3);

		// Displaying error if packet is not read.
		if(packet3 == -1)
		{
			perror("read fails");
			return EXIT_FAILURE;
		};

		// Format of recvfrom = <serversocket>, <buffer>, <buffer size>, <flags>, <client address>, <size of client>.
		packet4 = recvfrom(serverSocket, packet_recv4, sizeof(*packet_recv4), 0, (struct sockaddr *)&client, &size);
		// Reveiving ack for packet 4.
		ack_send4 = packet_recv4->sequenceNumber;
		// Storing sequence number of receiving packet to sequence number array.
		packet_seq[packet_recv4->sequenceNumber] = packet_recv4;
		// Sending ack of packet 4 to the client.
		sendto(serverSocket, &ack_send4, 4, 0, (struct sockaddr*)&client, size);

		// Displaying progress.
		printf("Packet Recieved %d\n", ack_send4);

		// Displaying error if packet is not read.
		if(packet4 == -1)
		{
			perror("read fails");
			return EXIT_FAILURE;
		};

		// Re-ordering of the received packets and storing them in a file.
		int i;
		for(i=0 ; i<5; i++)
		{
			// Writing bytes to file in server.
			write(file,packet_seq[write_var]->data,PACKETBUFFER);
			write_var++;
		}
	};

    // Storing the total bytes received in int variable. 
	count = count + packet0 + packet1 + packet2 + packet3 + packet4;
	
	// Displaying number of bytes that are received.
    printf("Number of bytes transferred: %lld \n",count);
    
	// Closing server socket.
    close(serverSocket);

	// Closing file.
    close(file);

	return EXIT_SUCCESS;
}



// Function allowing the calculation of the duration of the receiving.
int duration (struct timeval *start,struct timeval *stop,struct timeval *delta)
{
    suseconds_t microstart, microstop, microdelta;
    
    microstart = (suseconds_t) (100000*(start->tv_sec))+ start->tv_usec;
    microstop = (suseconds_t) (100000*(stop->tv_sec))+ stop->tv_usec;
    microdelta = microstop - microstart;
    
    delta->tv_usec = microdelta%100000;
    delta->tv_sec = (time_t)(microdelta/100000);
    
    if((*delta).tv_sec < 0 || (*delta).tv_usec < 0)
        return -1;
    else
        return 0;
}


// Function to create socket and binding it.
int createServerSocket (int port)
{
	// Variables.
    int size;
	int serverSocket;
    
	// Creating server socket for UDP.
	serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

	// Displaying error if socket does not create.
	if (serverSocket == -1)
	{
        perror("Failed to create socket.");
        return EXIT_FAILURE;
	}
    
    // Preparing the address of the destination socket.
	size = sizeof(struct sockaddr_in);

	// Initializing to 0 of size given in parameter.
	bzero(&sock_serv, size);
	
	// Setting server side.
	sock_serv.sin_family = AF_INET;
	sock_serv.sin_port = htons(port);
	sock_serv.sin_addr.s_addr = htonl(INADDR_ANY);
    
	// Assigning identity to the socket and displaying error message if this fails.
	if(bind(serverSocket, (struct sockaddr*)&sock_serv, size) == -1)
	{
		perror("bind fail");
		return EXIT_FAILURE;
	}
 
    return serverSocket;
}

