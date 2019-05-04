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
#define PORT 1212
#define TIMEHELLO 10
#define MIN_SYM 10


//pas oublier de supprimer les potentiels s'ils répondent pas depuis trop longtemps
int main(int argc, char *argv[]){
	//u_int64_t ID;
	//initialisation du pair
	random_on_octets(&ID,sizeof(u_int64_t));
	srand(time(NULL));
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
	printf("hello envoyé a %d\n",nb );

	struct message_h msg;
	struct sockaddr_in6 client;
	socklen_t client_len;
	fd_set fd_ens;
	int NEXTHELLO = 0;
	NEXTTIME=NEXTHELLO;
	init_list(&DATAF,compare_d,sizeof(flood_entry));

	while(1){
		//---- gere les réceptions de messages
		// XXX select a ajouter
		/*if(!NEIGHBORS){
			printf("Aucun voisins a disposition\n");
			if(argc==3)
				nb = send_first_message(soc,argv[1],argv[2]);
			else;
				nb = send_first_message(soc,"jch.irif.fr","1212");
		}*/

		FD_ZERO(&fd_ens);
		FD_SET(soc,&fd_ens);
		FD_SET(0,&fd_ens);
		NEXTTIME = (NEXTTIME<NEXTHELLO)?NEXTTIME:NEXTHELLO;
		struct timeval timeout = {(max(0,NEXTTIME-get_seconds())),0};
		NEXTTIME=NEXTHELLO;
		if(select(soc+1,&fd_ens,NULL,NULL,&timeout)){
			if(FD_ISSET(soc,&fd_ens)){
				socklen_t client_len = sizeof(struct sockaddr_in6);	
				int size_msg = recvfrom(soc,&msg,sizeof(struct message_h),0,&client,&client_len);
				struct neighbor ngb = sockaddr6_to_neighbor(client);
				print_addr(ngb.ip);
				print_msg(msg);
				handle_message_h(soc,&msg,size_msg,ngb);
			}
			if(FD_ISSET(0,&fd_ens)){
				unsigned char buf[PMTU-14];
				int tmp=read(0,buf,PMTU-14)-1;
				tmp=tlv_data(msg.body,MAX_SIZE,ID,0,buf,tmp);
				if(tmp>0){
					msg.magic=93;
					msg.version=2;
					msg.body_length=htons(tmp);
					nb = send_to_everyone(soc,&msg,tmp+4,NEIGHBORS);
				}
			}
		}
		if(get_seconds()>=NEXTHELLO){
			nb = send_hello_everyone(soc,NEIGHBORS);
			//printf("Hello long %d\n",nb );
			nb=send_symetrical_everyone(soc,NEIGHBORS);
			//printf("Neighbor %d\n",nb);
			NEXTHELLO=get_seconds()+TIMEHELLO;
			nb=send_goaway_asymetrical(soc);
			printf("GO away %d\n",nb );
			if(NB_SYMETRICAL<=MIN_SYM){
				nb = send_shorthello_everyone(soc,POTENTIAL);
				printf("Hello court %d\n",nb);
				printf("--------POTENTIAL ----------\n");
				print_tree(POTENTIAL);
				printf("-----------------------------\n");
			}
		}

		printf("--------NEIGHBORS ----------\n");
		print_tree(NEIGHBORS);
		printf("-----------------------------\n");

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