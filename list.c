#include "list.h"
#include "struct.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "util.h"

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

struct list_entry *init_list_entry(struct neighbor *peer,int times_sent,struct list_entry *next){
	struct list_entry *res=malloc(sizeof(struct list_entry));
	res->sym=malloc(sizeof(struct neighbor));
	if(peer) memmove(res->sym,peer,sizeof(struct neighbor));
	res->times_sent=times_sent;
	res->next=next;
	return res;
}

struct flood_entry *init_flood_entry(struct data_index *index, char *data,struct list_entry *sym_neighbors,struct flood_entry *next){
	struct flood_entry *current=malloc(sizeof(struct flood_entry));
	current->index=malloc(sizeof(struct data_index));
	if(data_index) memmove(current->index,data_index,sizeof(struct data_index));
	current->sym_neighbors=entry;
	current->data=msg;
	current->next=next;
	return current;
}

struct flood *init_flood(struct flood_entry *first, struct flood_entry *last){
	struct flood *current=malloc(sizeof(struct flood));
	current->first=first;
	current->last=last;
	return current;
}



short add_neighbor_to_flood(struct flood_entry *flood,struct neighbor *peer){
	struct list_entry *entry=init_list_entry(peer,0,flood->sym_neighbors);
	flood->sym_neighbors=entry;
	return 1;
}

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

}

short remove_neighbor_from_flood(struct flood_entry *flood, struct neighbor *peer){
	if(flood){
		struct list_entry *entry=flood->sym_neighbors;
		if(compare_n(entry->sym,peer)==0){
			flood->sym_neighbors=entry->next;
			free(entry->sym);
			free(entry);
			return 1
		}
		flood->sym_neighbors=remove_node(entry,peer);
		return 1;
	}
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

struct flood_entry *get_flood(struct flood *flood,struct data_index *data){
	if(flood){
		return get_flood_aux(flood->first,data);
	}
	return NULL;
}

struct flood_entry *get_flood_aux(struct flood_entry *entry,struct data_index *data){
	if(!entry) return NULL;
	if(compare_d(data,entry->index)==0){
		return entry;
	}
	return get_flood_aux(entry->next,data);
}


int length(struct list_entry *list){
	if(list==NULL) return 0;
	return length(list->next);
}



void free_list(struct list_entry *entry){
	if(entry){
		free_list(entry->next);
		free(entry->sym);
		free(entry);
	}
}

void free_entry(struct flood_entry *entry){
	free(entry->index);
	free_list(entry->sym_neighbors);
	free(entry);
}

void free_flood_entry(struct flood_entry *entry){
	free(entry->index);
	free_list(entry->sym_neighbors);
	free_flood_entry(entry->next);
	free(entry);
}

void rm_entry_aux(struct flood_entry *curr,struct flood_entry *entry){
	if(curr->next){
		if(compare_d(curr->next->index,entry->index)==0){
			struct flood_entry *tmp=curr->next;
			curr->next=curr->next->next;
			free_entry(tmp);
		}
		else rm_entry_aux(curr->next,entry);
	}
	
}

void rm_entry(struct flood *flood,struct flood_entry *entry){
	if(flood){
		if(flood->first){
			if(compare_d(flood->first->index,entry->index)){
				struct flood_entry *tmp=flood->first;
				flood->first=flood->first->next;
				free_entry(tmp);
			}
			else rm_entry_aux(flood->first,entry);
		}
	}
}

void free_flood(struct flood *flood){
	free_flood_entry(flood->first);
	free_flood_entry(flood->last);
	free(flood);
}


struct flood *init_data(){
	return init_flood(NULL,NULL);
}
