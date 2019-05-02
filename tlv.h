#ifndef TLV_H
#define TLV_H
#define NB_TLV 8
#include "struct.h"
//typedef int (*tlv_func)(TLV *);
extern char *tlv_types[NB_TLV];
extern int (*handle_tlv[NB_TLV])(int soc,char *,u_int8_t,struct neighbor );

int tlv_pad1(unsigned char *body,size_t bufsize);
int tlv_padN(unsigned char *body,size_t bufsize, u_int8_t length);

int tlv_short_hello(unsigned char *body,size_t bufsize, u_int64_t id);

int tlv_long_hello(unsigned char *body,size_t bufsize, u_int64_t source_id,u_int64_t dest_id);

int tlv_neighbour(unsigned char *body,size_t bufsize, struct neighbor ngb);

int tlv_data(unsigned char *body,size_t bufsize, u_int64_t id,u_int8_t type,unsigned char *data,u_int8_t msg_size);

int tlv_ack(unsigned char *body,size_t bufsize, u_int64_t id,u_int32_t nonce);

int tlv_goaway(unsigned char *body,size_t bufsize, u_int8_t code,unsigned char *msg,u_int8_t msg_size);

int tlv_warning(unsigned char *body,size_t bufsize, unsigned char *msg,u_int8_t msg_size);


int pad1(int soc,char *tlv,u_int8_t length,struct neighbor key);
int padN(int soc,char *tlv,u_int8_t length,struct neighbor key);
int hello(int soc,char *tlv,u_int8_t length,struct neighbor key);
int neighbour(int soc,char *tlv,u_int8_t length,struct neighbor key);
int data(int soc,char *tlv,u_int8_t length,struct neighbor key);
int ack(int soc,char *tlv,u_int8_t length,struct neighbor key);

int goaway(int soc,char *tlv,u_int8_t length,struct neighbor key);
int warning(int soc,char *tlv,u_int8_t length,struct neighbor key);

void handle_message_h(int soc,struct message_h *msg,size_t buf_t,struct neighbor rcpt);
#endif