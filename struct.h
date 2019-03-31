/* Toutes les structures utiles */
#include <sys/types.h>
#ifndef STRUCT_H
#define STRUCT_H
#define MAX_SIZE 4096


struct message_h{
	char magic;
	char version;
	u_int16_t body_length;
	char body[MAX_SIZE]; //XXX
};

struct TLV{
	char type;
	char length;
	u_int64_t id;
	//char body[MAX_SIZE];
};

struct neighbor{
	u_int8_t ip[16];
	int port;
};

struct ident{
	u_int64_t id;
	time_t last_hello;
	time_t last_hello_long;
};
#endif