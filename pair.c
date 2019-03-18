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

int main(int argc, char *argv[])
{
	random_on_octets(&id,sizeof(u_int64_t));
	/*struct TLV tlv;
	short_tlv_hello(&tlv,sizeof(tlv),id);
	printf("id = %lu\ntlv : type -> %d, length -> %d, id -> %lu\n",id,tlv.type,tlv.length,tlv.id);*/
	if(argc==3){
		int soc  = init_socket_client_udp_v2();//(argv[1],argv[2]);
		if(soc<0){
			fprintf(stderr, "Erreur à la création de la socket\n");
			exit(0);
		}
		//write(soc,&id,sizeof(u_int64_t));
		struct neighbor ngb = {0};
		ngb.port=atoi(argv[2]);
		inet_pton(AF_INET6,argv[1],ngb.ip);
		print_addr(ngb.ip);
		send_message(soc,&id,sizeof(id),ngb);
		printf("client : client -> %lu\n",id);
		char buf[10]={0}; 
		struct sockaddr_in6 client;
		socklen_t client_len = sizeof(struct sockaddr_in6);
		recvfrom(soc,buf,3,0,&client,&client_len);
		printf("port = %d\n",ntohs(client.sin6_port ));
		//read_socket(soc,buf,3,NULL);
		printf("J'ai recu %s\n",buf);
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
			send_message(soc,"hey",3,truc);
		}
	}
	return 0;
}