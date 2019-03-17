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
#include <string.h>
#include "net_lib.h"
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

int init_socket_client_udp_v2(){
	int soc =socket(AF_INET6, SOCK_DGRAM,0);
	return soc;
}

int init_socket_server_udp(int port){
	int soc = socket(AF_INET6,SOCK_DGRAM,0);
	if(soc<0){
		fprintf(stderr, "Erreur lors de la création de la socket.\n");
		return -1;
	}

	struct sockaddr_in6 tmp = {0};
	tmp.sin6_family = AF_INET6;
	tmp.sin6_port = htons(port);
	int val=0;
	if(setsockopt(soc,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &val, sizeof(val))<0){
		fprintf(stderr, "Erreur numéro %d à setsockopt\n",val);
	}

	if(bind(soc,&tmp,sizeof(struct sockaddr_in6))<0){
		fprintf(stderr, "Erreur lors du bind\n");
		return -2;
	}
	return soc;
}

int read_socket(int fd,void *buf, size_t size_buf,struct timeval *timeout){
	fd_set tmp;
	FD_ZERO(&tmp);
	FD_SET(fd,&tmp);
	for(unsigned int count = 0;count<size_buf;){
		if(select(fd+1,&tmp,NULL,NULL,timeout)==0)
			return 0;
		count+=read(fd,buf+count,size_buf-count);
	}
	return 1;
}

void random_on_octets(void *var, size_t octets_number){
	srand(time(NULL));
	char *tmp=(char*)var;
	for(int i=0;i<octets_number;i++)
		*(tmp+i)=(char)rand();
}

int send_message(int fd,void *buf, size_t taille,struct neighbor rcpt){
	struct sockaddr_in6 server;
	//server.sin6_len = sizeof(server);
	server.sin6_family = AF_INET6;
	server.sin6_flowinfo = 0;
	server.sin6_port = htons(rcpt.port);
	struct in6_addr tmp;
	memcpy(&tmp,&(rcpt.ip),16);
	server.sin6_addr = tmp;
	return sendto(fd,buf,taille,0,(struct sockaddr*)&server,sizeof(server));
}

struct neighbor sockaddr6_to_neighbor(struct sockaddr_in6 saddr){
	struct neighbor res;
	res.port = ntohs(saddr.sin6_port);
	memcpy(res.ip,saddr.sin6_addr.s6_addr,16);
	print_addr(res.ip);
	return res;
}

void print_addr(char *ip){
	for(int i=0;i<15;i++){
		printf("%d.",ip[i]);
	}
	printf("%d\n",ip[15]);
}
