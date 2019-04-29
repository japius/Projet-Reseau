#ifndef TLV_H
#define TLV_H
#define NB_TLV 9
#include "struct.h"
//typedef int (*tlv_func)(TLV *);
extern char *tlv_types[NB_TLV];
extern int (*handle_tlv[NB_TLV])(int soc,char *,u_int8_t,struct neighbor );

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