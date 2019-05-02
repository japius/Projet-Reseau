#include "struct.h"
#include "sortedlist.h"
#include "util.h"
#include <string.h>

int compare_w(struct neighbor_and_wait *nw, struct neighbor_and_wait *nw2){
	return nw->wait_time-nw2->wait_time;
}

struct neighbor_and_wait *init_nw(struct neighbor *n,int wait_time,int *times_sent){
	struct neighbor_and_wait *nw=malloc(sizeof(struct neighbor_and_wait));
	if(nw==NULL){
		perror("malloc");
		return NULL;
	}
	nw->neighbor=malloc(sizeof(struct neighbor));
	if(nw->neighbor==NULL){
		perror("malloc");
		return NULL;
	}
	if(n) memmove(nw->neighbor,n,sizeof(struct neighbor));
	nw->wait_time=wait_time;
	return nw;
	nw->times_sent=times_sent;
}

struct wait_node *init_wait_node(struct neighbor_and_wait *nw,struct wait_node *next){
	struct wait_node *list=malloc(sizeof(struct wait_list));
	if(list==NULL){
		perror("malloc");
		return NULL;
	}
	if(nw) list->nw=init_nw(nw->neighbor,nw->wait_time,nw->times_sent);
	else list->nw=malloc(sizeof(struct neighbor_and_wait));
	if(list->nw==NULL){
		perror("malloc");
		return NULL;
	}
	list->next=next;
	return list;
}

struct wait_list *init_wait_list(struct wait_node *node){
	struct wait_list *list=malloc(sizeof(struct wait_list));
	if(list==NULL){
		perror("malloc");
		return NULL;
	}
	//list->length=length;;
	list->node=node;
	return list;
}

struct wait_node *add_node_element(struct wait_node *list,struct neighbor_and_wait *nw){
	if(list==NULL) return init_wait_node(nw,NULL);
	if(compare_w(list->nw,nw)>=0){
		return init_wait_node(nw,list);
	}
	list->next=add_node_element(list->next,nw);
	return list;
}

short add_element(struct wait_list *list,struct neighbor_and_wait *nw){
	if(list->node==NULL){
		list->node=init_wait_node(nw,NULL);
		return (list->node!=NULL);
	}
	list->node=add_node_element(list->node,nw);
	return (list->node!=NULL);
}

void clean_nw(struct neighbor_and_wait *nw){
	free(nw->neighbor);
	free(nw);
}

void clean_node(struct wait_node *list){
	if(list){
		clean_node(list->next);
		clean_nw(list->nw);
		free(list);
	}
}

void cleanl(struct wait_list *list){
	if(list){
		clean_node(list->node);
		free(list);
	}
	
}

struct wait_node *remove_node_element(struct wait_node *list, struct neighbor_and_wait *rm){
		if(list==NULL) return NULL;
		if(compare_n(list->nw->neighbor,rm->neighbor)==0){
			struct wait_node *res=list->next;
			clean_nw(list->nw);
			free(list);
			return res;
		}
		list->next=remove_node_element(list->next,rm);
		return list;
}

short move_element(struct wait_list *list,struct neighbor_and_wait *k,int wait_time,short increment){
	int *times_sent=k->times_sent;
	if(increment)*times_sent=*times_sent+1;
	struct neighbor *n=k->neighbor;
	int i=remove_element(list,k);
	if(i==0) return 0;
	i=add_element(list,init_nw(n,wait_time,times_sent));
	if(i==0) return 0;
	return 1;
}

short remove_element(struct wait_list *list,struct neighbor_and_wait *rm){
	if(list==NULL) return 1;
	list->node=remove_node_element(list->node,rm);
	return (list->node!=NULL);
}

