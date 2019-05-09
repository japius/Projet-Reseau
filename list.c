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
	//res->times_sent=times_sent;
	res->times_sent=0;
	res->wait_time=wait_time(times_sent)+get_seconds();
	return res;
}

//Initialiser la donnée à inonder et sa liste de voisins à inonder
struct flood_entry *init_flood(struct data_index *index,unsigned char *data,struct list *sym_neighbors){
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
	if(data[1]+2>PMTU-4){
		free(current->index);
		free(current);
		return 0;
	}
	memcpy(current->data,data,data[1]+2);
	return current;
}

void free_flood(struct flood_entry *flood){
	free(flood->index);
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
	return addLast(flood->sym_neighbors,l);
}

void add_message_to_flood(unsigned char *msg_send, u_int8_t size_msg_send,struct neighbor *to_delete){
	struct data_index index;
	memcpy(&index.id,msg_send,8);
	memcpy(&index.nonce,msg_send+8,4);
	list symmetric=get_symmetrical(NEIGHBORS);
	if(!symmetric) return;
	if(to_delete!=NULL){
		struct ngb_entry ngb_ent ={0};
		ngb_ent.sym=to_delete;
		void *tmp=remove_elem(symmetric,&ngb_ent);
		if(tmp!=NULL) free(tmp);
	}
	//printf("un petit test : %d",ntohs(peer.port));
	//symmetric=remove_node(symmetric,&peer);
	//on reconstruit le message et on le met dans la struct pour l"envoyer plus tard
	//rajouter caractère de fin de ligne ? +1 pour type 4
	unsigned char msg[(1<<8) + 4];
	msg[0]=4;
	msg[1]=size_msg_send;
	memcpy(msg+2,msg_send,size_msg_send);
	struct flood_entry *flood = init_flood(&index,msg,symmetric);
	void *must_free = add_limited(&DATAF,flood,NBDATA);
	//print_list(&DATAF);
	if(!must_free){
		return 0;
	}
	free_flood(must_free);
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


short compare_n_s2(void *c1,void *c2){
	struct ngb_entry *n1=(struct ngb_entry *)c1;
	struct ngb_entry *n2=(struct ngb_entry *)c2;
	return compare_n(n1->sym,n2->sym)==0;
	//return compare_n(n1->sym,n2->sym);
}

//Retirer un voisin d'une liste de voisins à inonder
short remove_neighbor_from_flood(struct data_index *data,struct neighbor *peer){
	struct flood_entry fl = {0};
	fl.index=data;
	struct flood_entry *flood=(struct flood_entry *)get(&DATAF,&fl);
	if(flood==NULL){
		return 0;
	}
	struct ngb_entry l;
	l.sym=peer;
	l.times_sent=0;
	//init_compare(flood->sym,compare_n_s2);
	void *tmp=remove_elem(flood->sym_neighbors,&l);	
	if(tmp) free(tmp);
	//init_compare(flood->sym,compare_n_s);
	/*if(flood->sym_neighbors->length==0){
		void *tmp=remove_elem(&DATAF,&flood);
		if(tmp) free_flood(&flood);
	}*/
	//je suis pas sure de s'il faut free, vu que c'est la même adresse partout pour le voisin;
	return 1;
}

short remove_neighbor_everywhere(struct neighbor *peer){
	for(struct list_entry *list=DATAF.first;list;list=list->next){
		struct flood_entry *f=(struct flood_entry *)list->content;
		remove_neighbor_from_flood(f->index,peer);
	}
	remove_neighbor(peer);
	return 1;
}
