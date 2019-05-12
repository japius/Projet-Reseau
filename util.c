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

int add_to_neighbor_message(int soc,char *tlv,int length,struct neighbor *key){
	return 0;
	/*struct message_h *msg=key->msg;
	if(msg->body_length+4+length>=PMTU){
		send_message(soc,&msg,msg->body_length+4,*key);
		//retirer l'ancien message
		msg->body_length=0;
	}
	memcpy(msg->body+body_length,tlv,length);
	msg->body_length+=length;
	return 0;*/
}

void flood_message_to_neighbours(int soc,struct flood_entry *flood){
	int current_time=get_seconds();
	list l=flood->sym_neighbors;
	struct list_entry *tmp=l->first;
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
	for(struct list_entry *list=flood->first;list;list=list->next){
		struct flood_entry *f=(struct flood_entry *)list->content;
		flood_message_to_neighbours(soc,f);
	}

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
	write(FD_MAGIC_WRITE,data,data_size);
	if(data[data_size-1]!='\n'){
		write(1,"\n",1);
		//write(FD_MAGIC_WRITE,"\n",1);
	}
	printf("%s\n",blanc);
}

void disconnect(int sig){
	struct message_h msg;
	msg.magic=93;
	msg.version=2;
	u_int16_t size=tlv_goaway(msg.body,PMTU-4,1,"Bon j'me casse ciao!",20);;
	msg.body_length=htons(size);
	send_to_everyone(SOCKET,&msg,size+4,NEIGHBORS);
	send_to_everyone_now(SOCKET,NEIGHBORS);
	exit(sig);
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
	struct message_h msg,msg_hello;
	int rc=0;
	int sock=socket(AF_INET6,SOCK_DGRAM,0);
	if(sock<0){
		perror("sock");
		exit(EXIT_FAILURE);
	}
	struct sockaddr_in6 sin6;
	struct ipv6_mreq mreq;
	memset(&sin6,O,sizeof(sin6));
	memset(&mreq,O,sizeof(mreq));
	sin6.sin6_family=AF_INET6;
	sin6.sin6_port=htons(1212);

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
	rc=inet_pton(AF_INET6,"ff12:b456:dad4:cee1:4589:71de:a2ec:e66",&sin6.sin6_addr);
	if(rc < 0){
			fprintf(stderr, "Erreur lors de la conversion\n");
			return -1;
	}

	struct ifaddrs *ifaddr,*ifa;
	if(getifaddrs(ifaddr)==-1){
		perror("getifaddr");
		return 0;
	}
	//On rejoint le groupe sur toutes les interfaces accessibles et actives
	for(ifa=ifaddr;ifa!=NULL;ifa=ifa->ifa_next){
		if(ifa->ifa_addr==NULL) continue;
		if(ioctl(sock,SIOCGIFFLAGES,ifaddr)>0){
			perror("ioctl");
			return 0;
		}
		if((ifa.ifa_flags & IFF_UP & IFF_RUNNING) && !(ifa.ifa_name[0]=='l' && ifa.ifa_name[1]='o')){
			struct ipv6_mreq mreq;
			memset(&sin6,O,sizeof(sin6));
			memcpy(&mreq.ipv6mr_multiaddr,&sin6.sin6_addr,sizeof(sin6.sin6_addr));
			mreq.ipv6mr_interface=if_nametoindex(ifa.ifa_name);
			if(setsockopt(sock,IPPROTO_IPV6,IPV6_JOIN_GROUP,&mreq,sizeof(mreq))<0){
				perror("JOIN_GROUP");
				exit(EXIT_FAILURE)
			}
		}
	}
	//ON fait le receivfrim d'abord et ensuite le sendto ?
	struct sockaddr_in6 client;
	memset(&client,O,sizeof(client));
	struct message_h msg;
	rc=recvfrom(sock,&msg,sizeof(msg),0,&client,sizeof(client))
	//faire le sendto sur l'interface de la réponse;
	if(rc<0){

	}
	msg_hello.magic=93;
	msg_hello.version=2;
	int nb = tlv_short_hello(msg.body,PMTU-4,ID);
	msg.body_length = htons(nb);
	return send_to_everyone(fd,&msg,nb+4,POTENTIAL);
	//envoyer au client ou plutot, set le scope_id de sin6 à celui de client et send
	sin6.sin6_scope_id=client.sin6_scope_id;
	rc=sendto(sock,&msg,sizeof(msg),0,(struct sockaddr *)&sin6,sizeof(sin6));
	if(rc<0){

	}

}
*/

//LOL, IL TROLLE JULIUS
void hello_multicast_global(){

}

