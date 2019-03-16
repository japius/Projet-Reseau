/* Toutes les structures utiles */
#define MAX_SIZE 4096
#define MAX_BODY MAX_SIZE - sizeof(char)*2

struct TLV{
	char type;
	char length;
	char body[];
};
