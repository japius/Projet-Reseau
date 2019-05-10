#ifndef NET_LIB_H
#define NET_LIB_H
#include "struct.h"
#include "abr.h"
/*
renvoie une socket qui se connect au port 'port' de l'adresse 'addr'
erreurs :
-1 : erreur au getaddrinfo
-2 : n'arrive pas à se connecter à une addresse donnée par getaddrinfo
*/
int init_socket_client_udp(char *addr, char *port);

/*
renvoie une socket qui accepte les connections sur le port 'port'
erreurs :
-1 : la création de la socket a échoué
-2 : le bind a échoué
*/
int init_socket_server_udp(int port);

int init_socket_client_udp_v2();

/*lis size_buf octets sur le descripteur fd (renvoie 0 si le read est plus long que timeout)*/
int read_socket(int fd,void *buf, size_t size_buf,struct timeval *timeout);

/*remplis les 'octets_numbers' premiers octets de var avec des valeurs aléatoires */
void random_on_octets(void *var, size_t octets_number);

int send_message(int fd,struct message_h *buf, size_t taille,struct neighbor rcpt);

int send_message_now(int fd,void *buf, size_t taille,struct neighbor rcpt);

struct neighbor sockaddr6_to_neighbor(struct sockaddr_in6 saddr);

struct sockaddr_in6 neighbor_to_sockaddr6(struct neighbor);

void print_addr(u_int8_t *ip);
void print_addr2(u_int8_t *ip);

int send_to_everyone_new(int fd, tree*);
int send_to_everyone(int fd, void *buf, size_t length, tree *);
int send_hello_everyone(int fd, tree *people);
int send_symetrical_everyone(int fd, tree *people);
int send_shorthello_everyone(int fd, tree *people);
int send_goaway_asymetrical(int fd);
void remove_old_potential();

int send_first_message(int soc, char *addr, char *port);

void print_msg(struct message_h msg);
void print_sockaddr(struct sockaddr_in6);
int print_tlv(unsigned char *buf);
#endif
