#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <arpa/inet.h>
#include "struct.h"
#include "abr.h"
#include "tlv.h"
#include "list.h"
#include "net_lib.h"
#include "peer.h"



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
short tlv_pad1(char *body,size_t bufsize){
	if(bufsize>0){
		*body = 0;
		return 1;
	}
	return 0;
}

short tlv_padN(char *body,size_t bufsize, u_int8_t length){
	if(bufsize>length+1){
		*body = 1;
		*(body+1)=length;
		memset(body+2,0,length);
		return 1;
	}
	return 0;
}

short tlv_short_hello(char *body,size_t bufsize, u_int64_t ID){
	if(bufsize>9){
		*body = 2;
		*(body+1)=8;
		memcpy(body+2,&ID,sizeof(ID));
		return 1;
	}
	return 0;
}

short tlv_long_hello(char *body,size_t bufsize, u_int64_t source_id,u_int64_t dest_id){
	if(bufsize>15){
		size_t source_size=sizeof(dest_id),dest_size=sizeof(dest_id);
		*body = 2;
		*(body+1)=16;
		memcpy(body+2,&source_id,source_size);
		memcpy((body+2+source_size),&dest_id,dest_size);
		return 1;
	}
	return 0;
}

short tlv_neighbour(char *body,size_t bufsize, u_int8_t ip[16],u_int16_t port){
	if(bufsize>17){
		size_t ip_size=16,port_size=sizeof(port);
		*body = 2;
		*(body+1)=18;
		memcpy(body+2,&ip,ip_size);
		memcpy(body+2+ip_size,&port_size,port_size);
		return 1;
	}
	return 0;
}

short tlv_data(char *body,size_t bufsize, u_int64_t ID,u_int8_t type,unsigned char *data,u_int8_t msg_size){
	u_int32_t nonce;
	size_t id_size=sizeof(ID),nonce_size=sizeof(nonce),type_size=sizeof(type);
	u_int8_t length=id_size+nonce_size+type_size+msg_size;
	if(bufsize>length+1){
		random_on_octets(&nonce,4);
		*body = 4;
		*(body+1)=length;
		memcpy(body+2,&ID,id_size);
		memcpy(body+2+id_size,&nonce,nonce_size);
		memcpy(body+2+id_size+nonce_size,&type,type_size);
		memcpy(body+2+id_size+nonce_size+type_size,&data,msg_size);
		return 1;
	}
	return 0;
}

short tlv_ack(char *body,size_t bufsize, u_int64_t ID,u_int32_t nonce){
	size_t id_size=sizeof(ID),nonce_size=sizeof(nonce);
	u_int8_t length=id_size+nonce_size;
	if(bufsize>length+1){
		*body = 5;
		*(body+1)=length;
		memcpy(body+2,&ID,id_size);
		memcpy(body+2+id_size,&nonce,nonce_size);
		return 1;
	}
	return 0;
}

short tlv_goaway(char *body,size_t bufsize, u_int8_t code,unsigned char *msg,u_int8_t msg_size){
	size_t code_size=sizeof(code);
	u_int8_t length=code_size+msg_size;
	if(bufsize>length+1){
		*body=6;
		*(body+1)=length;
		*(body+2)=code;
		memcpy(body+2+code_size,&msg,msg_size);
		return 1;
	}
	return 0;
	
}

short tlv_warning(char *body,size_t bufsize, unsigned char *msg,u_int8_t msg_size){
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
int padN(char * tlv,u_int8_t length,struct neighbor peer){
	return 1;
}

int hello(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	//insérer dans la table de voisins et noter la date de réception
	//struct neighbor *key=sockaddr6_to_neighbor();
	//struct ident *val=create_ident()
	//récupérer le source_id et la data courante
	time_t current=time(0);
	struct ident val;
	u_int64_t source_id,dest_id;
	memcpy(&(source_id),tlv,8);
	val.ID=source_id;
	val.last_hello=current;
	if(length==16){
		memcpy(&dest_id,tlv+8,8);
		if(dest_id==ID){
			val.last_hello_long=current;
			NEIGHBORS=add_neighbor(NEIGHBORS,&peer,&val);
			return 1;
		}
		return 1;
	}
	NEIGHBORS=add_neighbor(NEIGHBORS,&peer,&val);
	return 1;
}

int neighbour(int soc,char * tlv,u_int8_t length,struct neighbor peer){
	struct neighbor key;
	memcpy(&key.ip,tlv,16);
	memcpy(&key.port,tlv+16,2);
	//inserer l'adresse contenue dans le tlv à la liste des voisins potentiels
	POTENTIAL=add_neighbor(POTENTIAL,&key,NULL);
	return 1;
	//return 0;
}

//On doit afficher les données recues dans le groupe de discussion si elles sont du bon format, juste les inonder sinon
int data(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	//si le type est 220, le truc de Alexandre et tristan, on sait que c'est un sous message, 
	//On crée une liste pour les messages, et chacun est un tableau de char *, on vérifie s'il est deja dans la liste, on le rajoute dans le tableau, sinon on crée un nouveau noeud,
	//type:220, nonce du message global sur 4 octets, type de la donnée sur un octet,taille du message en octet N sur 2 octet, position du messsage sur un octet, 
	//ptet une hashmap ouais, ID le nonce vu qu'il est unique, on devra tjrs juste récupérer le message à l'ordre truc, quand on le recoit on doit augmenter le compteur de nombre de messages restants et si on atteint la taille du t
	//map.get(nonce)[indice]=le nouveau message, ou on met a jour le nombre ?, ah la valeur va etre le tableau de messages et le nombre de messages lus
	//jeter les messages incomplets depuis trop longtemps aussi, stocker date de début de réception du message, si plus de 2/5 minutes, erreur et suppression
	//en vrai peut etre que juste un char * serait suffisant, vu qu'on a le numero de l'octet , suffit de faire un memset à partir de cet octet
	//taille du sous message: tlv.length-13 (ID sur 8 octets+ nonce sur 4 +type ) -9(4 nonce+ 1 type+ 2 +2)
	struct data_index index;
	memcpy(&index.ID,tlv,8);
	memcpy(&index.nonce,tlv+8,4);
	//On récupère la liste des voisins à inonder associée à la donnée
	struct flood_entry *flood=get_flood(DATAF,&index);
	//Si vide
	//envoyer un acquittement
	char body[13];
	if(tlv_ack(body,12,index.ID,index.nonce)){
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
	if(flood){

	}
	else{
		//Ici il ne faut pas mettre l'émetteur dans la liste de personnes à inonder
		struct list_entry *symmetric=get_symmetrical(NEIGHBORS);
		symmetric=remove_node(symmetric,&peer);
		//on reconstruit le message et on le met dans la struct pour l"envoyer plus tard
		//rajouter caractère de fin de ligne ? +1 pour type 4
		char msg[length+2];
		msg[0]=4;
		msg[1]=length;
		memcpy(msg+2,tlv,length);
		add_entry(DATAF,&index,msg,symmetric);
		//Peut etre ajouter seulement si pas déja dans la liste à inonder ?
	}
	if(*(tlv+13)==0){
		//afficher le message
		write(1,tlv+14,length-13);
	}
	//Afficher le message_h
	//verifier dans la liste
	//print_message_h
	//send_message_h to voisins_symetric
	//printf("%s\n", data);

	//verifier si id_nonce dans la liste des data
	//Sinon l'afficher
	//Juste inonder;
	//flood()
	//faire des free et des cleans ici
	return 1;
}

int ack(char *tlv,u_int8_t length,struct neighbor peer){
	//sortir de la liste des personnes à inonder
	struct data_index index;
	memcpy(&index.ID,tlv,8);
	memcpy(&index.nonce,tlv+8,4);
	struct flood_entry *flood=get_flood(DATAF,&index);
	if(flood){
		flood->sym_neighbors=remove_node(flood->sym_neighbors,&peer);
		return 1;
	}
	//pas sur
	return 1;
}

int goaway(int soc,char *tlv,u_int8_t length,struct neighbor peer){
	//Marquer l'émetteur comme voisin non symétrique ou le supprimer
	NEIGHBORS=remove_neighbor(&peer,NEIGHBORS);
	return 1;
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
	//struct message_h *msg=obtain_message_h(body,buf_t);
	//mettre un null à la fin de la liste de tlv, ou stocker la taille quelque part
	//Dans quel cas il faut créer un objet msg?
	int pos=0;
	u_int16_t body_length=ntohs(msg->body_length);
	char tlv[MAX_SIZE];
	while(pos<body_length){
		u_int8_t type=(u_int8_t)msg->body[pos];
		u_int8_t length=(u_int8_t)(type==0)?0:(u_int8_t)msg->body[pos+1];
		if(type>=0 && type<NB_TLV){
			memcpy(tlv,&msg->body[pos+2],length);
			if(handle_tlv[type](soc,tlv,length,rcpt)){

			}
			else{
				//perror("tlv")
			}
		}
		pos+=length;
	}
}


