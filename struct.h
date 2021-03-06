/* Toutes les structures utiles */
#include <sys/types.h>
#ifndef STRUCT_H
#define STRUCT_H
#define MAX_SIZE 4096
#define PMTU 1232


struct message_h{
	char magic;
	char version;
	u_int16_t body_length;
	unsigned char body[MAX_SIZE];
};
//Les données pour un voisin
struct neighbor{
	u_int8_t ip[16];
	u_int16_t port;
	struct message_h *msg;
};

struct ident{
	u_int64_t id;
	int last_hello;
	int last_hello_long;
};


//Les données pour la liste de données récentes
struct data_index{
	u_int64_t id;
	u_int32_t nonce;
};
//pour la fragmentation des données, il faudra une hashmap ?
//un hashcode pour le nonce

//jeter si trop longtemps, voir comment faire
//faire une fonction pour écrire de grand messages
struct submessages{
	int actual_length;
	int length_received;
	char submessages[512];
};

struct big_data{
	u_int32_t global_nonce;
	u_int16_t total_length;
	u_int16_t received_length;
	int start_time;
	int type;
	char *message;
};

#endif
