#include "socket.h"

#define BUFFER_SIZE 1024

void *tracker(void *vport){
	int sock, n;
	int port=*((int*)vport);
	socklen_t fromlen;
	char buffer[BUFFER_SIZE]={0};
	struct sockaddr_in server;
	struct sockaddr_in client;
 
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		perror("Failed to create socket");
	}

	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
 
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		printf("Can not bind in server!\n");
     
	fromlen = sizeof(struct sockaddr_in);
	while(1) {
		unsigned int gen;
		n = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client, &fromlen);
		if (n < 0) 
			printf("Error when receiving in server!\n");
//		nr = ntohl(nr);
		gen = ntohs(client.sin_port);
		printf("I have received %s from IP %s and port %u \n",buffer,inet_ntoa(client.sin_addr), gen);
		bzero(buffer, BUFFER_SIZE);
	}
}

