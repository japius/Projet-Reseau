/* Toutes les structures utiles */
#include <sys/types.h>
#ifndef STRUCT_H
#define STRUCT_H
#define MAX_SIZE 4096

typedef struct TLV TLV;

struct message_h{
	char magic;
	char version;
	u_int16_t body_length;
	unsigned char body[MAX_SIZE];
};
//Les données pour un voisin
struct neighbor{
	u_int8_t ip[16];
	int port;
};

struct ident{
	u_int64_t id;
	time_t last_hello;
	time_t last_hello_long;
};


//Les données pour la liste de données récentes
struct data_index{
	u_int64_t id;
	u_int32_t nonce;
};

struct list_entry{
	struct neighbor *sym;
	int times_sent;
	struct data_entry *next;
};


//Un noeud de la liste d'inondations, soit un couple (Id,Nonce) et une liste de voisins à inonder
struct flood_entry{
	struct data_index *index;
	struct list_entry *sym_neighbors;
	struct flood *next;
}

//La liste d'inondations 
struct flood{
	struct flood_entry *first;
	struct flood_entry *last;
}

#endif
