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
#define PORT 8080


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
	if(argc==2){
		struct sockaddr_in6 tmp = {0};
		tmp.sin6_family = AF_INET6;
		tmp.sin6_port = htons(PORT);
		bind(soc,&tmp,sizeof(tmp));
	}

	int nb;
	/*if(argc==3)
		nb = send_first_message(soc,argv[1],argv[2]);
	else
		nb = send_first_message(soc,"jch.irif.fr","1212");
	printf("J'ai envoye hello a %d adresse(s)\n", nb);*/

	struct message_h msg;
	struct sockaddr_in6 client;
	socklen_t client_len;
	fd_set fd_ens;
	NEXTTIME = get_seconds()+30;

	while(1){
		//---- gere les réceptions de messages
		// XXX select a ajouter
		if(!NEIGHBORS){
			printf("Aucun voisins a disposition\n");
			if(argc==3)
				nb = send_first_message(soc,argv[1],argv[2]);
			else
				nb = send_first_message(soc,"jch.irif.fr","1212");
			printf("J'ai envoye hello a %d adresse(s)\n", nb);
		}

		FD_ZERO(&fd_ens);
		FD_SET(soc,&fd_ens);
		FD_SET(0,&fd_ens);
		struct timeval timeout = {(max(0,NEXTTIME-get_seconds())),0};
		NEXTTIME = get_seconds()+30;
		if(select(soc+1,&fd_ens,NULL,NULL,&timeout)){
			if(FD_ISSET(soc,&fd_ens)){
				socklen_t client_len = sizeof(struct sockaddr_in6);	
				int size_msg = recvfrom(soc,&msg,sizeof(struct message_h),0,&client,&client_len);
				struct neighbor ngb = sockaddr6_to_neighbor(client);
				print_msg(msg);
				handle_message_h(soc,&msg,size_msg,ngb);
			}
			if(FD_ISSET(0,&fd_ens)){
				//if faut retirer les 15 octets d'infos sur le data
				//et 48 octets pour l'entete IPV6 et la charge du datagramme udp
				unsigned char buf[PMTU-63];
				int tmp=read(0,buf,PMTU-63);
				tmp=tlv_data(msg.body,MAX_SIZE,ID,0,buf,tmp);
				if(tmp>0){
					msg.magic=93;
					msg.version=2;
					msg.body_length=tmp;
					printf("envoie a %d\n",send_to_everyone(soc,&soc,tmp+4,NEIGHBORS));
				}
			}
		}

	}
	/*Il faut:
	-envoyer toutes les 30s des hellos long à chaque voisin
	-envoyer souvent des tlv neighbour aux voisins pour les informer de ses voisins symétriques
	-si moins de 8 symétriques envoyer un hello court à 1 ou plusieurs potentiels
	-envoyer goaway puis suppression, si pas de message depuis 2 min
	-si un voisin pas de hello long depuis 2 min, marqué comme non symétrique
	-traiter la réception de tlv
	*/
	return 0;
}