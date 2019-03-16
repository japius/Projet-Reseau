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

int init_socket_client_udp(char *addr, char *port){
	struct addrinfo h = {0};
	struct addrinfo *r = {0};
	int rc = 0;

	h.ai_family = AF_UNSPEC;
	h.ai_socktype = SOCK_DGRAM;
	h.ai_flags = 0;
	h.ai_protocol = 0;

	rc = getaddrinfo(addr, port, &h, &r);
	if(rc < 0){
		fprintf(stderr, "Erreur lors de getaddrinfo\n");
		return -1;
	}

	int soc = -1;
	rc=-1;
	for(struct addrinfo *p =r; p!=NULL; p = p->ai_next){
		soc = socket(p->ai_family, p->ai_socktype,p->ai_protocol);
		if(soc>=0){
			rc = connect(soc,p->ai_addr,p->ai_addrlen);
			if(rc >= 0) break;
				close(soc);
		}
	}
	freeaddrinfo(r);
	if(rc < 0 ){
		fprintf(stderr, "Ne se connecte à aucune addresse de addrinfo\n");
		return -2;
	}
	return soc;
}

void read_socket(int fd,void *buf, size_t size_buf){
	for(unsigned int count = 0;count<size_buf;){
		printf("J'ai lu %d\n",count);
		count+=read(fd,buf+count,size_buf-count);
		printf("Je suis apres read %d\n",count );
	}
}