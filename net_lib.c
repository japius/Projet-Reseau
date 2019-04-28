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
	int soc =socket(PF_INET6, SOCK_DGRAM,0),val=0;
	int rc=setsockopt(soc,IPPROTO_IPV6,IPV6_V6ONLY,&val,sizeof(val));
	if(rc<0){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	return soc;
}

int send_first_message(int soc, char *addr, char *port,void *buf,size_t size_buf){
	struct addrinfo h = {0};
	struct addrinfo *r = {0};
	int rc = 0;
	h.ai_family = AF_INET6;
	h.ai_socktype = SOCK_DGRAM;
	h.ai_flags = AI_V4MAPPED|AI_ALL;
	h.ai_protocol = 0;

	rc = getaddrinfo(addr, port, &h, &r);

	if(rc < 0){
		fprintf(stderr, "Erreur lors de getaddrinfo\n");
		return -1;
	}
	printf("je suis la : %d\n",soc);

	for(struct addrinfo *p =r; p!=NULL; p = p->ai_next){
		printf("iciiiiiii\n");
		int blop = sendto(soc,buf,size_buf,0,p->ai_addr,p->ai_addrlen);
		perror("trucouille");
		printf("retour de sendto = %d\n",blop );
	}
	freeaddrinfo(r);
	return 0;
}

int init_socket_server_udp(int port){
	int soc = socket(PF_INET6,SOCK_DGRAM,0);
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

//On récupère le message
/*int get_message(int sock, struct sockaddr_in6 client,unsigned char req[4096]){
	return recvfrom(sock,req,4096,0,&client,sizeof(client));

}*/

struct neighbor sockaddr6_to_neighbor(struct sockaddr_in6 saddr){
	struct neighbor res;
	res.port = ntohs(saddr.sin6_port);
	print_addr(saddr.sin6_addr.s6_addr);
	memcpy(res.ip,saddr.sin6_addr.s6_addr,16);
	print_addr(res.ip);
	return res;
}

struct sockaddr_in6 neighbor_to_sockaddr6(struct neighbor neighbor){
	struct sockaddr_in6 sin6={0};
	sin6.sin6_family = AF_INET6;
	sin6.sin6_port = htons(neighbor.port);
	//apparemment pas besoin de pton
	memmove(&sin6.sin6_addr,neighbor.ip,16);
	return sin6;
}


void print_addr(u_int8_t *ip){
	char buffer[50];
	inet_ntop(AF_INET6,ip,buffer,50);
	printf("Adress: %s\n",buffer);
}


int print_tlv(unsigned char *buf){
	if(buf[0] == 0){
		printf("\t tlv 1, size 1\n");
		return 1;
	}
	printf("\t tlv %d, size %d\n",buf[0],buf[1]);
	return buf[1];
}

void print_msg(struct message_h msg){
	int l = ntohs(msg.body_length);
	printf("magic = %d, version = %d, length = %d\n data :\n",msg.magic, msg.version, l);
	for(int i=0;i<l;)
		i+=print_tlv(msg.body+i)+2;
	printf("\n");
}
