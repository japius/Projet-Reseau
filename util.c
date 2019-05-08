#include <math.h>
#include <time.h>
#include "struct.h"
#include "peer.h"
#include "list.h"
#include "abr.h"
#include "tlv.h"
#include "net_lib.h"
#include <string.h>
#include "util.h"
#include "genlist.h"
#include "list.h"
#include <arpa/inet.h>


int rand_a_b(int a,int b){
	int truc = rand()%(b-a) +a;
	return truc;
}


void handle_inactive(int soc,struct data_index *data,struct neighbor  *sym){
	//Send go_away;
	char goaway[25]="Wesh, t'es où ? Bref,bye";
		//Supprimer de la liste des voisins
	struct message_h msg;
	msg.magic=93;
	msg.version=2;
	int tmp=tlv_goaway(msg.body,PMTU-4,2,goaway,24);
	msg.body_length=htons(tmp);
	send_message(soc,&msg,tmp+4,*sym);
		//ici le sendto
	remove_neighbor_everywhere(sym);
}


int wait_time(int times_sent){
	//pas oublier de rajouter get_seconds();
	if(times_sent==0) return 1;
	return rand_a_b((int)pow(2.0,times_sent-1),(int)pow(2.0,times_sent));
}


int send_data(int soc, char *tlv,struct neighbor *key){
	int body_length=tlv[1]+2;
	u_int16_t msg_length=body_length+4;
	struct message_h msg;
	msg.magic=93;
	msg.version=2;
	msg.body_length=htons(body_length);
	memcpy(msg.body,tlv,body_length);
	return send_message(soc,&msg,msg_length,*key);
}

int add_to_neighbor_message(char *tlv,struct neighbor *key){
	return 0;
	/*if(get_message(key)==NULL){
		char *body;
		struct message_h msg;
		msg.magic=93;
		msg.version=2;
		int body_length=tlv[1],msg_length=body_length+4;
		memcpy(&msg.body_length,&body_length,2);
		memcpy(msg.body,tlv,body_length);
		//copier dans le message à envoyer au neighbor
	}
	//sinon
	//juste rajouter le tlv si pas de problème avec le PMTU
	return 1;*/
}

void flood_message_to_neighbours(int soc,struct flood_entry *flood){
	int current_time=get_seconds();
	list l=flood->sym_neighbors;
	struct list_entry *tmp=l->first;
	printf("message %d, taille %d\n",flood->index->nonce,l->length);
	while(tmp){
		struct ngb_entry *nw=(struct ngb_entry *)tmp->content;
		if(nw->times_sent==5){
			struct list_entry *tmp2=tmp->next;
			handle_inactive(soc,flood->index,nw->sym);
			tmp=tmp2;
			continue;

		}
		if(current_time>=nw->wait_time){
			int i=send_data(soc,flood->data,nw->sym);
			if(i==0){
				perror("send");
				return ;
			}
			struct list_entry *tmp2=tmp->next;
			void *temp=remove_elem(l,nw);
			if(temp){
				//surement inutile en fait car stocké nulle part
				struct ngb_entry *n=(struct ngb_entry *)temp;
				n->times_sent=n->times_sent+1;
				n->wait_time=wait_time(n->times_sent)+current_time;
				i=add_elem(l,n);
				if(i==0) return ;
				tmp=tmp2;
			}
			//Pb, pas normal
		}
		else{
			NEXTTIME=min(NEXTTIME,nw->wait_time);
			return;
		}

	}
}

void flood_messages(int soc,list flood){
	printf("--------------------------------\n");
	printf("liste d'inondation de taille %d\n",flood->length);
	for(struct list_entry *list=flood->first;list;list=list->next){
		struct flood_entry *f=(struct flood_entry *)list->content;
		flood_message_to_neighbours(soc,f);
	}
	printf("--------------------------------\n");

}

short compare_flood(void *c1, void *c2){
	struct flood_entry *f1=(struct flood_entry *)c1;
	struct flood_entry *f2=(struct flood_entry *)c2;
	return compare_d(f1->index,f2->index);
}

short compare_d(void *c1,void *c2){
	struct data_index *data=(struct data_index *)c1;
	struct data_index *data2=(struct data_index *)c2;
	return !(data->id==data2->id && data->nonce==data2->nonce);
}

short compare_n(void *c1,void *c2){
	struct neighbor *key1=(struct neighbor *)c1;
	struct neighbor *key2=(struct neighbor *)c2;
	for(int i = 0;i<16;i++){
		if(key1->ip[i] < key2->ip[i]) return -1;
		if(key1->ip[i] > key2->ip[i]) return 1;
	}

	if(key1->port < key2->port) return -1;
	if(key1->port > key2->port) return 1;
	return 0;
}

void print_on_screen(char *data, size_t data_size){
	char *rouge="\033[31m";
	char *blanc="\033[0m";
	printf("%s\n",rouge );
	write(1,data,data_size);
	printf("%s\n",blanc);
}

/*short compare_w(void *nw, void *nw2){
	struct neighbor_and_wait *n1=(struct neighbor_and_wait *)nw;
	struct neighbor_and_wait *n2=(struct neighbor_and_wait *)nw2;
	return compare_n(n1->neighbor,n2->neighbor);
}*/

int max(int x,int y){
  return (x<=y)?y:x;
}

int min(int x,int y){
	return (x<=y)?x:y;
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

