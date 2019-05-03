#include "list.h"
#include "genlist.h"
#include "struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

//Initialiser la liste de voisins à inonder
struct list_entry *init_list_entry(struct neighbor *peer,int times_sent){
	struct list_entry *res=malloc(sizeof(struct list));
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

//Initialiser l'ensemble des données à inonder
struct flood *init_flood(struct list *first){
	struct flood *current=malloc(sizeof(struct flood));
	if(current==NULL){
		perror("malloc");
		return NULL;
	}
	current->first=first;
	if(first){
		struct list *tmp=first;
		while(tmp->next){
			tmp=tmp->next;
		}
		current->last=tmp;	
		current->last->next=first;
	}
	return current;
}


//Pour free une donnée à inonder et sa liste de voisins
void free_entry(struct flood_entry *entry){
	free(entry->index);
	free_list(entry->sym_neighbors);
	free(entry);
}

//Pour free l'ensemble de données
void free_flood(struct flood *flood){
	free_list(flood->first);
	flood->last=NULL;
	//free_flood_entry(flood->last);
	free(flood);
}


//Ajouter un voisin dans une liste donnnée de personnes à inonder
short add_neighbor_to_flood(struct data_index *index,struct neighbor *peer){
	struct flood_entry *flood=get_flood(index);
	if(flood==NULL){
		return 0;
	}
	struct list_entry l;
	l.sym=peer;
	l.times_sent=0;
	struct list *entry=init_list(&l,sizeof(struct list_entry),flood->sym_neighbors);
	if(entry==NULL) return 0;
	flood->sym_neighbors=entry;
	return 1;
}

//Ajouter une donnée et sa liste de personnes à inonder (FIFO)
short add_entry(struct data_index *data,char *msg,struct list *entry){
	if(entry){
		if(DATAF==NULL){
			struct flood_entry *f=init_flood_entry(data,msg,entry);
			if(f==NULL) return 0;
			DATAF=init_flood(init_list(f,sizeof(struct flood_entry),NULL));
			return (DATAF!=NULL);
		}
		struct flood_entry f;
		f.index=data;
		f.data=msg;
		f.sym_neighbors=entry;
		DATAF->last->next=init_list(&f,sizeof(struct flood_entry),DATAF->first);
		if(DATAF->last->next==NULL) return 0;
		DATAF->last=DATAF->last->next;
		//DATAF->last->next=DATAF->first;
		return 1;
	}
	return 1;

}

int compare_n_s(void *c1,void *c2){
	struct list_entry *n1=(struct list_entry *)c1;
	struct list_entry *n2=(struct list_entry *)c2;
	return compare_n(n1->sym,n2->sym);
}
//Retirer un voisin d'une liste de voisins à inonder
short remove_neighbor_from_flood(struct data_index *data,struct neighbor *peer){
	struct flood_entry *flood=get_flood(data);
	if(flood==NULL) return 1;
	struct list *entry=flood->sym_neighbors;
	return remove_element(&entry,peer,compare_n_s);
}


//Fonction auxialiare pour supprimer une donnée à inonder et sa liste de voisins
short rm_entry_aux(struct  list *curr,struct data_index *index){
	if(curr->next){
		struct flood_entry *f=(struct flood_entry *)curr->next->content;
		if(compare_d(f->index,index)==0){
			struct list *tmp=curr->next;
			curr->next=curr->next->next;
			free_entry(f);
			free(tmp);
			return 1;
		}
		return rm_entry_aux(curr->next,index);
	}
	return 1;
}

//Supprimer une donnée à inonder et sa liste de voisins de l'ensemble des données
short rm_entry(struct data_index *key){
	if(DATAF){
		if(DATAF->first){
			struct flood_entry *f=(struct flood_entry *)DATAF->first->content;
			if(compare_d(f->index,key)==0){
				struct list *tmp=DATAF->first;
				DATAF->first=DATAF->first->next;
				free_entry(f);
				free(tmp);
				return 1;
			}
			return rm_entry_aux(DATAF->first,key);
		}
	}
	return 1;
}

//obtenir la donnée à inonder et sa liste de voisins à partir de (Id,Nonce)
struct flood_entry *get_flood(struct data_index *data){
	return (struct flood_entry *)get_element(DATAF->first,data,compare_n_s);
}


struct flood *init_data(){
	return init_flood(NULL);
}
