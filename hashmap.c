#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"

struct node *createnode(void *key, void *val,struct node *next){
    struct node *node=malloc(sizeof(struct node));
    node->key=key;
    node->val=val;
    node->next=next;
    return node;
}

struct hashmap *createhashmap(){
    struct hashmap *t = (struct hashmap*)malloc(sizeof(struct hashmap));
    for(i=0;i<LENGTH;i++)
        t->arr[i] = NULL;
    return t;
}

int hashcode(int length,void *key){
  //  To be chosen
 //   if(key<0)
     //   return -(key%t->size);
   // return key%t->size;
	return 0;
}

struct node * insertKeyValue(struct node *list, void *key, void *val){
    return createnode(key,val,list);
}

short add_entry(hashmap *map,void *key,void *value){
	int pos=hashcode(LENGTH,key);
	//some optimization here, in case we often get the same value
	map->arr[pos]=insertKeyValue(map->arr[pos],key,value);
}

void *getValue(struct node *node, void *key){
	//see if we shouldn't order the list or put a search tree ...
    struct node *tmp=node;
    while(tmp!=NULL){
        if(tmp->key==key){
            return tmp->val;
        }
        tmp=tmp->next;
    }
    return NULL;
}

void *get_entry(hashmap *map,void *key){
	int pos=hashcode(LENGTH,key);
	return getValue(map->arr[pos],key);
}

struct ident * get_ident(hashmap *map,struct neighbor *key){
	return (struct ident *)get_entry(map,key);
}

struct to_flood *get_flood(hashmap *map,struct data_index *key){
	return (struct to_flood *)get_entry(map,key);
}


short rm_neighbor(hashmap *map,struct neighbor key){
	return 0;
}

void print(hashmap *map){

}

/*struct node *createnode(struct neighbor *key, struct ident *val,struct node *next){
    struct node *node=malloc(sizeof(struct node));
    node->key=key;
    node->val=val;
    node->next=next;
    return node;
}

struct hashmap *createhashmap(){
    struct hashmap *t = (struct hashmap*)malloc(sizeof(struct hashmap));
    for(i=0;i<LENGTH;i++)
        t->arr[i] = NULL;
    return t;
}

int hashcode(int length,struct neighbor *key){
  //  To be chosen
 //   if(key<0)
     //   return -(key%t->size);
   // return key%t->size;
	return 0;
}

struct node * insertKeyValue(struct node *list, struct neighbor *key, struct ident *val){
    return createnode(key,val,list);
}

short add_neighbor(hashmap *map,struct neighbor *key,struct ident *value){
	int pos=hashcode(LENGTH,key);
	//some optimization here, in case we often get the same value
	mpa->arr[pos]=insertKeyValue(map->arr[pos],key,value);
}

struct ident *getValue(struct node *node, struct neighbor *key){
	//see if we shouldn't order the list or put a search tree ...
    struct node *tmp=node;
    while(tmp!=NULL){
        if(tmp->key==key){
            return tmp->val;
        }
        tmp=tmp->next;
    }
    return NULL;
}

struct ident *get_neighbor(hashmap *map,struct neighbor *key){
	int pos=hashcode(LENGTH,key);
	return getValue(map->arr[pos],key);
}


short rm_neighbor(hashmap *map,strut neighbor *key){
	return 0;
}

void print(hashmap *map){

}*/