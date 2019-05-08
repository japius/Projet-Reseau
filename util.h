#ifndef UTIL_H
#define UTIL_H

#include "struct.h"
#include "peer.h"

void flood_messages(int soc,list flood);

short compare_n(void *,void *);

short compare_flood(void *, void *);

short compare_d(void *,void *);
//struct data_index *data,struct data_index *data2);

short compare_w(void *, void *);

void print_on_screen(char *data, size_t data_size);

//MULTICAST LOCAL IPV6
void discover_neighbors();

int get_seconds();

int wait_time(int times_sent);

int max(int x,int y);

int min(int x,int y);

#endif