#ifndef SORTEDLIST_H
#define SORTEDLIST_H
#include "struct.h"

struct wait_node{
	struct neighbor_and_wait *nw;
	struct wait_node *next;
};

struct wait_list{
	struct wait_node *node;
	//int length;
};

int compare_w(struct neighbor_and_wait *nw, struct neighbor_and_wait *nw2);

struct neighbor_and_wait *init_nw(struct neighbor *, int wait_time,int *times_sent);

struct wait_list *init_wait_list(struct wait_node *node);

short add_element(struct wait_list *list,struct neighbor_and_wait *nw);


void clean_nw(struct neighbor_and_wait *nw);

void cleanl(struct wait_list *list);

short remove_element(struct wait_list *list, struct neighbor_and_wait *rm);

short move_element(struct wait_list *list,struct neighbor_and_wait *k,int wait_time,short increment);
#endif