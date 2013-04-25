#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>

#include "client.h"
#include "tracker.h"
#include "fifo.h"

struct _fifo *recv_list;
pthread_mutex_t recv_list_lock;

#endif

