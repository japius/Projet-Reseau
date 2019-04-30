#ifndef UTIL_H
#define UTIL_H

#include "struct.h"
#include "peer.h"


int compare_n(struct neighbor *key1,struct neighbor *key2);

int compare_d(struct data_index *data,struct data_index *data2);

//MULTICAST LOCAL IPV6
void discover_neighbors();

int get_seconds();

int max(int x,int y);

#endif