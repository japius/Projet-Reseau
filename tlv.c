#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include "struct.h"

int short_tlv_hello(void *buf,size_t buf_t,u_int64_t id){
	struct TLV *tlv = (struct TLV *)buf;
	if(buf_t<10) return 0;
	tlv->type=2;
	tlv->length=8;
	memcpy(&(tlv->id),&id,sizeof(id));
	return 1;
}

//int make_tlv(tlv *)