#ifndef HASHMAP_H
#define HASHMAP_H
#define LENGTH 101
typedef struct node node;
typedef struct hashmap hashmap;

struct node{
	struct neighbor *key;
	struct ident *val;
	struct node *next

}

struct hashmap{
	struct node *arr[LENGTH];
}
#endif