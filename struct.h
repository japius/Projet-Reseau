/* Toutes les structures utiles */
#include <sys/types.h>

#define MAX_SIZE 4096

struct massage_h{
	char magic;
	char version;
	u_int16_t body_length;
	char body[0]; //XXX
};

struct TLV{
	char type;
	char length;
	char body[0]; //XXX
};
