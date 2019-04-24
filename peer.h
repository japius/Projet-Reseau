#ifndef PEER_H
#define PEER_H

#include "struct.h"
#include "hashmap.h"

extern u_int64_t id;
extern abr *potential;
extern abr *neighbors;
extern struct flood *data;

/*typedef struct peer peer;

//rendre accessible dans tous les fichiers
struct peer{
	u_int64_t id;
	hashmap *potential,*neighbors,*data;
	//map des données
};*/




#endif