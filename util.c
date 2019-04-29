#include "struct.h"
#include "peer.h"
#include "list.h"
#include "abr.h"
#include <time.h>


/*void flood_message(struct flood_entry flood){
	//faire un fork pour leur envoyer en même temps ? ou select c'est suffisant ?
	for(struct list_entry list=flood.sym_neighbors;!list;list=list.next){
		flood_message_to_neighbour(flood.data_index,flood.data,list);
	}
}

void flood_message_to_neighbour(struct data_index index, char *tlv,struct list_entry list){
	// On doit =
	//-si times_sent=5, envoyer un go_away
	//-sinon:
	//-tirer un nombre aleatoire entre 2 times sent-1 et +1
	//-faire +1 sur times sent
	//-envoyer un data qui contient data, ID nonce 
	
	if(list->times_sent==5){
		//Send go_away;
		char goaway[26]="Wesh, t'es où ? Bref,bye";
		char tlv2[26+3];
		//Supprimer de la liste des voisins
		tlv_goaway(tlv2,28,2,goaway,26);
		//ici le sendto
		NEIGHBORS=remove_neighbour(list->sym,NEIGHBORS);
	}
	char *body;
	int wait;
	//wait=rand(pow(2,times_sent-1),pow(2,times_sent));
	// a revoir
	//htons ?

	if(rc==0){
		perror("data");
		exit(EXIT_FAILURE);
	}
	//send le tlv après wait secondes;
	//struct sockaddr_in6=neighbor_to_sockaddr6(*list->sym);
	int body_length=tlv[1],msg_length=body_length+4;
	//create_message, sendto
	times_sent++;
}*/

int compare_n(struct neighbor *key1,struct neighbor *key2){
	for(int i = 0;i<16;i++){
		if(key1->ip[i] < key2->ip[i]) return -1;
		if(key1->ip[i] > key2->ip[i]) return 1;
	}

	if(key1->port < key2->port) return -1;
	if(key1->port > key2->port) return 1;
	return 0;
}

int get_seconds(){
	struct timespec tmspc = {0};
	int tmp=clock_gettime(CLOCK_MONOTONIC,&tmspc);
	return tmspc.tv_sec;
}

//Peut etre récupérer l'interface de JUlius apres un recevfrom et enovyer au groupe de multicast
void discover_neighbors(){

}

//MULTICAST
/*void hello_multicast_local(){
	int rc=0;
	int sock=socket(AF_INET6,SOCK_DGRAM,0);
	if(sock<0){
		perror("sock");
		exit(EXIT_FAILURE);
	}
	int val=1,val2=0;
	if(setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&val,sizeof(val))<0){
			perror("REUSE_ADDR");
			exit(EXIT_FAILURE)
	}
	if(setsockopt(sock,SOL_SOCKET,IPV6_V6ONLY,&val,sizeof(val))<0){
			perror("V6_ONLY");
			exit(EXIT_FAILURE)
	}
	if(setsockopt(sock,IPPROTO_IPV6,IPV6_MULTICAST_LOOP,&val2,sizeof(val2))<0){
			perror("MUTLICAST_LOOP");
			exit(EXIT_FAILURE)
	}
	if(setsockopt(sock,IPPROTO_IPV6,IPV6_UNICAST_HOPS,&val,sizeof(val))<0){
			perror("UNICAST_HOPS");
			exit(EXIT_FAILURE)
	}
	if(setsockopt(sock,IPPROTO_IPV6,IPV6_MULTICAST_HOPS,&val,sizeof(val))<0){
			perror("MUTLICAST_HOPS");
			exit(EXIT_FAILURE)
	}
	if(setsockopt(sock,IPPROTO_IPV6,IPV6_JOIN_GROUP,&mreq,sizeof(mreq))<0){
			perror("JOIN_GROUP");
			exit(EXIT_FAILURE)
	}
	struct sockaddr_in6 sin6;
	sin6.sin6_family=AF_INET6;
	sin6.sin6_port=htons(1212);
	rc=inet_pton(AF_INET6,"ff12:b456:dad4:cee1:4589:71de:a2ec:e66",&sin6.sin6_addr);
	//pas sur ptet faire un recvfrom de cette adresse et verifier le champ
	//donc s'abonner au groupe de multicast
	sin6.sin6_scope_id=if_nametoindex("eth0");
	//send un hello court à cette interface
	rc = sendto(sock, req, req_len, 0, (struct sockaddr *)&sin6, client_len);
					if(rc < 0) {
						perror("sendto");
						exit(EXIT_FAILURE);
	}

}*/


//LOL, IL TROLLE JULIUS
void hello_multicast_global(){

}

