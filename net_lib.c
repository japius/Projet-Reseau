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
#include "peer.h"


int init_socket_client_udp_v2(){
	int soc =socket(PF_INET6, SOCK_DGRAM,0),val=0;
	int rc=setsockopt(soc,IPPROTO_IPV6,IPV6_V6ONLY,&val,sizeof(val));
	if(rc<0){
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	return soc;
}

int send_first_message(int soc, char *addr, char *port){
	struct addrinfo h = {0};
	struct addrinfo *r = {0};
	int rc = 0;
	h.ai_family = AF_INET6;
	h.ai_socktype = SOCK_DGRAM;
	h.ai_flags = AI_V4MAPPED|AI_ALL;
	h.ai_protocol = 0;

	struct message_h msg = {0};
	msg.magic = 93;
	msg.version = 2;
	msg.body_length  = 10;
	msg.body_length = htons(msg.body_length);
	msg.body[0]=2;
	msg.body[1]=8;
	memcpy((msg.body)+2,&ID,8);

	rc = getaddrinfo(addr, port, &h, &r);

	if(rc < 0){
		fprintf(stderr, "Erreur lors de getaddrinfo\n");
		return -1;
	}

	int nb=0;
	for(struct addrinfo *p =r; p!=NULL; p = p->ai_next){
		print_addr(p->ai_addr);
		int blop = sendto(soc,&msg,14,0,p->ai_addr,p->ai_addrlen);
		if(blop>0) nb++;
	}

	freeaddrinfo(r);
	return nb;
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
	char *tmp=(char*)var;
	for(int i=0;i<octets_number;i++)
		*(tmp+i)=(char)rand();
}

int send_message(int fd,void *buf, size_t taille,struct neighbor rcpt){
	struct sockaddr_in6 server = neighbor_to_sockaddr6(rcpt);
	return sendto(fd,buf,taille,0,(struct sockaddr*)&server,sizeof(server));
}

int send_to_everyone(int fd, void *buf, size_t length, tree *people){
	if(!people) return 0;
	int res = (send_message(fd,buf,length,*(people->key))>0)?1:0;
	res += send_to_everyone(fd,buf,length,people->left);
	res += send_to_everyone(fd,buf,length,people->right);
	return res;
}

int send_hello_everyone(int fd, tree *people){
	if(!people) return 0;
	struct message_h msg;
	msg.magic=93;
	msg.version=2;
	int tmp = tlv_long_hello(msg.body,MAX_SIZE,ID,people->val->id);
	msg.body_length=htons(tmp);
	int res = (send_message(fd,&msg,tmp+4,*(people->key))>0)?1:0;
	res += send_hello_everyone(fd,people->left);
	res += send_hello_everyone(fd,people->right);
	return res;
}

//On récupère le message
/*int get_message(int sock, struct sockaddr_in6 client,unsigned char req[4096]){
	return recvfrom(sock,req,4096,0,&client,sizeof(client));

}*/

struct neighbor sockaddr6_to_neighbor(struct sockaddr_in6 saddr){
	struct neighbor res;
	res.port = ntohs(saddr.sin6_port);
	//print_addr(saddr.sin6_addr.s6_addr);
	memcpy(res.ip,saddr.sin6_addr.s6_addr,16);
	//print_addr(res.ip);
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
	printf("Address: %s\n",buffer);
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

void print_sockaddr(struct sockaddr_in6 tmp){
	printf("sin6_family = %d\n sin6_port = %d\nsin6_flowinfo = %d\n",tmp.sin6_family,tmp.sin6_port,tmp.sin6_flowinfo);
	write(1,"sin6_addr = ",12);
	print_addr(tmp.sin6_addr.s6_addr);
	printf("sin6_scope_id = %d\n",tmp.sin6_scope_id);
}