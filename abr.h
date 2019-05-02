#ifndef ABR_H
#define ABR_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct.h"

typedef struct tree{
  struct neighbor *key;
  struct ident *val;
  struct  tree *left;
  struct tree *right;
}tree;




tree *init(struct neighbor *key,struct ident *val,tree *left,tree *right);

//Pour ajouter un voisin s'il n"existe pas et le mettre à jour sinon
short add_neighbor(struct neighbor *key,struct ident *val);

short add_potential(struct neighbor *key,struct ident *val);

//Pour chercher un certain voisin
struct ident *get_ident(tree *t,struct neighbor *key);

//A modifier
short issymmetrical(struct ident *val);

struct list *get_symmetrical(tree *t);

short  remove_neighbor(struct neighbor *key);

short  remove_potential(struct neighbor *key);

void clean(tree *t);

void print_tree(tree *abr);

#endif
