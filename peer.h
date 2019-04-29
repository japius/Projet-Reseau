#ifndef PEER_H
#define PEER_H

#include "struct.h"
#include "list.h"
#include "abr.h"

#define JCH 1212

extern u_int64_t ID;
extern tree *POTENTIAL;
extern tree *NEIGHBORS;
extern struct flood *DATAF;

/*typedef struct peer peer;

//rendre accessible dans tous les fichiers
struct peer{
	u_int64_t ID;
	hashmap *POTENTIAL,*NEIGHBORS,*data;
	//map des données
};*/




#endif