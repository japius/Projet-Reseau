#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Peut etre trier en fonction de la date de reéception du dernier hello long comme ca le plus à gauche on aura les voisins symétriques et le reste non !
//Cimment indexer avec (ip,port), à quoi est ce que ca servirait ?
typedef struct tree{
  struct neighbor *key;
  struct ident *val;
  struct  tree *left;
  struct tree *right;
}tree;



tree *init(struct neighbor *key,struct ident *val,tree *left,tree *right){
  tree *current=malloc(sizeof(tree));
  current->val=val;
  current->key=key;
  current->left=left;
  current->right=right;
  return current;
}

//Pour ajouter un voisin s'il n"existe pas et le mettre à jour sinon
tree *add_neighbor(tree *t,struct neighbor *key,struct ident *val){
  if(t==NULL) return init(key,val,NULL,NULL);
  int comp=compare_n(t->key,key);
  if(comp>0){
    if(t->left==NULL){
      t->left=init(key,val,NULL,NULL);
      return t;
    }
    t->left=add_neighbor(t->left,key,val);
    return t;
  }
  else if(comp<0){
    if(t->right==NULL){
      t->right=init(key,val,NULL,NULL);
      return t;
    }
    t->right=add_neighbor(t->right,key,val);
    return t;
  }    
  t->val->last_hello=val->last_hello;
  t->val->last_hello_long=val->last_hello_long;
  return t;
}

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

short issymmetrical(struct neighbor *key){
  time_t now=time(0);
  if(now-key->last_hello_long<120) return 1;
  return 0;
}

//Pour chercher tous les voisins symétriques
//a vérifier
struct list_entry *get_symmetrical(tree *t){
  if(t!=NULL){
    if(issymmetrical(t->key)){
      struct list_entry *sym=malloc(sizeof(list_entry));
      sym->sym=key;
      sym->times_sent=0;
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
}

//Renvoie un pointeur vers le maximum du sous arbre de racine r
tree *maxUnder(tree *r){
  if(r==NULL || r->right==NULL) return r;
  return maxUnder(r->right);
}

tree * remove_neighbor(struct neighbor *key, tree *t){
  int comp=compare_n(t->key,key);
  if(comp==0){
    if(t->left==NULL && t->right==NULL) return NULL;
    else if(t->left==NULL){
      t=t->right;
      return t;
    }
    else if(t->right==NULL){
      t=t->left;
      return t;
    }
    tree *m=maxUnder(t->left);
    t=remove_neighbor(m->key,t);
    t->key=m->key;
    t->val=m->val;
    return t;
  } 
  else if(comp>0){
    t->left=remove_neighbor(key,t->left);
    return t;
  }
  t->right=remove_neighbor(key,t->right);
  return t;
}


void clean(tree *t){
  if(t->left!=NULL) clean(t->left);
  if(t->right!=NULL ) clean(t->right);
  free(t->key);
  free(t->val);
  free(t);  
}

int max(int x,int y){
  return (x<=y)?y:x;
}

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

int maximum(tree *t){
  if(t->right==NULL) return t->val;
  return maximum(t->right);
}

int minimum(tree *t){
  if(t->left==NULL) return t->val;
  return minimum(t->left);
}

short check(tree *t){
  if(t==NULL) return 1;
  if((t->left!=NULL && compare_n(t->key,t->left->val)<0) || (t->right!=NULL && compare_n(t->val,t->right->val)>0)) return 0;
  return check(t->left) && check(t->right);
}



void print_key(struct neighbor *key){
}

void print_val(struct ident *val){

}

void print_tree(tree *abr){
  if(abr!=NULL){
    print_tree(abr->left);
    print_key(abr->key);
    print_val(abr->val);
    //printf("| %d",abr->val); 
    print_tree(abr->right);
  }
}


