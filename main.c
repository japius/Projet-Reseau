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
#include "peer.h"
#include "tlv.h"
#include "abr.h"
#include "list.h"
#include "util.h"
#include "websocket.h"
#include <libwebsockets.h>
#define PORT 1212
#define TIMEHELLO 30
#define MIN_SYM 10

#define TEST 30

//pas oublier de supprimer les potentiels s'ils répondent pas depuis trop longtemps
int main(int argc, char *argv[]){
	//handle_gui();
	//initialisation du pair
	srand(time(NULL));
	random_on_octets(&ID,sizeof(u_int64_t));
	/*POTENTIAL=init_first();
	NEIGHBORS=init_first();
	DATAF=init_data();*/

	int soc = init_socket_client_udp_v2();
	if(argc==2||argc==4){
		struct sockaddr_in6 tmp = {0};
		tmp.sin6_family = PF_INET6;
		tmp.sin6_port = htons(atoi(argv[1]));
		bind(soc,&tmp,sizeof(tmp));
	}

	int nb;

	
		if(argc==3)
			nb = send_first_message(soc,argv[1],argv[2]);
		else if(argc==4)
			nb = send_first_message(soc,argv[2],argv[3]);
		else
			nb = send_first_message(soc,"jch.irif.fr","1212");
	
	FD_MAGIC_READ = 0;
	FD_MAGIC_WRITE = 1;


	struct message_h msg;
	struct sockaddr_in6 client;
	socklen_t client_len;
	fd_set fd_ens;
	int NEXTHELLO = 0;
	NEXTTIME=NEXTHELLO;
	init_list(&DATAF,compare_flood,sizeof(struct flood_entry));

	while(1){
		//---- gere les réceptions de messages
		// XXX select a ajouter

		FD_ZERO(&fd_ens);
		FD_SET(soc,&fd_ens);
		FD_SET(FD_MAGIC_READ,&fd_ens);
		NEXTTIME = (NEXTTIME<NEXTHELLO)?NEXTTIME:NEXTHELLO;
		struct timeval timeout = {(max(0,NEXTTIME-get_seconds())),0};
		NEXTTIME=NEXTHELLO;
		if(select(max(FD_MAGIC_READ,soc)+1,&fd_ens,NULL,NULL,&timeout)){
			if(FD_ISSET(soc,&fd_ens)){
				socklen_t client_len = sizeof(struct sockaddr_in6);	
				int size_msg = recvfrom(soc,&msg,sizeof(struct message_h),0,&client,&client_len);
				struct neighbor ngb = sockaddr6_to_neighbor(client);
				//print_addr(ngb.ip);
				//print_msg(msg);
				handle_message_h(soc,&msg,size_msg,ngb);
			}
			if(FD_ISSET(FD_MAGIC_READ,&fd_ens)){

				unsigned char buf[(1<<8)-1] = {0};
				u_int8_t tmp=read(FD_MAGIC_READ,buf,(1<<8)-14);
				if(buf[tmp-1]==0) tmp--;
				if(buf[tmp-1]=='\n') tmp--;

				unsigned char msg_to_send[(1<<8)+4];
				tlv_data(msg_to_send,(1<<8)+4,ID,0,buf,tmp);
				add_message_to_flood(msg_to_send+2,msg_to_send[1],NULL);
				print_on_screen(buf,tmp);
			}
		}
		if(get_seconds()>=NEXTHELLO){
			nb = send_hello_everyone(soc,NEIGHBORS);
			nb=send_symetrical_everyone(soc,NEIGHBORS);
			NEXTHELLO=get_seconds()+TIMEHELLO;
			nb=send_goaway_asymetrical(soc);
			remove_old_potential();
			if(NB_SYMETRICAL<=MIN_SYM){
				nb = send_shorthello_everyone(soc,POTENTIAL);
				/*printf("Hello court %d\n",nb);
				printf("--------POTENTIAL ----------\n");
				print_tree(POTENTIAL);
				printf("-----------------------------\n");*/
			}
			
		printf("--------NEIGHBORS ----------\n");
		print_tree(NEIGHBORS);
		printf("-----------------------------\n");
		}
		flood_messages(soc,&DATAF);

		send_to_everyone_now(soc, NEIGHBORS);

	}
	/*Il faut:
	++-envoyer toutes les 30s des hellos long à chaque voisin
	+-envoyer souvent des tlv neighbour aux voisins pour les informer de ses voisins symétriques
	+-si moins de 8 symétriques envoyer un hello court à 1 ou plusieurs potentiels
	-envoyer goaway puis suppression, si pas de message depuis 2 min
	-si un voisin pas de hello long depuis 2 min, marqué comme non symétrique
	-traiter la réception de tlv
	*/
	return 0;
}