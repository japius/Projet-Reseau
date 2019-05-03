#ifndef LIST_H
#define LIST_H
#define NBDATA 10

#include "struct.h"


struct ngb_entry{
	struct neighbor *sym;
	int times_sent;
	//int wait_time;
};

//Un noeud de la liste d'inondations, soit un couple (ID,Nonce) et une liste de voisins à inonder
struct flood_entry{
	struct data_index *index;
	//A revoir, ptet autre chose que MAX_SIZE
	char *data;
	struct list *sym_neighbors;
};


struct list_entry *init_list_entry(struct neighbor *peer,int times_sent);

struct flood_entry *init_flood_entry(struct data_index *,char *,struct list *);

struct flood *init_flood(struct list *);

short add_neighbor_to_flood(struct data_index *,struct neighbor *peer);

short remove_neighbor_from_flood(struct data_index *, struct neighbor *);

short add_entry(struct data_index *data,char *msg,struct list *entry);

short rm_entry(struct data_index *);

int compare_n_s(void *c1,void *c2);
struct flood_entry *get_flood(struct data_index *data);


void free_flood(struct flood *flood);

void free_entry(struct flood_entry *entry);

#endif
