#include "main.h"

int main(int argc, char **argv){
	pthread_t tracker_thread;
	int local_port;
	int remote_port;
	char remote_ip[1024];
	
	//parse args
	while((optopt=getopt(argc, argv, "p:r:"))!=-1){
		if(optopt=='p') sscanf(optarg, "%d", &local_port);
		if(optopt=='r')	sscanf(optarg, "%[^:]:%d", remote_ip, &remote_port);
	}
	
	printf("Bind on port %d\nConnect to %s port %d\n",local_port, remote_ip, remote_port);

	recv_list=fifo_init(NULL);
	pthread_mutex_init(&recv_list_lock, NULL);
	
	//start tracker
	void *tmp=&local_port;
	pthread_create( &tracker_thread, NULL, tracker, tmp);
	
	//connect to parent tracker

	//read and write messages to peers

	//redirect peers to other tracker and quit
	pthread_join(tracker_thread, NULL);
	
	return 0;
}
