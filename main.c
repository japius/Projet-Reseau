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


int main(int argc, char *argv[]){
	//u_int64_t ID;
	//initialisation du pair
	random_on_octets(&ID,sizeof(u_int64_t));
	POTENTIAL=init_first();
	NEIGHBORS=init_first();
	DATAF=init_data();
	int soc = init_socket_client_udp_v2();
	int nb;
	if(argc==3)
		nb = send_first_message(soc,argv[1],argv[2]);
	else
		nb = send_first_message(soc,"jch.irif.fr","1212");
	printf("J'ai envoye hello a %d adresse(s)\n", nb);

	struct message_h msg;
	struct sockaddr_in6 client;
	socklen_t client_len;

	while(1){
		//---- gere les réceptions de messages
		// XXX select a ajouter
		socklen_t client_len = sizeof(struct sockaddr_in6);
		int size_msg = recvfrom(soc,&msg,sizeof(struct message_h),0,&client,&client_len);
		struct neighbor tmp = sockaddr6_to_neighbor(client);
		print_msg(msg);
		handle_message_h(soc,&msg,size_msg,tmp);

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