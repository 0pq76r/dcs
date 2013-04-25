#include "tracker.h"

#define BUFFER_SIZE 1024

pthread_mutex_t lock_accept;

struct sender_data
{
	int socket;
	char data[BUFFER_SIZE];
	int data_lenght;
};

void *child_tracker_tx(void *send)
{
	int sock = ((struct sender_data*)send)->socket;
	char *buffer=((struct sender_data*)send)->data;
	pthread_mutex_lock(&recv_list_lock);
	int rx_sock;
	struct _fifo *currecv;
	
	for(currecv=recv_list; currecv!=NULL; currecv=currecv->next){
		if(currecv->data!=NULL && (rx_sock=*((int*)currecv->data))!=sock)
		{
			int sendt=0;
			while((((struct sender_data*)send)->data_lenght-sendt)>0)
			{
				sendt=write(rx_sock,&buffer[sendt],
					(((struct sender_data*)send)->data_lenght-sendt));
			}
		}
	}
	
	pthread_mutex_unlock(&recv_list_lock);
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
		pthread_mutex_lock(&recv_list_lock);
		recv_list=fifo_add(recv_list, &sock);
		pthread_mutex_unlock(&recv_list_lock);

		
		int data_lenght;

		while((data_lenght=read(sock, buffer, BUFFER_SIZE-2))>0){
			send=malloc(sizeof(struct sender_data));
			send->socket=sock;
			strncpy(send->data, buffer, BUFFER_SIZE-1);
			send->data_lenght=data_lenght;
			pthread_create( &sender, NULL, child_tracker_tx, send);
			bzero(buffer, BUFFER_SIZE);
		}
		pthread_join(sender, NULL);
		pthread_mutex_lock(&recv_list_lock);
		while(1)
		{
			if(recv_list->data==NULL)
			{
				recv_list=fifo_remove(recv_list);
			}
			else if(*((int*)recv_list->data)==sock)
			{
				recv_list=fifo_remove(recv_list);
				break;
			}
			else
			{
				recv_list=fifo_add(recv_list, recv_list->data);
				recv_list=fifo_remove(recv_list);
			}			
		}
		pthread_mutex_unlock(&recv_list_lock);
		printf("Remove Client\n");
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
	{
		printf("Can not bind in server!\n");
		return NULL;
	}
     
	listen(sock,5);
	
	pthread_mutex_init(&lock_accept, NULL);
	pthread_create( &childs[0], NULL, child_tracker_rx, &sock);
	pthread_create( &childs[1], NULL, child_tracker_rx, &sock);
	pthread_join(childs[0], NULL);
	pthread_join(childs[1], NULL);
    close(sock);
    
    return NULL;
}

