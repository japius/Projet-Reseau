#ifndef LIST_H
#define LIST_H

#include "struct.h"

struct list_entry{
	struct neighbor *sym;
	int times_sent;
	struct list_entry *next;
};

//Un noeud de la liste d'inondations, soit un couple (ID,Nonce) et une liste de voisins à inonder
struct flood_entry{
	struct data_index *index;
	//A revoir, ptet autre chose que MAX_SIZE
	char *data;
	struct list_entry *sym_neighbors;
	struct flood_entry *next;
};

//La liste d'inondations 
struct flood{
	struct flood_entry *first;
	struct flood_entry *last;
};

struct list_entry *init_list_entry(struct neighbor *peer,int times_sent,struct list_entry *next);

struct flood_entry *init_flood_entry(struct data_index *,char *,struct list_entry *,struct flood_entry *);

struct flood *init_flood(struct flood_entry *,struct flood_entry *);

short add_neighbor_to_flood(struct flood_entry *flood,struct neighbor *peer);

short remove_neighbor_from_flood(struct flood_entry *flood, struct neighbor *peer);

struct list_entry *add_node(struct list_entry *,struct list_entry *);

struct list_entry *remove_node(struct list_entry *,struct neighbor *);



struct flood *init_data();

struct flood_entry *get_flood(struct flood *,struct data_index *data);

int length(struct list_entry *);

void add_entry(struct flood *flood, struct data_index *data,char *msg,struct list_entry *entry);

void rm_entry(struct flood *flood,struct flood_entry *entry);

void free_flood(struct flood *flood);

void free_list(struct list_entry *entry);

#endif
