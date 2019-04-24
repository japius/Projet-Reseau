#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "struct.h"


typedef struct tree tree;

struct tree{
  struct neighbor *key;
  struct ident *val;
  struct  tree *left;
  struct tree *right;
};


tree *init_first();

tree *init(struct neighbor *key,struct ident *val,tree *left,tree *right);
//Pour ajouter un voisin s'il n"existe pas et le mettre à jour sinon
tree *add_neighbor(tree *t,struct neighbor *key,struct ident *val);
//Pour chercher un certain voisin
struct ident *get_ident(tree *t,struct neighbor *key);
//A modifier
short issymmetrical(struct ident *val);

struct list_entry *get_symmetrical(tree *t);

tree * remove_neighbor(struct neighbor *key, tree *t);

void clean(tree *t);

void print_tree(tree *abr);


