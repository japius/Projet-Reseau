#ifndef GENLIST_H
#define GENLIST_H
#include <unistd.h>

struct list{
	void *content;
	struct list *next;
};


struct list *init_list(void *content, size_t content_size,struct list *next);

void free_node(struct list *node);

void free_list(struct list *list);

struct list *add_element_aux(struct list *node,void *content,size_t content_size,int (*compare)(void *content1,void *content2));

short add_element(struct list **list,void *content,size_t content_size,int (*compare)(void *content1,void *content2));

void *get_element(struct list *list,void *content,int (*compare)(void *content1,void *content2));

//short remove_element_aux(struct list *node,void *content,int (*compare)(void *content1,void *content2));


short remove_element(struct list **list, void *content,int (*compare)(void *content1,void *content2));

struct list *get_last(struct list *node);

int length(struct list *list);
#endif