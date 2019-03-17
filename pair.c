#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "struct.h"
#include "net_lib.h"

u_int64_t id;

int main(int argc, char const *argv[])
{
	random_on_octets(&id,sizeof(u_int64_t));
	printf("%ld = neighbor, %ld = message_h\n",sizeof(struct neighbor),sizeof(struct message_h));
	return 0;
}