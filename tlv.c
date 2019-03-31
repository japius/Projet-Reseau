#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
#include "tlv.h"


char *tlv_types[NB_TLV]={
  "pad1","padN","hello","neighbour","data","ack","goaway","warning"
};

int (*handle_tlv[NB_TLV])(TLV *)={
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

char **split(int length,char *data,char delim){
	char *res[2];
	res[0]=memchr(data,delim,length);
	if(data==NULL) return NULL;
	memset(&res[1],0,sizeof(res[1]));
	printf("Taille: %d",res[0]-data);
	memcpy(&(res[1]),&data,res[0]-data);
	return res;
}

TLV *tlv_init(u_int8_t type,u_int8_t length,unsigned char *body){
	TLV *tlv=malloc(sizeof(struct TLV));
	if(tlv==NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	tlv->type=type;
	tlv->length=length;
	//tlv->body= {0};
	//tlv->body=body;
	if(body) memcpy(&(tlv->body),&body,sizeof(body));
	else tlv->body=NULL;
	return tlv;
}

struct message *message(char magic,char version,u_int16_t body_length,TLV ** list){
	struct message *msg=malloc(sizeof(struct message));
	if(msg==NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	msg->magic=magic;
	msg->version=version;
	msg->body_length=body_length;
	msg->body=list;
	return msg;
	
}

//Fonctions pour créer les tlv avec les paramètres en argument
TLV *tlv_pad1(){
	return tlv_init(0,NULL,NULL);
}
TLV *tlv_padN(u_int8_t length){
	unsigned char *body;
	memset(&body,0,sizeof(body));
	return tlv_init(1,length,body);
}
TLV *tlv_short_hello(u_int64_t id){
	unsigned char *body;
	memcpy(&(body),&id,sizeof(id));
	return tlv_init(2,8,body);

}
TLV *tlv_long_hello(u_int64_t source_id,u_int64_t dest_id){
	unsigned char *body;
	size_t source_size=sizeof(dest_id),dest_size=sizeof(dest_id);
	memcpy(&(body),&source_id,source_size);
	memcpy((body+source_size),&dest_id,dest_size);
	return tlv_init(2,16,body);

}

TLV *tlv_neighbour(unsigned char ip[16],u_int16_t port){
	unsigned char *body;
	size_t ip_size=sizeof(ip),port_size=sizeof(port);
	memcpy(&(body),&ip,ip_size);
	memcpy(body+ip_size,&port_size,port_size);
	return tlv_init(3,ip_size+port_size,body);
}

TLV *tlv_data(u_int64_t id,u_int8_t type,unsigned char *data){
	unsigned char *body;
	u_int32_t nonce=0;
	size_t id_size=sizeof(id),nonce_size=sizeof(nonce),type_size=sizeof(type),data_size=sizeof(data);
	u_int8_t length=id_size+nonce_size+type_size+data_size;
	memcpy(&(body),&id,id_size);
	memcpy(body+id_size,&nonce,nonce_size);
	memcpy(body+id_size+nonce_size,&type,type_size);
	memcpy(body+id_size+nonce_size+type_size,&data,data_size);
	return tlv_init(4,length,body);
}

TLV *tlv_ack(u_int64_t id,u_int32_t nonce){
	unsigned char *body;
	size_t id_size=sizeof(id),nonce_size=sizeof(nonce);
	u_int8_t length=id_size+nonce_size;
	memcpy(&(body),&id,id_size);
	memcpy(body+id_size,&nonce,nonce_size);
	return tlv_init(5,length,body);

}

TLV *tlv_goaway(u_int8_t code,unsigned char *msg){
	unsigned char *body;
	size_t code_size=sizeof(code),msg_size=sizeof(msg);
	u_int8_t length=code_size+msg_size;
	memcpy(&(body),&code,code_size);
	memcpy(body+code_size,&msg,msg_size);
	return tlv_init(6,length,body);


}

TLV *tlv_warning(unsigned char *msg){
	unsigned char *body;
	u_int8_t length=sizeof(msg);
	memcpy(&(body),&msg,length);
	return tlv_init(7,length,body);

}

//Fonctions de gestion de la réception d'un TLV

int pad1(TLV *tlv){
	return 0;
}
int padN(TLV *tlv){
	return 0;
}
int hello(TLV *tlv){
	if(tlv->length==8){

	}
	else{

	}
	return 0;
}
int neighbour(TLV *tlv){
	return 0;
}

//On doit afficher les données recues dans le groupe de discussion si elles sont du bon format, juste les inonder sinon
int data(TLV *tlv){
	char delim=':';
	char type=*(tlv->body+8+4);
	char *data=tlv->body+8+4+1;
	if(type==0){
		char msg=split(tlv->length-(8+4+1),data,'.');
		if(!msg){
			//Afficher le message
			printf("%s\n", data);
		}
	}
	//Juste inonder;
	return 0;
}
int ack(TLV *tlv){
	//chercher le data correspondant et recopier les champs correspondant (dernier data recu ?)
	return 0;
}

int goaway(TLV *tlv){
	//Marquer l'émetteur comme voisin non symétrique
	return 0;
}
int warning(TLV *tlv){
	//juste lire et passer à autre chose
	printf("Warning: %s\n", tlv->body);
	return 0;
}


int short_tlv_hello(void *buf,size_t buf_t,u_int64_t id){
	struct TLV *tlv = (struct TLV *)buf;
	if(buf_t<10) return 0;
	tlv->type='2';
	tlv->length='8';
	//memcpy(&(tlv->id),&id,sizeof(id));
	return 1;

}


struct message *create_message(void *buffer,size_t buf_t){
	unsigned char *buf=(unsigned char *)buffer;
	char magic=*buf;
	char version=*(buf+1);
	if(magic!='93' || version!='2') return NULL;
	u_int16_t body_length;
	memcpy(&(body_length),buf+2,2);
	unsigned char *pos=buf+4,i=0;
	TLV **list;
	while(pos<=(buf+4+body_length)){
		if(i<NB_TLV){
			//On récupère la fonction du tableau à l'indice le type du TLV
			u_int8_t type=*pos;
			//faire un atoi plutot 
			u_int8_t length=(type==0)?NULL:(u_int8_t)*(pos+1);
			unsigned char *body=NULL;
			if (type!=0) memcpy(&(body),(pos+2),length);
			list[i]=tlv_init(type,length,body);
			i++;
		}
		pos=pos+*(pos+1);
	}
	return message(magic,version,body_length,list);
	
}


//int make_tlv(tlv *)
/*
TLV *tlv_pad1(){
	return tlv_init(0,NULL);
}

TLV *tlv_padN(u_int8_t length){
	TLV tlv=tlv_init(1,length);
	memset(&tlv.body,0,sizeof(tlv.body));
}

TLV *short_tlv_hello(u_int64_t id){
	TLV tlv=tlv_init(2,8);
	memcpy(&(tlv.body),&id,sizeof(id));
	return tlv;
}

TLV *long_tlv_hello(u_int64_t source_id,u_int64_t dest_id){
	size_t source_size=sizeof(dest_id),dest_size=sizeof(dest_id);
	TLV tlv=tlv_init(2,16);
	memcpy(&(tlv.body),&source_id,source_size);
	memcpy(&(tlv.body+source_size-1),&dest_id,dest_size);
	return tlv;
}

TLV *neighbour(unsigned char[16] ip,u_int16_t port){
	size_t ip_size=sizeof(ip),port_size=sizeof(port);
	TLV tlv=tlv_init(3,18);
	memcpy(&(tlv.body),&ip,ip_size);
	memcpy(&(tlv.body+ip_size-1),&port_size,port_size);
	return tlv;
}

TLV *data(u_int64_t id,u_int8_t type,unsigned char *data){
	u_int32_t nonce=0;
	size_t id_size=sizeof(id),nonce_size=sizeof(nonce),type_size=sizeof(type),data_size=sizeof(data);
	u_int8_t length=id_size+nonce_size+type_size+data_size;
	TLV *tlv=tlv_init(4,length);
	memcpy(&(tlv.body),&id,id_size);
	memcpy(&(tlv.body+id_size-1),&nonce,nonce_size);
	memcpy(&(tlv.body+id_size+nonce_size-1),&type,type_size);
	memcpy(&(tlv.body+id_size+nonce_size+type_size-2),&data,data_size);
	return tlv;
}

TLV *ack(u_int64_t id,u_int32_t nonce){
	size_t id_size=sizeof(id),nonce_size=sizeof(nonce);
	TLV *tlv=tlv_init(5,12);
	memcpy(&(tlv.body),&id,id_size);
	memcpy(&(tlv.body+id_size-1),&nonce,nonce_size);
}

TLV *goaway(u_int8_t code,unsigned char *msg){
	size_t code_size=sizeof(code),msg_size=sizeof(msg);
	u_int8_t length=code_size+msg_size;
	TLV *tlv=tlv_init(6,length);
	memcpy(&(tlv.body),&code,code_size);
	memcpy(&(tlv.body+code_size-1),&msg,msg_size);
	return tlv;

}

TLV *warning(unsigned char *msg){
	u_int8_t length=sizeof(msg);
	TLV *tlv=tlv_init(7,length);
	memcpy(&(tlv.body),&msg,length);
}


TLV *init_tlv(u_int8_t type,u_int8_t length){
	TLV *tlv=malloc(sizeof(struct TLV));
	if(tlv==NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	tlv->type=type;
	tlv->length=length;
	tlv->body=NULL;

}

TLV *init_tlv(u_int8_t type,u_int8_t length,char *body){
	TLV *tlv=malloc(sizeof(struct TLV));
	if(tlv==NULL){
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	tlv->type=type;
	tlv->length=length;
	memcpy(&(tlv->body),&body,length);
	return tlv;

}

//Fonctions pour séparer les différents élements d'un tlv reçu
TLV *pad1(unsigned char *tlv){
	return tlv_pad1();
}
TLV *padN(unsigned char *tlv){
	return tlv_padN(*(tlv+8))
}*/
