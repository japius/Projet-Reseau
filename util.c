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
	return rand()%(b-a) +a;
}


void handle_inactive(int soc,struct data_index *data,struct neighbor  *sym){
	//Send go_away;
	char goaway[26]="Wesh, t'es où ? Bref,bye";
	char tlv2[26+3];
		//Supprimer de la liste des voisins
	int tmp=tlv_goaway(tlv2,28,2,goaway,25);
	struct message_h msg;
	msg.magic=93;
	msg.version=2;
	msg.body_length=tmp;
	send_message(soc,&msg,tmp+4,*sym);
		//ici le sendto
	remove_neighbor(sym);
	remove_neighbor_from_flood(data,sym);
}


int wait_time(int times_sent){
	return get_seconds()+rand_a_b((int)pow(2.0,times_sent-1),(int)pow(2.0,times_sent));
}

struct neighbor_and_wait make(struct neighbor *n,int *times_sent){
	struct neighbor_and_wait id;
	id.neighbor=n;
	//pas sure que ca marche
	id.times_sent=times_sent;
	id.wait_time=wait_time(*times_sent);
	return id;
}

struct list *get_wait_list(struct flood_entry *flood){
	struct list_entry *tmp=flood->sym_neighbors->first;
	struct list *list=malloc(sizeof(struct list));
	init_list(list,compare_w,sizeof(struct neighbor_and_wait));
	while(tmp){
		struct ngb_entry *l=(struct ngb_entry *)tmp->content;
		//faire un malloc pour 
		struct neighbor_and_wait id=make(l->sym,&(l->times_sent));
		short i=add_elem(list,&id);
		if(i==0){
			free(list);
			return NULL;
		}
		tmp=tmp->next;
	}
	return list;
}

int send_data(int soc, char *tlv,struct neighbor *key){
	int body_length=tlv[1],msg_length=body_length+4;
	struct message_h msg;
	msg.magic=93;
	msg.version=2;
	msg.body_length=htons(msg_length);
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
	list l=get_wait_list(flood);
	if(l==NULL) return ;
	struct list_entry *tmp=l->first;
	while(tmp){
		struct neighbor_and_wait *nw=(struct neighbor_and_wait *)tmp->content;
		if(*(nw->times_sent)==5){
			struct list_entry *tmp2=tmp->next;
			handle_inactive(soc,flood->index,nw->neighbor);
			tmp=tmp2;
			continue;

		}
		if(current_time>=nw->wait_time){
			int i=send_data(soc,flood->data,nw->neighbor);
			if(i==0){
				perror("send");
				return ;
			}
			struct list_entry *tmp2=tmp->next;
			void *tmp=remove_elem(l,nw);
			if(tmp){
				//surement inutile en fait car stocké nulle part
				struct neighbor_and_wait *n=(struct neighbor_and_wait *)tmp;
				*(n->times_sent)=*(n->times_sent)+1;
				n->wait_time=wait_time(*(nw->times_sent));
				int i=add_elem(l,n);
				if(i==0) return ;
				tmp=tmp2;
			}
			//Pb, pas normal
		}
		else{
			NEXTTIME=min(NEXTTIME,nw->wait_time);
			break;
		}

	}
}

void flood_message(int soc,list flood){
	for(struct list_entry *list=flood->first;!list;list=list->next){
		struct flood_entry *f=(struct flood_entry *)list->content;
		flood_message_to_neighbours(soc,f);
	}

}


/*void flood_message(int soc,struct flood_entry *flood){
	struct wait_list *list=get_wait_time(flood);
	flood_message_to_neighbour(soc,)

}


void flood_message(int soc,struct flood_entry *flood){
	//faire un fork pour leur envoyer en même temps ? ou select c'est suffisant ?
	int *length=&length(flood->sym_neighbors);
	struct neighbor_and_wait nw[]=get_wait_time(flood,flood->sym_neighbors,length);
	for(struct list *list=flood->sym_neighbors;!list;list=list->next){
		flood_message_to_neighbour(soc,flood->data,nw,*length);
	}
}*/



short compare_d(struct data_index *data,struct data_index *data2){
	return (data->id==data2->id && data->nonce==data2->nonce);
}

short compare_n(struct neighbor *key1,struct neighbor *key2){
	for(int i = 0;i<16;i++){
		if(key1->ip[i] < key2->ip[i]) return -1;
		if(key1->ip[i] > key2->ip[i]) return 1;
	}

	if(key1->port < key2->port) return -1;
	if(key1->port > key2->port) return 1;
	return 0;
}

short compare_w(void *nw, void *nw2){
	struct neighbor_and_wait *n1=(struct neighbor_and_wait *)nw;
	struct neighbor_and_wait *n2=(struct neighbor_and_wait *)nw2;
	return compare_n(n1->neighbor,n2->neighbor);
}

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

