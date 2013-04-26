#ifndef __FIFO_H__
#define __FIFO_H__

#include "socket.h"
#include "main.h"

struct _fifo
{
	void *data;
	struct _fifo *next;
	struct _fifo *last;
};


struct _fifo *fifo_init(void *data);
struct _fifo *fifo_add(struct _fifo *fifo, void *data);
struct _fifo *fifo_remove(struct _fifo *fifo);

#endif
