#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include "struct.h"
#include "net_lib.h"
//#include "tlv.h"

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
		/*if(test){
			struct neighbor ngb = {0};
			ngb.port=atoi(argv[2]);
			inet_pton(AF_INET6,argv[1],ngb.ip);
			print_addr(ngb.ip);
			send_message(soc,&id,sizeof(id),ngb);
		}*/
		msg.magic = 93;
		msg.version = 2;
		msg.body_length  = 10;
		msg.body_length = htons(msg.body_length);
		msg.body[0]=2;
		msg.body[1]=8;
		memcpy((msg.body)+2,&id,8);
		print_msg(msg);
		printf("Icic\n");
		int soc2 = init_socket_client_udp_v2();
		if(test){
			char test[7]="bonbon";
			//printf(test);
			//send_first_message(soc,argv[1],argv[2],test,7);
			send_first_message(soc,argv[1],argv[2],&msg,4+ntohs(msg.body_length));
		}else{
			write(soc,&msg,4+ntohs(msg.body_length));
		}	

		struct message_h mmm;
		struct sockaddr_in6 client;
		socklen_t client_len = sizeof(struct sockaddr_in6);
		int t = recvfrom(soc,&mmm,sizeof(struct message_h),0,&client,&client_len);
		print_msg(mmm);
		printf("J'ai recu un msg taille %d\n",t);
		struct neighbor truc = sockaddr6_to_neighbor(client);
		send_message(soc2,&msg,4+ntohs(msg.body_length),truc);
		recvfrom(soc2,&mmm,sizeof(struct message_h),0,&client,&client_len);
		print_msg(mmm);
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
			int rc = recvfrom(soc,&msg,sizeof(struct message_h),0,&client,&client_len);
			//read_socket(soc,&id_pair,sizeof(id_pair),NULL);
			struct neighbor truc = sockaddr6_to_neighbor(client);
			printf("port = %d\n",truc.port);
			msg.body[10] = 0;
			print_msg(msg);
			printf("send message = %d",send_message(soc,&msg,4+ntohs(msg.body_length),truc));
		}
	}
	return 0;
}