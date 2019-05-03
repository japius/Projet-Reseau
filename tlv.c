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
	random_on_octets(&nonce,sizeof(u_int32_t));
	size_t id_size=sizeof(id),nonce_size=sizeof(nonce),type_size=sizeof(type);
	u_int8_t length=id_size+nonce_size+type_size+msg_size;
	if(bufsize>length+1){
		random_on_octets(&nonce,4);
		*body = 4;
		*(body+1)=length;
		memcpy(body+2,&id,id_size);
		memcpy(body+2+id_size,&nonce,nonce_size);
		memcpy(body+2+id_size+nonce_size,&type,type_size);
		memcpy(body+2+id_size+nonce_size+type_size,&data,msg_size);
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
		return 1;
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
		memcpy(body+2+code_size,&msg,msg_size);
		return length+1;
	}
	return -1;
	
}

int tlv_warning(unsigned char *body,size_t bufsize, unsigned char *msg,u_int8_t msg_size){
	if(bufsize>msg_size+1){
		*body=7;
		*(body+1)=msg_size;
		memcpy(body+2,&msg,msg_size);
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
			add_neighbor(&peer,&val);
			printf("Nouveau voisin : \n");
			//print_tree(NEIGHBORS);
			return 1;
		}
		return 1;
	}
	add_neighbor(&peer,&val);
	//print_tree(NEIGHBORS);
	return 1;
}

int neighbour(int soc,char * tlv,u_int8_t length,struct neighbor peer){
	struct neighbor key;
	memcpy(key.ip,tlv,16);
	memcpy(&key.port,tlv+16,2);
	key.port=ntohs(key.port);
	//inserer l'adresse contenue dans le tlv à la liste des voisins potentiels
	add_potential(&key,NULL);
	return 1;
	//return 0;
}

//si le type est 220, le truc de Alexandre et tristan, on sait que c'est un sous message, 
	//On crée une liste pour les messages, et chacun est un tableau de char *, on vérifie s'il est deja dans la liste, on le rajoute dans le tableau, sinon on crée un nouveau noeud,
	//type:220, nonce du message global sur 4 octets, type de la donnée sur un octet,taille du message en octet N sur 2 octet, position du messsage sur un octet, 
	//ptet une hashmap ouais, ID le nonce vu qu'il est unique, on devra tjrs juste récupérer le message à l'ordre truc, quand on le recoit on doit augmenter le compteur de nombre de messages restants et si on atteint la taille du t
	//map.get(nonce)[indice]=le nouveau message, ou on met a jour le nombre ?, ah la valeur va etre le tableau de messages et le nombre de messages lus
	//jeter les messages incomplets depuis trop longtemps aussi, stocker date de début de réception du message, si plus de 2/5 minutes, erreur et suppression
	//en vrai peut etre que juste un char * serait suffisant, vu qu'on a le numero de l'octet , suffit de faire un memset à partir de cet octet
	//taille du sous message: tlv.length-13 (ID sur 8 octets+ nonce sur 4 +type ) -9(4 nonce+ 1 type+ 2 +2)
//On doit afficher les données recues dans le groupe de discussion si elles sont du bon format, juste les inonder sinon
int data(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	struct data_index index;
	memcpy(&index.id,tlv,8);
	memcpy(&index.nonce,tlv+8,4);
	//envoyer un acquittement
	unsigned char body[13];
	if(tlv_ack(body,12,index.id,index.nonce)){
		//créer le message_h et faire un send message_h
		struct message_h msg;
		msg.magic=93;
		msg.version=2;
		msg.body_length=14;
		memcpy(msg.body,body,12);
		send_message(soc,&msg,18,peer);
	}
	else{
		perror("tlv");
		return 0;
		//exit(EXIT_FAILURE);
	}
	//On récupère la liste des voisins à inonder associée à la donnée
	struct flood_entry *flood=get(DATAF,&index);
	//Si non vide
	if(flood){
			//retirer l'émetteur de la liste de voisins à inonder
		remove_neighbor_from_flood(&index,&peer);
		//inonder
	}
	else{
		//Ici il ne faut pas mettre l'émetteur dans la liste de personnes à inonder
		list symmetric=get_symmetrical(NEIGHBORS);
		struct ngb_entry ngb_ent;
		ngb_ent.sym=&peer;
		void *tmp=remove_elem(symmetric,&ngb_ent);
		if(tmp!=NULL) free(tmp);
		//symmetric=remove_node(symmetric,&peer);
		//on reconstruit le message et on le met dans la struct pour l"envoyer plus tard
		//rajouter caractère de fin de ligne ? +1 pour type 4
		char msg[PMTU];
		msg[0]=4;
		msg[1]=length;
		memcpy(msg+2,tlv,length);
		struct flood_entry flood;
		flood.index=&index;
		flood.data=msg;
		flood.sym_neighbors=symmetric;
		if(!add_elem(DATAF,&flood)){
			return 0;
		}
	}
	if(*(tlv+13)==0){
		//afficher le message
		write(1,tlv+14,length-13);
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
	return remove_neighbor(&peer);
}


int warning(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	//juste lire et passer à autre chose
	char buf[MAX_SIZE];
	memcpy(buf,tlv,length);
	tlv[length]=0; 
	printf("Warning: %s\n", buf);
	return 1;
}



void handle_message_h(int soc,struct message_h *msg,size_t buf_t,struct neighbor rcpt){
	int pos=0;
	if(msg->magic!=93 || msg->version!=2) return ;
	u_int16_t body_length=ntohs(msg->body_length);
	char tlv[MAX_SIZE];
	while(pos<body_length){
		u_int8_t type=(u_int8_t)msg->body[pos];
		if(type==0){
			pos++;
			continue;
		}
		u_int8_t length=(u_int8_t)msg->body[pos+1];
		if(length>buf_t-pos) return;
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


