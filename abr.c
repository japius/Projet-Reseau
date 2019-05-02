#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "struct.h"
#include "genlist.h"
#include "abr.h"
#include "peer.h"
#include "util.h"
#include "net_lib.h"


tree *init(struct neighbor *key,struct ident *val,tree *left,tree *right){
  tree *current=malloc(sizeof(tree));
  if(!current) return NULL;
  current->val=malloc(sizeof(struct ident));
  if(!current->val){
    free(current);
    return NULL;
  }
  if(val)memmove(current->val,val,sizeof(struct ident));
  current->key=malloc(sizeof(struct neighbor));
  if(!current->key){
    free(current->val);
    free(current);
    return NULL;
  }
  if(key)memmove(current->key,key,sizeof(struct neighbor));
  current->left=left;
  current->right=right;
  return current;
}

short add_neighbor_aux(tree *t,struct neighbor *key,struct ident *val){
  //if(t==NULL) return init(key,val,NULL,NULL);
  int comp=compare_n(t->key,key);
  if(comp>0){
    if(t->left==NULL){
      t->left=init(key,val,NULL,NULL);
      return 1;
    }
    return add_neighbor_aux(t->left,key,val);
  }
  if(comp<0){
    if(t->right==NULL){
      t->right=init(key,val,NULL,NULL);
      return 1;
    }
    return add_neighbor_aux(t->right,key,val);
  }    
  t->val->last_hello=val->last_hello;
  t->val->last_hello_long=val->last_hello_long;
  return 1;
}


short add_neighbor(tree *NEIGHBORS,struct neighbor *key,struct ident *val){
  if(NEIGHBORS==NULL){
    NEIGHBORS=init(key,val,NULL,NULL);
    //printf("Je modifie le NEIGHBORS %p\n",NEIGHBORS);
    return NEIGHBORS != 0;
  }
  return add_neighbor_aux(NEIGHBORS,key,val);
}

//Pour ajouter un voisin s'il n"existe pas et le mettre à jour sinon


//Pour chercher un certain voisin
struct ident *get_ident(tree *t,struct neighbor *key){
  if(t!=NULL){
    int comp=compare_n(t->key,key);
    if(comp==0) return t->val;
    else if(comp>0) return get_ident(t->left,key);
    return get_ident(t->right,key);
  }
  return NULL;
}

//A modifier
short issymmetrical(struct ident *val){
  int now=get_seconds();
  if(now-val->last_hello_long<120) return 1;
  return 0;
}

//Pour chercher tous les voisins symétriques
struct list_entry *get_symmetrical(tree *t){

    if(t!=NULL){
        if(issymmetrical(t->val)){
          if(t->left==NULL && t->right==NULL) return init_list_entry(t->key,0,NULL);
          if(t->left==NULL) return init_list_entry(t->key,0,get_symmetrical(t->right));
          if(t->right==NULL) return init_list_entry(t->key,0,get_symmetrical(t->left));
          struct list_entry *entry=init_list_entry(t->key,0,get_symmetrical(t->left));
          //return add_node(get_symmetrical(t->right),get_last(t->left));
          get_last(entry)->next=get_symmetrical(t->right);
          return entry;
        }
    }
    return NULL;;
}


/*struct list_entry *get_symmetrical(tree *t){
  if(t!=NULL){
      struct list_entry *entry=NULL;
      get_sym_aux(entry,tree);
      return entry;
  }
  return NULL;
}
void get_sym_aux(struct list_entry *entry,tree *t){
    if(issymmetrical(t->val)){
      if(entry==NULL){
        entry=init_list_entry(t->key,0,NULL);
        get_sym_aux(entry->next,)
      else entry->next=init_list_entry(t->key,0,NULL);
    }
}

struct list_entry *get_symmetrical(tree *t){
  if(t!=NULL){
    if(issymmetrical(t->val)){
      struct list_entry *sym=init_list_entry(t->key,0,NULL)
      struct list_entry *sym2=get_symmetrical(t->left),*sym3=get_symmetrical(t->right);

      if(sym2){
        sym2->next=sym;
        if(sym3){
          sym3->next=sym2;
          return sym3;
        }
        return sym2;
      }  
      if(sym3){
        sym3->next=sym;
        return sym3;
      }
      return sym;
    }
  }
  return NULL;
}*/

//Le nombre de voisins
int number_of_neighbors(tree *t){
  if(t==NULL) return 0;
  return 1+number_of_neighbors(t->left)+number_of_neighbors(t->right);
}

//Le nombre de voisins symétriques
int number_of_symmetrical(tree *t){
  struct list_entry *list=get_symmetrical(t);
  return length(list);
}

//Renvoie un pointeur vers le maximum du sous arbre de racine r
tree *maxUnder(tree *r){
  if(r==NULL || r->right==NULL) return r;
  return maxUnder(r->right);
}

tree * remove_neighbor_aux(struct neighbor *key, tree *t){
  int comp=compare_n(t->key,key);
  if(comp==0){
    if(t->left==NULL && t->right==NULL){
      clean(t);
      return NULL;
    }
    else if(t->left==NULL){
      clean(t);
      return t->right;
    }
    else if(t->right==NULL){
      clean(t);
      return t->left;
    }
    tree *m=maxUnder(t->left);
    tree *res=remove_neighbor_aux(m->key,t);
    res->key=m->key;
    res->val=m->val;
    free(t);
    return res;
  } 
  if(comp>0){
    t->left=remove_neighbor_aux(key,t->left);
    return t;
  }
  t->right=remove_neighbor_aux(key,t->right);
  return t;
}

short remove_neighbor(struct neighbor *key){
      NEIGHBORS=remove_neighbor_aux(key,NEIGHBORS);
      return 1;
}



void clean(tree *t){
  if(t->left!=NULL) clean(t->left);
  if(t->right!=NULL ) clean(t->right);
  free(t->key);
  free(t->val);
  free(t);  
}


/*Pourquoi pas ?*/


int depth(tree *abr){
  if(abr==NULL) return -1;
  return 1+max(depth(abr->left),depth(abr->right));
}

short isBalanced(tree *abr){
  if(abr==NULL) return 1;
  if(abr->left==NULL && abr->right==NULL) return 1;
  if(abr->left==NULL && abr->right!=NULL && (abr->right->right!=NULL || abr->right->left!=NULL)) return 0 ;
  if(abr->right==NULL && abr->left!=NULL && (abr->left->right!=NULL || abr->left->left!=NULL)) return 0 ; return isBalanced(abr->left) && isBalanced(abr->right);
}

short check(tree *t){
  if(t==NULL) return 1;
  if((t->left!=NULL && compare_n(t->key,t->left->key)<0) || (t->right!=NULL && compare_n(t->key,t->right->key)>0)) return 0;
  return check(t->left) && check(t->right);
}


/*Affichage*/

void print_key(struct neighbor *key){
  printf("IP : ");
  print_addr(key->ip);
  printf(" Port : %u ",key->port);
}

void print_val(struct ident *val){
    printf("Id : %u",val->id);
    printf("Last hello : %u ",val->last_hello);
    printf("Last long hello : %u ",val->last_hello_long);
}

void print_tree_aux(tree *abr,int tmp){
  char vide[40] = {'\t'};
  if(abr==NULL) {
    write(1,vide,tmp);
    printf("null\n");
    return;
  }
  print_tree_aux(abr->left,tmp+1);
  write(1,vide,tmp);
  print_addr(abr->key->ip);
  print_tree_aux(abr->right,tmp+1);
}

void print_tree(tree *abr){
  printf("affichage de l'arbre\n");
  print_tree_aux(abr,0);
}
