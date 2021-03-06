#include <stdlib.h>
#include <stdio.h>
#include "genlist.h"

void init_list(list this,short (*comp)(void *, void*), size_t cont_len){
	this->length = 0;
	this->first=0;
	this->last=0;
	this->compare_f = comp;
	this->content_len = cont_len;
}

static list_entry *init_entry(list_entry *prev, list_entry *next, void *elem){
	list_entry *res = calloc(sizeof(list_entry),1);
	if(!res) return 0;
	if(prev!=NULL){
		prev->next=res;
	}
	res->next=next;
	res->content=elem;
	return res;
}

short addLast(list this,void *elem){
	if(!this->last){
		this->first =init_entry(0,0,elem);
		if(!this->first) return 0;
		this->last=this->first;
		this->length++;
		return 1;
	}
	list_entry *tmp = init_entry(this->last,0,elem); 
	if(!tmp) return 0;
	this->last=tmp;
	this->length++;
	return 1;
}

short add_elem(list this, void *elem){
	if(!this->last){
		this->first =init_entry(0,0,elem);
		if(!this->first) return 0;
		this->last=this->first;
		this->length++;
		return 1;
	}
	short dif = this->compare_f(this->first->content,elem);
	if(dif>0){
		list_entry *tmp = init_entry(0,this->first,elem); 
		if(!tmp) return 0;
		this->first = tmp;
		this->length++;
		return 1;
	}
	if(dif==0){
		return 2;
	}

	for(list_entry *ent=this->first;ent->next;ent=ent->next){
		dif=this->compare_f(ent->next->content,elem);
		if(dif>0){
			list_entry *tmp = init_entry(ent,ent->next,elem); 
			if(!tmp) return 0;
			this->length++;
			return 1;
		}
		if(dif==0){
			return 2;
		}
	}

	return addLast(this, elem);
}

void *remove_first(list this){
	if(!this->first)
		return 0;
	if(this->first==this->last)
		this->last=NULL;
	void *cont=this->first->content;
	list_entry *tmp = this->first;
	this->first=this->first->next;
	free(tmp);
	this->length--;
	return cont;
}

void *remove_elem(list this, void *elem){
	if(!this->first) return 0;
	if(this->compare_f(this->first->content,elem)==0){
		return remove_first(this);
	}
	for(list_entry *ent=this->first;ent->next;ent=ent->next){
		if(this->compare_f(ent->next->content,elem)==0){
			list_entry *next = ent->next->next;
			void * cont = ent->next->content;
			free(ent->next);
			ent->next=next;
			if(next==NULL){
				this->last=ent;
				this->length--;
			}
			return cont;
		}
	}
	return 0;
}

void *add_limited(list this, void *elem, int max_t){
	addLast(this,elem);
	if(this->length>max_t)
		return remove_first(this);
	return 0;
}


void *get(struct list *this, void *elem){
	for(list_entry *ent=this->first;ent;ent=ent->next){
		if(this->compare_f(ent->content,elem)==0) return ent->content;
	}
	return 0;
}

void init_compare(list this,short (*comp)(void *, void *)){
	this->compare_f = comp;
}

void free_list(list this, void (*free_f)(void *content)){
	for(void *tmp=remove_first(this);tmp;tmp=remove_first(this)){
		if(free_f!=NULL)
			free_f(tmp);
	}
}


void print_list(list this){
	printf("La longueur de la liste : %lu\n",this->length);
	for(list_entry *l=this->first;l;l=l->next){
		printf("Element de liste : %p\n",l);
		printf("Content : %p\n",l->content);
	}
}


