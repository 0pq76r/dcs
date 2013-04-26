#ifndef __TRACKER_H__
#define __TRACKER_H__

#include "main.h"
#include "socket.h"

extern void *tracker(void* port);
extern void *child_tracker_tx(void *send);

#define BUFFER_SIZE 1024

struct sender_data
{
	int socket;
	char data[BUFFER_SIZE];
	int data_lenght;
};


#endif

