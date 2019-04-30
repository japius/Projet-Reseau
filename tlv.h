#ifndef TLV_H
#define TLV_H
#define NB_TLV 9
#include "struct.h"
//typedef int (*tlv_func)(TLV *);
extern char *tlv_types[NB_TLV];
extern int (*handle_tlv[NB_TLV])(int soc,char *,u_int8_t,struct neighbor );

short tlv_pad1(char *body,size_t bufsize);
short tlv_padN(char *body,size_t bufsize, u_int8_t length);

short tlv_short_hello(char *body,size_t bufsize, u_int64_t id);

short tlv_long_hello(char *body,size_t bufsize, u_int64_t source_id,u_int64_t dest_id);

short tlv_neighbour(char *body,size_t bufsize, u_int8_t ip[16],u_int16_t port);

short tlv_data(char *body,size_t bufsize, u_int64_t id,u_int8_t type,unsigned char *data,u_int8_t msg_size);

short tlv_ack(char *body,size_t bufsize, u_int64_t id,u_int32_t nonce);

short tlv_goaway(char *body,size_t bufsize, u_int8_t code,unsigned char *msg,u_int8_t msg_size);

short tlv_warning(char *body,size_t bufsize, unsigned char *msg,u_int8_t msg_size);


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