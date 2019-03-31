#ifndef TLV_H
#define TLV_H
#define NB_TLV 9
#include "struct.h"
//typedef int (*tlv_func)(TLV *);
extern char *tlv_types[NB_TLV];
extern int (*handle_tlv[NB_TLV])(TLV *);

int short_tlv_hello(void *buf,size_t buf_t,u_int64_t id);
int pad1(TLV *tlv);
int padN(TLV *tlv);
int hello(TLV *tlv);
int neighbour(TLV *tlv);
int data(TLV *tlv);
int ack(TLV *tlv);

int goaway(TLV *tlv);
int warning(TLV *tlv);

#endif