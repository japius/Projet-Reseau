#ifndef UTIL_H
#define UTIL_H

#include "struct.h"
#include "peer.h"


int compare_n(struct neighbor *key1,struct neighbor *key2);

//MULTICAST LOCAL IPV6
void discover_neighbors();


#endif