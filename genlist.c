#include "genlist.h";

struct list *init_list(void *content, size_t content_size,struct list *next){
	struct list *list=malloc(sizeof(struct list));
	if(list==NULL){
		perror("malloc");
		return NULL;
	}
	list->content=malloc(content_size);
	if(list->content==NULL){
		perror("malloc");
		return NULL;
	}
	if(content) memmove(&list->content,&content,content_size);
	list->next=next;
	return list;
}

void free_node(struct list *node){
	if(list){
		free(node->content);
		free(node);
	}
}

void free_list(struct list *list){
	if(list){
		free_list(list->next);
		free_node(node);
	}
}

struct list *add_element_aux(struct list *node,void *content,size_t content_size,(int *)compare(void *content1,void *content2)){
	int comp=compare(node->content,content);
	if(node==NULL) return init_list(content,content_size,NULL);
	if(comp>0){
		return init_list(content,content_size,node);
	}
	if(comp<0){
		node->next=add_element_aux(node->next,content,content_size,&compare);
		return node;
	}
	return node;
}

short add_element(struct list **list,void *content,size_t content_size,(int *)compare(void *content1,void *content2)){
	if(*list==NULL){
		*list=init_list(*content,content_size,NULL);
		return (**list!=NULL);
	}
	list->next=add_element_aux(list->next,content,content_size,compare);
	return **list->next!=NULL;
}

void *get_element(struct list *list,void *content,(int *)compare(void *content1,void *content2)){
	if(list==NULL) return NULL;
	int comp=compare(list->content,content);
	if(comp==0) return list->content;
	if(comp<0) return NULL;
	return get_element(list->next,content,&compare);
}

short remove_element_aux(struct list *node,void *content,(int *)compare(void *content1,void *content2)){
	int comp=compare(node->content,content);
	if(node==NULL) return NULL;
	if(comp==0){
		struct list *res=node->next;
		free_node(node);
		return res;
	}
	node->next=remove_element_aux(node->next,content,&compare);
	return node;
}


short remove_element(struct list **list, void *content,(int *)compare(void *content1,void *content2)){
	if(*list==NULL) return 1;
	*list=remove_element_aux(*list,content,&compare);
	return (**list!=NULL);
}

struct list *get_last(struct list *node){
	if(node){
		if(node->next) return get_last(node->next);
		return node;
	}
	return NULL;
}


int length(struct list *list){
	if(list==NULL) return 0;
	return 1+length(list->next;)
}