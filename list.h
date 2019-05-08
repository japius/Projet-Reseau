#ifndef LIST_H
#define LIST_H
#define NBDATA 10

#include "struct.h"
#include "genlist.h"


struct ngb_entry{
	struct neighbor *sym;
	int times_sent;
	int wait_time;
};

//Un noeud de la liste d'inondations, soit un couple (ID,Nonce) et une liste de voisins à inonder
struct flood_entry{
	struct data_index *index;
	//A revoir, ptet autre chose que MAX_SIZE
	unsigned char data[(1<<8) +4];
	struct list *sym_neighbors;
};


struct ngb_entry *init_ngb_entry(struct neighbor *peer,int times_sent);

struct flood_entry *init_flood(struct data_index *,unsigned char *,struct list *);

short add_neighbor_to_flood(struct data_index *,struct neighbor *peer);

short remove_neighbor_from_flood(struct data_index *, struct neighbor *);

void add_message_to_flood(unsigned char *msg_send, u_int8_t size_msg_send, struct neighbor* to_delete);

short remove_neighbor_everywhere(struct neighbor *peer);

short compare_n_s(void *c1,void *c2);

short compare_n_s2(void *c1,void *c2);


void free_flood(struct flood_entry *flood);


#endif
