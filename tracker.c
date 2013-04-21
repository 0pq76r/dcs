#include "tracker.h"

#define BUFFER_SIZE 1024

pthread_mutex_t lock_accept;
pthread_mutex_t lock_write;

struct sender_data
{
	int socket;
	char data[BUFFER_SIZE];
};

void *child_tracker_tx(void *send)
{
	int sock = ((struct sender_data*)send)->socket;
	char *buffer=((struct sender_data*)send)->data;

	printf("%s", sock, buffer);
	
	free(send);	
	return NULL;
}

void *child_tracker_rx(void *_sock)
{
    int sock;
    pthread_t sender;
	char buffer[BUFFER_SIZE];
	struct sender_data *send;
    
    printf("Waiting for Client\n");
    
    while(1){
		socklen_t slen=sizeof(struct sockaddr);
		struct sockaddr saddr;
		
		pthread_mutex_lock(&lock_accept);
		sock=accept(*((int*)_sock), &saddr, &slen);
		pthread_mutex_unlock(&lock_accept);

		printf("Accept Client\n");

		while(read(sock, buffer, BUFFER_SIZE-2)>0){
			send=malloc(sizeof(struct sender_data));
			send->socket=sock;
			strncpy(send->data, buffer, BUFFER_SIZE-1);
			pthread_create( &sender, NULL, child_tracker_tx, send);
			bzero(buffer, BUFFER_SIZE);
		}
		pthread_join(sender, NULL);
	}

	close(sock);	
	return NULL;
}

void *tracker(void *vport){
	
	int sock;
	int port=*((int*)vport);
	struct sockaddr_in server;
	
	pthread_t childs[2];
 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failed to create socket");
	}

	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
 
	if(bind(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
		printf("Can not bind in server!\n");
     
	listen(sock,5);
	
	pthread_mutex_init(&lock_accept, NULL);
	pthread_mutex_init(&lock_write, NULL);
	pthread_create( &childs[0], NULL, child_tracker_rx, &sock);
	pthread_create( &childs[1], NULL, child_tracker_rx, &sock);
	pthread_join(childs[0], NULL);
	pthread_join(childs[1], NULL);
    close(sock);
    
    return NULL;
}

