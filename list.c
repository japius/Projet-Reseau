#include "list.h"
#include "struct.h"
#include <stdlib.h>
#include <stdio.h>

int compare_d(struct data_index *data,struct data_index *data2){
	return !(data->id==data2->id && data->nonce==data2->nonce);
}
struct list_entry *add_node(struct list_entry *entry,struct list_entry *node){
	if(node){
		node->next=entry;
		return node;
	}
	return entry;	
}
struct list_entry *rm_node(struct list_entry *entry,struct neighbor *peer){
	//if(compare_n(entry->list))
	if(compare(entry->sym,peer)==0){
		struct list_entry *tmp=entry,*res=entry->next;
		free(tmp->sym);
		free(tmp);
		return res;
	}
	entry->next=rm_node(entry->next,peer);
	return entry;
}

struct flood_entry *get_flood_aux(struct flood_entry *entry,struct data_index *data){
	if(!entry) return NULL;
	if(compare_d(data,entry->index)==0){
		return entry;
	}
	return get_flood_aux(entry->next,data);
}

struct flood_entry *get_flood(struct flood *flood,struct data_index *data){
	if(flood){
		return(flood->first,data);
	}
	return NULL;
}

void free_list(struct list_entry *entry){
	free(entry->sym);
	free_list(entry->next);
	free(entry);
}

struct flood_entry *init_fe(struct data_index *data_index,struct list_entry *entry,struct flood_entry *next){
	struct flood_entry *current=malloc(sizeof(struct flood_entry));
	current->index==data_index;
	current->sym_neighbors=entry;
	current->next=next;
	return current;
}

struct flood *init_flood(struct flood_entry *first,struct flood_entry *last){
	struct flood *current=malloc(sizeof(struct flood));
	current->first=first;
	current->last=last;
	return current;
}

/*struct flood *init_flood(struct flood_entry *first){
	current->first=first;
	//current->last=last;
	return current;
}*/



struct flood *init_data(){
	return init_flood(NULL,NULL);
}

void add_entry(struct flood *flood, struct data_index *data,struct list_entry *entry){
	if(entry){
		struct flood *f=init_fe(data,entry,flood->first);
		flood->first=f;
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


