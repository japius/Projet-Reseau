/* Toutes les structures utiles */
#include <sys/types.h>

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
	char body[0]; //XXX
};

struct neighbor{
	char ip[16];
	int port;
};

struct ident{
	u_int64_t id;
	time_t last_hello;
	time_t last_hello_long;
};