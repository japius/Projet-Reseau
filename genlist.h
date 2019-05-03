#ifndef GENLIST_H
#define GENLIST_H
#include <unistd.h>


typedef struct list_entry{
	void *content;
	struct list_entry *next;
} list_entry;

struct list{
	size_t length;
	struct list_entry *first;
	struct list_entry *last;
	short (*compare_f)(void *,void*);
	size_t content_len;
};

typedef struct list* list;

void init_list(list this,short (*comp)(void *, void*), size_t cont_len);
short addLast(list this,void *elem);
short add_elem(list this, void *elem);
void *remove_first(list this);
void *remove_elem(list this, void *elem);
void *get(list this, void *elem);
void init_compare(list this,short (*comp)(void *, void *));
void free_list(list this,void *(free_f)(void *content));


#endif