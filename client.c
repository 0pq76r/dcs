#include "client.h"

#define BUFFER_SIZE 1024

pthread_mutex_t lock_accept;

void client(char* dst_ip, int dst_port){
	
	int sock;
	struct sockaddr_in server;
	char buffer[BUFFER_SIZE];
 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Failed to create socket");
	}

	memset(&server, 0, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(dst_port);
	inet_aton(dst_ip, &(server.sin_addr));
 
	if(connect(sock, (struct sockaddr*)&server, sizeof(struct sockaddr))==-1)
	{
		printf("Working as Tracker only");
		return;
	}
	
	pthread_mutex_lock(&recv_list_lock);
	recv_list=fifo_add(recv_list, &sock);
	pthread_mutex_unlock(&recv_list_lock);

		
	int data_lenght;
	struct sender_data *send;

	while((data_lenght=read(sock, buffer, BUFFER_SIZE-2))>0){
		send=malloc(sizeof(struct sender_data));
		send->socket=sock;
		strncpy(send->data, buffer, BUFFER_SIZE-1);
		send->data_lenght=data_lenght;
		child_tracker_tx(send);
		bzero(buffer, BUFFER_SIZE);
	}


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
		
    close(sock);
    
    return;
}

