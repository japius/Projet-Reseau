#ifndef PEER_H
#define PEER_H

#include "struct.h"
#include "list.h"
#include "abr.h"

#define JCH 1212

u_int64_t ID;
tree *POTENTIAL;
tree *NEIGHBORS;
struct flood *DATAF;
int NEXTTIME;
//Mettre une liste ou une struct iov
int NB_SYMETRICAL;
/*typedef struct peer peer;

//rendre accessible dans tous les fichiers
struct peer{
	u_int64_t ID;
	flood *POTENTIAL,*NEIGHBORS,*data;
	//map des données
};*/




#endif