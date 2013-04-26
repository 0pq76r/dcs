#include "fifo.h"

struct _fifo *fifo_init(void *data)
{
	struct _fifo *fifo;
	fifo=malloc(sizeof(struct _fifo));

	fifo->next=NULL;
	fifo->last=fifo;
	fifo->data=data;
	
	return fifo;
};

struct _fifo *fifo_add(struct _fifo *fifo, void *data)
{
	if(fifo==NULL){
		fifo=fifo_init(data);
		return fifo;
	}
	struct _fifo *new_fifo;
	new_fifo=fifo_init(data);

	fifo->last->next=new_fifo;
	fifo->last=new_fifo;
	return fifo;
}

struct _fifo *fifo_remove(struct _fifo *fifo)
{
	void *ret;
	
	if(fifo==NULL){
		return NULL;
	}

	if(fifo->next==NULL){
		return NULL;
	}

	fifo->next->last=fifo->last;
	ret=fifo->next;
	free(fifo);
	
	return ret;
}
