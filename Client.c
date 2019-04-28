#include <stdio.h>
#include <stdlib.h>

// Including Time function, sockets, htons... file stat libraries.
#include <sys/time.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <math.h>
#include <sys/stat.h>

// Including File function and bzero libraries. 
#include <fcntl.h>
#include <unistd.h>
#include <strings.h>

// Buffer size.
#define PACKETBUFFER 471

/* Declaring functions. */
// Function that displays sending time.
int duration (struct timeval *start, struct timeval *stop, struct timeval *delta);
// Function to create client socket.
int createClientSocket (int port, char* ipaddr);

// Structures
struct sockaddr_in sock_serv;   // Server socket.
struct timeval timeout;         // Timeout.
// Packet structure.
struct packets
{
	int sequenceNumber;
	char data[PACKETBUFFER];
};


// Main function.
int main (int argc, char**argv)
{	
	// Structures initialization.
    struct timeval start, stop, delta,timeout;
    int serverSocket, file;
   
	// Variables initialization.
    long int size;			
    int no_packets;
    int total_acks;
    struct packets packet_s0, packet_s1, packet_s2, packet_s3, packet_s4;
    char buf[PACKETBUFFER];
    off_t count=0, m, totalFileSize;
    
    long int packet0, packet1, packet2,packet3, packet4;
   
    int adressSize = sizeof(struct sockaddr_in);
	struct stat buffer;
    
	// Displaying error message if input is invalid.
	if (argc != 4)
	{
		printf("Error usage : %s <ip_serv> <port_serv> <filename>\n",argv[0]);
		return EXIT_FAILURE;
	}

    // Creating socket.
    serverSocket = createClientSocket(atoi(argv[2]), argv[1]);
    
	// Generating error message if file to be sent does not open.
	if ((file = open(argv[3],O_RDONLY))==-1)
	{
		perror("Cannot open file..");
		return EXIT_FAILURE;
	}
    
	// File size is stored in totalFileSize.
	if (stat(argv[3],&buffer)==-1)
	{
		perror("stat fail");
		return EXIT_FAILURE;
	}
	else
		totalFileSize = buffer.st_size;
    
	/* Sending file. */

	// Determination of total packets to be sent.	
	no_packets = ceil((float)totalFileSize/471);
	// Declaring array of the same size as the total number of packets. 
	struct packets packets_array[no_packets];
	// Initialization of ack array.	
	int acks_recv[no_packets];

	// Storing default ack.
	int i;
	for(i = 0; i < no_packets; i++)
	{
      		acks_recv[i]=-1;
   	}

	// Getting time of file.
    gettimeofileay(&start,NULL);
    // Adding packets into the array.
	int j;
	for (j = 0; j < no_packets; j++)
	{
		packet0 = read(file, packets_array[j].data, PACKETBUFFER);
	}

    // Setting the sequence numbers.
	int k;
    for(k=0; k<no_packets;k++)
	{
    	packets_array[k].sequenceNumber = k;
	}

	// Total number of acks for 5 packets.
    total_acks = 5;  
    
	// Initializing window sizes for packets and acks.
    int pack_window=0;
    int ack_window=0;
    int var_x=0;
    int count_1=0;


    /* Sending packets */

	// While loop till all packets are sent.
    while(pack_window != no_packets)
	{
		total_acks=0;

		// Sending the 5 packets.
		int u;
		for(u=0; u<5; u++)
		{
			// Checking if the sending fails.
    		if(sendto(serverSocket, &packets_array[pack_window], sizeof(packets_array[pack_window]), 0, (struct sockaddr*)&sock_serv, addressSize) == -1)
			{
				perror("Error Sending File...");
				return EXIT_FAILURE;
			}

			// Incrementing packet window.
			pack_window++;
		}

		// Waiting for the ack of the sent packets. Implementing Stop and Wait.
		while(1)
		{
			// Waiting to receive acks.
			recvfrom(serverSocket, &acks_recv[ack_window], 4, 0, (struct sockaddr*)&sock_serv, &addressSize);

			if(acks_recv[ack_window] == ack_window)
			{
				printf("Ack Recieved %d\n",acks_recv[ack_window]);
				total_acks++;
				ack_window++;
			}
			else
			{
				if (sendto(serverSocket, &packets_array[ack_window], sizeof(packets_array[ack_window]), 0, (struct sockaddr*)&sock_serv, addressSize) == -1)
				{
					perror("send error1");
					return EXIT_FAILURE;
				}
			}
			if(total_acks==5)
			{
				break;
			}
		}
	}
		

	// Getting file sending time.
	gettimeofileay(&stop,NULL);
	duration(&start,&stop,&delta);
    
	// Displaying information.
	printf("On a total size of: %lld \n",totalFileSize);
	printf("For a total duration of: %ld.%d \n",delta.tv_sec,delta.tv_usec);
    
	// Closing server socket.
    close(serverSocket);

	// Closing file.
    close(file);
    return EXIT_SUCCESS;}


// Function which gives the duration.
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


// Function to create socket.
int createClientSocket (int port, char* ipaddr)
{
    int addressSize;
	int serverSocket;
    
	serverSocket = socket(AF_INET,SOCK_DGRAM,0);
	if (serverSocket == -1)
	{
        perror("socket fail");
        return EXIT_FAILURE;
	}
    
    // Preparing the address of the destination socket
	addressSize = sizeof(struct sockaddr_in);
	bzero(&sock_serv,addressSize);
	
	// Socket operations.
	sock_serv.sin_family=AF_INET;
	sock_serv.sin_port=htons(port);
	timeout.tv_sec = 5;
    timeout.tv_usec = 0;

	// Displaying if user enters invalid IP.
    if (inet_pton(AF_INET,ipaddr,&sock_serv.sin_addr)==0)
	{
		printf("Invalid IP Adress\n");
		return EXIT_FAILURE;
	}
	
	// Implementing timeout error.
	if (setsockopt(serverSocket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) 
	{
    		perror("Error");
	}
    
    return serverSocket;
}
