#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "struct.h"
#include "abr.h"
#include "tlv.h"
#include "list.h"
#include "net_lib.h"
#include "peer.h"
#include "genlist.h"
#include "util.h"



char *tlv_types[NB_TLV]={
  "pad1","padN","hello","neighbour","data","ack","goaway","warning"
};

int (*handle_tlv[NB_TLV])(int soc,char *,u_int8_t,struct neighbor )={
  &pad1,
  &padN,
  &hello,
  &neighbour,
  &data,
  &ack,
  &goaway,
  &warning
};

int nb_tlv(){
  return sizeof(tlv_types)/sizeof(tlv_types[0]);
}


//Fonction pour créer le message qu'on veut

//Fonction pour envoyer un certain message

//Fonctions pour créer les tlv avec les paramètres en argument
int tlv_pad1(unsigned char *body,size_t bufsize){
	if(bufsize>0){
		*body = 0;
		return 1;
	}
	return -1;
}

int tlv_padN(unsigned char *body,size_t bufsize, u_int8_t length){
	if(bufsize>length+1){
		*body = 1;
		*(body+1)=length;
		memset(body+2,0,length);
		return length+2;
	}
	return -1;
}

int tlv_short_hello(unsigned char *body,size_t bufsize, u_int64_t id){
	if(bufsize>9){
		*body = 2;
		*(body+1)=8;
		memcpy(body+2,&id,sizeof(id));
		return 10;
	}
	return -1;
}

int tlv_long_hello(unsigned char *body,size_t bufsize, u_int64_t source_id,u_int64_t dest_id){
	if(bufsize>15){
		size_t source_size=sizeof(dest_id),dest_size=sizeof(dest_id);
		*body = 2;
		*(body+1)=16;
		memcpy(body+2,&source_id,source_size);
		memcpy((body+2+source_size),&dest_id,dest_size);
		return 18;
	}
	return -1;
}

int tlv_neighbour(unsigned char *body,size_t bufsize, struct neighbor ngb){
	if(bufsize>21){
		*body = 3;
		*(body+1)=18;
		memcpy(body+2,&ngb.ip,sizeof(ngb.ip));
		u_int16_t port = htons(ngb.port);
		memcpy(body+2+sizeof(ngb.ip),&port,sizeof(ngb.port));
		return 20;
	}
	return -1;
}

int tlv_data(unsigned char *body,size_t bufsize, u_int64_t id,u_int8_t type,unsigned char *data,u_int8_t msg_size){
	u_int32_t nonce;
	size_t id_size=sizeof(id),nonce_size=sizeof(nonce),type_size=sizeof(type);
	u_int8_t length=id_size+nonce_size+type_size+msg_size;
	if(bufsize>length+1){
		random_on_octets(&nonce,4);
		*body = 4;
		*(body+1)=length;
		memcpy(body+2,&id,id_size);
		memcpy(body+2+id_size,&nonce,nonce_size);
		memcpy(body+2+id_size+nonce_size,&type,type_size);
		memcpy(body+2+id_size+nonce_size+type_size,data,msg_size);
		return msg_size+15;
	}
	return -1;
}

int tlv_ack(unsigned char *body,size_t bufsize, u_int64_t id,u_int32_t nonce){
	size_t id_size=sizeof(id),nonce_size=sizeof(nonce);
	u_int8_t length=id_size+nonce_size;
	if(bufsize>length+1){
		*body = 5;
		*(body+1)=length;
		memcpy(body+2,&id,id_size);
		memcpy(body+2+id_size,&nonce,nonce_size);
		return id_size+nonce_size+2;
	}
	return 0;
}

int tlv_goaway(unsigned char *body,size_t bufsize, u_int8_t code,unsigned char *msg,u_int8_t msg_size){
	size_t code_size=sizeof(code);
	u_int8_t length=code_size+msg_size;
	if(bufsize>length+1){
		*body=6;
		*(body+1)=length;
		*(body+2)=code;
		memcpy(body+2+code_size,msg,msg_size);
		return length+2;
	}
	return -1;
	
}

int tlv_warning(unsigned char *body,size_t bufsize, unsigned char *msg,u_int8_t msg_size){
	if(bufsize>msg_size+1){
		*body=7;
		*(body+1)=msg_size;
		memcpy(body+2,msg,msg_size);
		return 1;
	}
	return 0;
}

//Fonctions de gestion de la réception d'un TLV

int pad1(int soc,char * tlv,u_int8_t length,struct neighbor peer){
	return 1;
}
int padN(int soc,char * tlv,u_int8_t length,struct neighbor peer){
	return 1;
}

int hello(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	unsigned int current=get_seconds();
	struct ident val;
	u_int64_t source_id,dest_id;
	memcpy(&(source_id),tlv,8);
	val.id=source_id;
	val.last_hello=current;
	if(length==16){
		memcpy(&dest_id,tlv+8,8);
		if(dest_id==ID){
			val.last_hello_long=current;
			int rc = add_neighbor(&peer,&val);
			if(rc==1){
				remove_potential(&peer);
				send_hello_everyone(soc,NEIGHBORS);
			}

			//print_tree(NEIGHBORS);
			return 1;
		}
		return 1;
	}
	int rc = add_neighbor(&peer,&val);
	if(rc==1){
		remove_potential(&peer);
		send_hello_everyone(soc,NEIGHBORS);
	}
	return 1;
}

int neighbour(int soc,char * tlv,u_int8_t length,struct neighbor peer){
	struct neighbor key;
	memcpy(key.ip,tlv,16);
	memcpy(&key.port,tlv+16,2);
	key.port=ntohs(key.port);
	if(get_ident(NEIGHBORS,&key)) return 1;
	struct ident val = {0};
	val.last_hello = get_seconds();
	//inserer l'adresse contenue dans le tlv à la liste des voisins potentiels
	add_potential(&key,&val);
	return 1;
	//return 0;
}

int data(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	struct data_index index;
	memcpy(&index.id,tlv,8);
	memcpy(&index.nonce,tlv+8,4);
	struct message_h msg;
	msg.magic=93;
	msg.version=2;
	int size=0;
	size=tlv_ack(msg.body,PMTU-4,index.id,index.nonce);
	if(size>0){
		//créer le message_h et faire un send message_h
		msg.body_length=htons(size);
		int i=send_message(soc,&msg,size+4,peer);
	}
	//On récupère la liste des voisins à inonder associée à la donnée
	struct flood_entry fl={0};
	fl.index=&index;
	struct flood_entry *flood=get(&DATAF,&fl);
	//Si non vide
	if(flood){
			//retirer l'émetteur de la liste de voisins à inonder
		remove_neighbor_from_flood(&index,&peer);
		//inonder
	}
	else{
		add_message_to_flood(tlv,length,&peer);
		size=*(tlv+12);
		if(size==0){
		//afficher le message
			print_on_screen(tlv+13,length-13);
			return 1;
		}
		if(size==220){
			//return bigdata(soc,tlv,u_int8_t length,&peer);
		}
		else if(size==2){
			//gif
		}
		else if(size==3){
			//jpeg
		}
		else if(size==4){
			//png
		}
		else if(size==5){
			//svg
		}
	}
	
	return 1;
}

int ack(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	//sortir de la liste des personnes à inonder
	struct data_index index;
	memcpy(&index.id,tlv,8);
	memcpy(&index.nonce,tlv+8,4);
	//struct flood_entry *flood=get_flood(&index);
	//if(flood){
	remove_neighbor_from_flood(&index,&peer);
	return 1;
}

int goaway(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	//Marquer l'émetteur comme voisin non symétrique ou le supprimer
	printf("go away de type %d\n",tlv[0]);
	write(1,tlv+1,length-1);
	return remove_neighbor_everywhere(&peer);
}


int warning(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	//juste lire et passer à autre chose
	char buf[MAX_SIZE];
	memcpy(buf,tlv,length);
	tlv[length]=0; 
	printf("Warning: %s\n", buf);
	return 1;
}

//si le type est 220, le truc de Alexandre et tristan, on sait que c'est un sous message, 
	//On crée une liste pour les messages, et chacun est un tableau de char *, on vérifie s'il est deja dans la liste, on le rajoute dans le tableau, sinon on crée un nouveau noeud,
	//type:220, nonce du message global sur 4 octets, type de la donnée sur un octet,taille du message en octet N sur 2 octet, position du messsage sur un octet, 
	//ptet une hashmap ouais, ID le nonce vu qu'il est unique, on devra tjrs juste récupérer le message à l'ordre truc, 
//quand on le recoit on doit augmenter le compteur de nombre de messages restants et si on atteint la taille du t
	//map.get(nonce)[indice]=le nouveau message, ou on met a jour le nombre ?, ah la valeur va etre le tableau de messages et le nombre de messages lus
	//jeter les messages incomplets depuis trop longtemps aussi, stocker date de début de réception du message, si plus de 2/5 minutes, erreur et suppression
	//en vrai peut etre que juste un char * serait suffisant, vu qu'on a le numero de l'octet , suffit de faire un memset à partir de cet octet
	//taille du sous message: tlv.length-13 (ID sur 8 octets+ nonce sur 4 +type ) -9(4 nonce+ 1 type+ 2 +2)
//On doit afficher les données recues dans le groupe de discussion si elles sont du bon format, juste les inonder sinon

int bigdata(int soc,char *tlv,u_int8_t length,struct neighbor *peer){
	return 0;
	//metree certrains trucs dans des fonctions et créer la liste
	//on a une liste de struct big data indexée par nonce
	/*u_int32_t nonce=tlv[13];
	struct big_data *bg=get(data_list,&nonce);
	if(bg==NULL){
		//on crée et on rajoute dans la liste
		bg=malloc(sizeof(struct big_data));
		if(bg==NULL){
			perror("malloc");
			return 0;
		}
		bg->global_nonce=nonce;
		bg->total_length=tlv[18];
		bg->received_length=length-22;
		bg->start_time=get_seconds();
		bg->message=malloc(total_length);
		memcpy(bg->message,tlv+22,length-22);
		//ajouter à la listre
		return add_elem(data_list,bg);
	}
	else{
		if(get_seconds()-bg->start_time>=300){
			//Si on a attendu plus de trois secondes et le data est tjrs pas la, on le vire
			//envoyer un wzrning, trop lent ?
			void *tmp=remove_elem(data_list,bg);
			if(tmp){
				struct big_data *b=(struct big_data *)tmp;
				free(b->message);
				free(bg);
			}
			return 1;
		}
		if(bg->received_length+length-22>total_length){
			//il a pas respcté les critères 
			//envoyer un warnng par exemple;
			return 0;
		}
		memcpy(bg->message+received_length,tlv+22,length-22);
		bg->received_length+=length-22;
		if(bg->received_length==bg->total_length){
			//o a recu le msg totral, on peut l"afficher et le supprimer
			write(1,bg->message,total_length);
			void *tmp=remove_elem(data_list,bg);
			if(tmp){
				struct big_data *b=(struct big_data *)tmp;
				free(b->message);
				free(bg);
			}
			return 1;
		}
	}*/

}



void handle_message_h(int soc,struct message_h *msg,size_t buf_t,struct neighbor rcpt){
	int pos=0;
	if(buf_t<4) return;
	if(msg->magic!=93 || msg->version!=2) return ;
	u_int16_t body_length=ntohs(msg->body_length);
	if(body_length+4>buf_t) return;
	char tlv[MAX_SIZE];
	while(pos<body_length){
		u_int8_t type=(u_int8_t)msg->body[pos];
		if(type==0){
			pos++;
			continue;
		}
		u_int8_t length=(u_int8_t)msg->body[pos+1];
		if(length>body_length-pos) return;
		if(type>0 && type<NB_TLV){
			memcpy(tlv,&msg->body[pos+2],length);
			if(handle_tlv[type](soc,tlv,length,rcpt)){
				printf("J'ai géré un tlv %d\n",type);
			}
			else{
				//perror("tlv")
			}
		}
		pos+=length+2;
	}
	printf("Message terminé\n" );
}


