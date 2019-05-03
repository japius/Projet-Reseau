#include "list.h"
#include "struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

//Initialiser la liste de voisins à inonder
struct  ngb_entry *init_ngb_entry(struct neighbor *peer,int times_sent){
	struct ngb_entry *res=malloc(sizeof(struct ngb_entry));
	if(res==NULL){
		perror("malloc");
		return NULL;
	}
	res->sym=peer;
	res->times_sent=times_sent;
	return res;
}

//Initialiser la donnée à inonder et sa liste de voisins à inonder
struct flood_entry *init_flood_entry(struct data_index *index, char *data,struct list *sym_neighbors){
	struct flood_entry *current=malloc(sizeof(struct flood_entry));
	if(current==NULL){
		perror("malloc");
		return NULL;
	}
	current->index=malloc(sizeof(struct data_index));
	if(current->index==NULL){
		free(current);
		perror("malloc");
		return NULL;
	}
	if(index) memmove(current->index,index,sizeof(struct data_index));
	current->sym_neighbors=sym_neighbors;
	current->data=data;
	return current;
}


//Ajouter un voisin dans une liste donnnée de personnes à inonder
short add_neighbor_to_flood(struct data_index *index,struct neighbor *peer){
	struct flood_entry *flood=get_flood(index);
	if(flood==NULL){
		return 0;
	}
	struct ngb_entry l;
	l.sym=peer;
	l.times_sent=0;
	struct list *entry=init_list(&l,sizeof(struct ngb_entry),flood->sym_neighbors);
	if(entry==NULL) return 0;
	flood->sym_neighbors=entry;
	return 1;
}

int compare_n_s(void *c1,void *c2){
	struct ngb_entry *n1=(struct ngb_entry *)c1;
	struct ngb_entry *n2=(struct ngb_entry *)c2;
	return compare_n(n1->sym,n2->sym);
}

//Retirer un voisin d'une liste de voisins à inonder
short remove_neighbor_from_flood(struct data_index *data,struct neighbor *peer){
	struct flood_entry *flood=get_flood(data);
	if(flood==NULL) return 1;
	struct list *entry=flood->sym_neighbors;
	return remove_element(&entry,peer,compare_n_s);
}

struct flood *init_data(){
	return init_flood(NULL);
}
