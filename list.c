#include "list.h"
#include "struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

//Initialiser la liste de voisins à inonder
struct list_entry *init_list_entry(struct neighbor *peer,int times_sent,struct list_entry *next){
	struct list_entry *res=malloc(sizeof(struct list_entry));
	res->sym=malloc(sizeof(struct neighbor));
	if(peer) memmove(res->sym,peer,sizeof(struct neighbor));
	res->times_sent=times_sent;
	res->next=next;
	return res;
}

//Initialiser la donnée à inonder et sa liste de voisins à inonder
struct flood_entry *init_flood_entry(struct data_index *index, char *data,struct list_entry *sym_neighbors,struct flood_entry *next){
	struct flood_entry *current=malloc(sizeof(struct flood_entry));
	current->index=malloc(sizeof(struct data_index));
	if(index) memmove(current->index,index,sizeof(struct data_index));
	current->sym_neighbors=sym_neighbors;
	current->data=data;
	current->next=next;
	return current;
}

//Initialiser l'ensemble des données à inonder
struct flood *init_flood(struct flood_entry *first, struct flood_entry *last){
	struct flood *current=malloc(sizeof(struct flood));
	current->first=first;
	current->last=last;
	return current;
}


//Ajouter un voisin dans une liste donnnée de personnes à inonder
short add_neighbor_to_flood(struct flood_entry *flood,struct neighbor *peer){
	struct list_entry *entry=init_list_entry(peer,0,flood->sym_neighbors);
	flood->sym_neighbors=entry;
	return 1;
}

//Ajouter une donnée et sa liste de personnes à inonder
short add_entry(struct flood *flood,struct data_index *data,char *msg,struct list_entry *entry){
	if(entry){
		if(flood==NULL){
			struct flood_entry *f=init_flood_entry(data,msg,entry,NULL);
			flood=init_flood(f,f);
			return 1;
		}
		struct flood_entry *f=init_flood_entry(data,msg,entry,flood->first);
		flood->first=f;
		return 1;
	}
	return 1;

}

//Retirer un voisin d'une liste de voisins à inonder
short remove_neighbor_from_flood(struct flood_entry *flood, struct neighbor *peer){
	if(flood){
		struct list_entry *entry=flood->sym_neighbors;
		if(compare_n(entry->sym,peer)==0){
			flood->sym_neighbors=entry->next;
			free(entry->sym);
			free(entry);
			return 1;
		}
		flood->sym_neighbors=remove_node(entry,peer);
		return 1;
	}
	return 1;
}
//Fonction auxialiare pour supprimer une donnée à inonder et sa liste de voisins
short rm_entry_aux(struct flood_entry *curr,struct flood_entry *entry){
	if(curr->next){
		if(compare_d(curr->next->index,entry->index)){
			struct flood_entry *tmp=curr->next;
			curr->next=curr->next->next;
			free_entry(tmp);
			return 1;
		}
		return rm_entry_aux(curr->next,entry);
	}
	return 1;
}

//Supprimer une donnée à inonder et sa liste de voisins de l'ensemble des données
short rm_entry(struct flood *flood,struct flood_entry *entry){
	if(flood){
		if(flood->first){
			if(compare_d(flood->first->index,entry->index)){
				struct flood_entry *tmp=flood->first;
				flood->first=flood->first->next;
				free_entry(tmp);
				return 1;
			}
			return rm_entry_aux(flood->first,entry);
		}
	}
	return 1;
}

struct flood_entry *get_flood_aux(struct flood_entry *entry,struct data_index *data){
	if(!entry) return NULL;
	if(compare_d(data,entry->index)==0){
		return entry;
	}
	return get_flood_aux(entry->next,data);
}

//obtenir la donnée à inonder et sa liste de voisins à partir de (Id,Nonce)
struct flood_entry *get_flood(struct flood *flood,struct data_index *data){
	if(flood){
		return get_flood_aux(flood->first,data);
	}
	return NULL;
}





struct list_entry *remove_node(struct list_entry *entry,struct neighbor *peer){
	//if(compare_n(entry->list))
	if(compare_n(entry->sym,peer)==0){
		struct list_entry *tmp=entry,*res=entry->next;
		free(tmp->sym);
		free(tmp);
		return res;
	}
	entry->next=remove_node(entry->next,peer);
	return entry;
}

struct list_entry *add_node(struct list_entry *entry,struct list_entry *node){
	if(node){
		node->next=entry;
		return node;
	}
	return entry;	
}

struct list_entry *get_last(struct list_entry *node){
	if(node){
		if(node->next) return get_last(node->next);
		return node;
	}
	return NULL;
}


int length(struct list_entry *list){
	if(list==NULL) return 0;
	return length(list->next);
}



//Pour free tous les éléments d'une liste de voisins à inonder
void free_list(struct list_entry *entry){
	if(entry){
		free_list(entry->next);
		free(entry->sym);
		free(entry);
	}
}

//Pour free une donnée à inonder et sa liste de voisins
void free_entry(struct flood_entry *entry){
	free(entry->index);
	free_list(entry->sym_neighbors);
	free(entry);
}

//Pour free toutes les données à inonder d'un ensemble
void free_flood_entry(struct flood_entry *entry){
	free(entry->index);
	free_list(entry->sym_neighbors);
	free_flood_entry(entry->next);
	free(entry);
}

//Pour free l'ensemble de données
void free_flood(struct flood *flood){
	free_flood_entry(flood->first);
	flood->last=NULL;
	//free_flood_entry(flood->last);
	free(flood);
}


struct flood *init_data(){
	return init_flood(NULL,NULL);
}
