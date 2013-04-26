#include "main.h"

void *input()
{
        int data_lenght;
        char buffer[BUFFER_SIZE];
        struct sender_data *send;

        while(1) {
                data_lenght=read(0, buffer, BUFFER_SIZE-2);
                send=malloc(sizeof(struct sender_data));
                send->socket=-1;
                strncpy(send->data, buffer, BUFFER_SIZE-1);
                send->data_lenght=data_lenght;
                child_tracker_tx(send);
                bzero(buffer, BUFFER_SIZE);
        }
        return NULL;
}

int main(int argc, char **argv)
{
        pthread_t tracker_thread;
        pthread_t input_thread;
        int local_port;
        int remote_port;
        char remote_ip[1024];

        //parse args
        while((optopt=getopt(argc, argv, "p:r:"))!=-1) {
                if(optopt=='p') sscanf(optarg, "%d", &local_port);
                if(optopt=='r')	sscanf(optarg, "%[^:]:%d", remote_ip, &remote_port);
        }

        printf("Bind on port %d\nConnect to %s port %d\n",local_port, remote_ip, remote_port);

        recv_list=fifo_init(NULL);
        pthread_mutex_init(&recv_list_lock, NULL);

        //start tracker
        void *tmp=&local_port;
        pthread_create( &tracker_thread, NULL, tracker, tmp);

        //read and write messages to peers
        pthread_create( &input_thread, NULL, input, NULL);

        //connect to parent tracker
        client(remote_ip, remote_port);

        //redirect peers to other tracker and quit
        pthread_join(tracker_thread, NULL);
        pthread_join(input_thread, NULL);

        return 0;
}
