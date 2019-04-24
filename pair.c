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
#include "tlv.h"

static u_int64_t id;
static int test = 0;
struct message_h msg;

int main(int argc, char *argv[])
{
	random_on_octets(&id,sizeof(u_int64_t));
	/*struct TLV tlv;
	short_tlv_hello(&tlv,sizeof(tlv),id);
	printf("id = %lu\ntlv : type -> %d, length -> %d, id -> %lu\n",id,tlv.type,tlv.length,tlv.id);*/
	if(argc>=3){
		if(argc>=4){
			test=atoi(argv[3]);
		}
		int soc ;
		if(test)
			soc = init_socket_client_udp_v2();//(argv[1],argv[2]);
		else 
			soc = init_socket_client_udp(argv[1],argv[2]);

		if(soc<0){
			fprintf(stderr, "Erreur à la création de la socket\n");
			exit(0);
		}
		if(test==1){
			struct neighbor ngb = {0};

			ngb.port=atoi(argv[2]);
			inet_pton(AF_INET6,argv[1],ngb.ip);
			print_addr(ngb.ip);
			send_message(soc,&id,sizeof(id),ngb);
		}else if(test==2){
			//send_first_message(soc,argv[1],argv[2],"hey",3);
		}else{
			//write(soc,&id,sizeof(u_int64_t));
		}
		/*printf("client : client -> %lu\n",id);
		char buf[10]={0}; 
		struct sockaddr_in6 client;
		socklen_t client_len = sizeof(struct sockaddr_in6);
		int t = recvfrom(soc,buf,4,0,&client,&client_len);
		printf("port = %d\n",ntohs(client.sin6_port ));
		printf("J'ai recu %s, %d\n",buf,t);*/
		msg.magic = 93;
		msg.version = 2;
		msg.body_length  = sizeof(struct TLV);
		msg.body_length = htonl(msg.body_length);
		short_tlv_hello(msg.body,400,id);
		printf("Icic\n");
		if(test){
			send_first_message(soc,argv[1],argv[2],&msg,sizeof(struct message_h));
		}else{
			write(soc,&msg,sizeof(msg));
		}	

		struct message_h mmm;
		struct sockaddr_in6 client;
		socklen_t client_len = sizeof(struct sockaddr_in6);
		printf("hey");
		int t = recvfrom(soc,&mmm,sizeof(struct message_h),0,&client,&client_len);
		printf("J'ai recu un msg taille %d",t);
	}else if(argc==2){
		int soc = init_socket_server_udp(atoi(argv[1]));
		if(soc<0){
			fprintf(stderr, "Erreur à la création de la socket\n");
			exit(0);
		}
		u_int64_t id_pair;
		struct sockaddr_in6 client;
		socklen_t client_len = sizeof(struct sockaddr_in6);
		while(1){
			recvfrom(soc,&id_pair,sizeof(id_pair),0,&client,&client_len);
			//read_socket(soc,&id_pair,sizeof(id_pair),NULL);
			printf("server : server -> %lu\nserver : client -> %lu\n",id,id_pair);
			struct neighbor truc = sockaddr6_to_neighbor(client);
			printf("port = %d\n",truc.port );
			printf("send message = %d",send_message(soc,"hey",3,truc));
		}
	}
	return 0;
}