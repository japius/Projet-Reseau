#ifndef LIST_H
#define LIST_H

#include "struct.h"

struct list_entry{
	struct neighbor *sym;
	int times_sent;
	int wait_time;
};

//Un noeud de la liste d'inondations, soit un couple (ID,Nonce) et une liste de voisins à inonder
struct flood_entry{
	struct data_index *index;
	//A revoir, ptet autre chose que MAX_SIZE
	char *data;
	struct list *sym_neighbors;
};

//La liste d'inondations 
struct flood{
	struct list *first;
	struct list *last;

};

struct list_entry *init_list_entry(struct neighbor *peer,int times_sent);

struct flood_entry *init_flood_entry(struct data_index *,char *,struct list *);

struct flood *init_flood(struct list *,struct list *);

short add_neighbor_to_flood(struct list *flood,struct neighbor *peer);

short remove_neighbor_from_flood(struct flood_entry *flood, struct neighbor *peer);

short add_entry(struct flood *flood, struct data_index *data,char *msg,struct list_entry *entry);

short rm_entry(struct flood *flood,struct flood_entry *entry);

struct flood_entry *get_flood(struct flood *,struct data_index *data);


struct list_entry *add_node(struct list_entry *,struct list_entry *);

struct list_entry *get_last(struct list_entry *);

struct list_entry *remove_node(struct list_entry *,struct neighbor *);



int length(struct list_entry *);

void free_flood(struct flood *flood);

void free_list(struct list_entry *entry);

void free_entry(struct flood_entry *entry);

#endif
