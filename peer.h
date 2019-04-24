#ifndef PEER_H
#define PEER_H

#include "struct.h"
#include "list.h"
#include "abr.h"

extern u_int64_t id;
extern tree *potential;
extern tree *neighbors;
extern struct flood *dataf;

/*typedef struct peer peer;

//rendre accessible dans tous les fichiers
struct peer{
	u_int64_t id;
	hashmap *potential,*neighbors,*data;
	//map des données
};*/




#endif