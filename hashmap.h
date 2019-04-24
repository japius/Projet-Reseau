#ifndef HASHMAP_H
#define HASHMAP_H
#define LENGTH 101

#include "struct.h"
typedef struct node node;
typedef struct hashmap hashmap;

struct node{
	void *key;
	void *val;
	struct node *next;
}

struct hashmap{
	struct node *arr[LENGTH];
};

int hashcode(int len, void *key);

void * get_value(hashmap *map,void *key);

struct ident *get_ident(hashmap *map,struct neighbor *key);

struct to_flood *get_flood(hashmap *map,struct data_index *key);

short add_entry(hashmap *map,void *key,void *value);

short rm_entry(hashmap *map,void *key);

void print(hashmap *map);

hashmap* init_potential();
hashmap *init_neighbors();
hashmap *init_data();


//void initialize();

/*struct node{
	struct neighbor *key;
	struct ident *val;
	struct node *next;

};

struct ident *get_neighbor(hashmap *map,struct neighbor *key);

short add_neighbor(hashmap *map,struct neighbor *key,struct ident *value);

short rm_neighbor(hashmap *map,struct neighbor *key);

void print(hashmap *map);

*/
#endif