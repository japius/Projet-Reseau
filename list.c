#include "list.h"
#include "struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"
#include "genlist.h"

//Initialiser la liste de voisins à inonder
struct  ngb_entry *init_ngb_entry(struct neighbor *peer,int times_sent){
	struct ngb_entry *res=malloc(sizeof(struct ngb_entry));
	if(res==NULL){
		perror("malloc");
		return NULL;
	}
	res->sym=peer;
	res->times_sent=times_sent;
	res->wait_time=wait_time(times_sent);
	return res;
}

//Initialiser la donnée à inonder et sa liste de voisins à inonder
struct flood_entry *init_flood(struct data_index *index, char *data,struct list *sym_neighbors){
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
	memcpy(current->data,data,data[1]+2);
	return current;
}

int count;
void free_flood(struct flood_entry *flood){
	free(flood->index);
	printf("//////// appelle %d //////////////////////////////////\n",++count);
	free_list(flood->sym_neighbors,free);
	free(flood->sym_neighbors);
	free(flood);
}


//Ajouter un voisin dans une liste donnnée de personnes à inonder
short add_neighbor_to_flood(struct data_index *index,struct neighbor *peer){
	struct flood_entry *flood=(struct flood_entry *)get(&DATAF,index);
	if(flood==NULL){
		return 0;
	}
	struct ngb_entry *l=init_ngb_entry(peer,0);
	/*struct ngb_entry l;
	l.sym=peer;
	l.times_sent=0;
	//ici tirer un temps aléatoire*/
	return addLast(flood->sym_neighbors,&l);
}


short compare_n_s(void *c1,void *c2){
	struct ngb_entry *n1=(struct ngb_entry *)c1;
	struct ngb_entry *n2=(struct ngb_entry *)c2;
	if(compare_n(n1->sym,n2->sym)==0) return 0;
	int i=n1->wait_time-n2->wait_time;
	if(i==0) return 1;
	return i;
	//return compare_n(n1->sym,n2->sym);
}

//Retirer un voisin d'une liste de voisins à inonder
short remove_neighbor_from_flood(struct data_index *data,struct neighbor *peer){
	struct flood_entry *flood=(struct flood_entry *)get(&DATAF,index);
	if(flood==NULL){
		return 0;
	}
	struct ngb_entry l;
	l.sym=peer;
	l.times_sent=0;
	void *tmp=remove_elem(flood->sym_neighbors,&l);	
	if(tmp) free(tmp);
	if(flood->sym_neighbors->length==0){
		void *tmp=remove_elem(&DATAF,&flood);
		if(tmp) free_flood(&flood);
	}
	//je suis pas sure de s'il faut free, vu que c'est la même adresse partout pour le voisin;
	return 1;
}

short remove_neighbor_everywhere(struct neighbor *peer){
	for(struct list_entry *list=DATAF.first;list;list=list->next){
		struct flood_entry *f=(struct flood_entry *)list->content;
		remove_neighbor_from_flood(f->data,peer);
	}
	remove_neighbor(peer);
	return 1;
}
