#ifndef NET_LIB_H
#define NET_LIB_H
#include "struct.h"
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

int send_message(int fd,void *buf, size_t taille,struct neighbor rcpt);

struct neighbor sockaddr6_to_neighbor(struct sockaddr_in6 saddr);

struct sockaddr_in6 neighbor_to_sockaddr6(struct neighbor);

void print_addr(u_int8_t *ip);

int send_first_message(int soc, char *addr, char *port);

void print_msg(struct message_h msg);
#endif
