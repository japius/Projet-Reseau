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

	//initialisation du pair
	random_on_octets(&id,sizeof(u_int64_t));
	potential=init_first();
	neighbors=init_first();
	dataf=init_data();

	int soc = init_socket_client_udp_v2();
	int nb = send_first_message(soc,argv[1],argv[2]);
	printf("J'ai envoye hello a %d adresse(s)\n", nb);

	while(1){

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