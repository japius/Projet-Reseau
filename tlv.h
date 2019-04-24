#ifndef TLV_H
#define TLV_H
#define NB_TLV 9
#include "struct.h"
//typedef int (*tlv_func)(TLV *);
extern char *tlv_types[NB_TLV];
extern int (*handle_tlv[NB_TLV])(char *,u_int8_t,struct neighbor );

int pad1(char *tlv,u_int8_t length,struct neighbor key);
int padN(char *tlv,u_int8_t length,struct neighbor key);
int hello(char *tlv,u_int8_t length,struct neighbor key);
int neighbour(char *tlv,u_int8_t length,struct neighbor key);
int data(char *tlv,u_int8_t length,struct neighbor key);
int ack(char *tlv,u_int8_t length,struct neighbor key);

int goaway(char *tlv,u_int8_t length,struct neighbor key);
int warning(char *tlv,u_int8_t length,struct neighbor key);

#endif