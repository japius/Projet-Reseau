#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define BUF_SIZE 1024
#define REQ_SIZE 14

void fill_requete(unsigned char *req) {
  req[0] =  93 ; // Magic
  req[1] =   2 ; // Version
  req[2] =   0 ; // Octet 2 taille -> attention big indian
  req[3] =  10 ; // Octet 1 taille
  req[4] =   2 ; // TLV Hello
  req[5] =   8 ; // Taille du TLV Hello Long
  req[13] = 45 ; // Id sur 8 octets j'ai mis au hasard  une case
  printf("Req : ");
  for (int i = 0 ; i < REQ_SIZE ; i++) { printf("%.2x ", req[i]); }

  // Tu peux générer un nombre de 64bits et le caster dans (uint64_t*)req[6]

}

void fill_sockaddr(struct sockaddr_in6 * peer) {
  int rc = 0;
  struct addrinfo hints = {0};
  struct addrinfo *r;
  hints.ai_family = AF_INET6; // Oblige l'IPV6
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = (AI_V4MAPPED | AI_ALL);
  // Remplace gethostbyname _
  rc = getaddrinfo("jch.irif.fr", "1212", &hints, &r);
  // Ici tu peux faire une itération (cf man)
  struct addrinfo *p = r;
  if (rc < 0 || p == NULL) {
    fprintf(stderr, "Bug socket\n");
    freeaddrinfo(r);
    exit(1);
  }

  // On copie l'adresse dans le pair
  memmove(peer,p->ai_addr, sizeof(struct sockaddr_in6));
  freeaddrinfo(r);
}

int main () {

  unsigned char req [REQ_SIZE] = {0}; // TLV Hello Court
  unsigned char buf [BUF_SIZE] = {0}; // Buffer de réception

  // Socket de type datagram
  int s = socket(AF_INET6, SOCK_DGRAM, 0);
  if (s < 0) {
    fprintf(stderr, "Bug socket\n");
    exit(1);
  }

  /*
    [...] Zone où rendre la socket non bloquante
    si besoin ou pour ajouter des options
   */

  // Sockaddr pour "bind" l'adresse
  struct sockaddr_in6 sin6 = {0};

  // Socket de réception de l'adresse du pair
  struct sockaddr_in6 peer = {0};
  int peer_size = sizeof(struct sockaddr_in6);

  // Définition de la socket
  sin6.sin6_family = AF_INET6; // Adresse IPV6
  sin6.sin6_port = htons(1212); // Port local

  // On lit la socket au port 1212 pour être en écoute sur
  // ce port
  int rc = bind(s, &sin6, sizeof(sin6));
  if (rc < 0) {
    fprintf(stderr,"Error bind\n");
    exit(1);
  }

  // Remplit l'adresse client
  fill_sockaddr(&peer);

  // Prépare la requête
  fill_requete(req);

  while(1) {

    // Envoi de la requête à la bonne adresse
    sendto(s,req,REQ_SIZE, 0, &peer, sizeof(struct sockaddr_in6));
    printf("\nEnvoi effectué\n");

    // Réception d'un TLV Hello
    rc = recvfrom(s, buf, BUF_SIZE,0,&peer, &peer_size);
    if(rc < 0) {
      fprintf(stderr,"Error recev\n");
      exit(1);
    }

    // Affichage en octets
    printf("Message reçu\n");
    for (int i = 0 ; i < rc ; i++) { printf("%.2x ", buf[i]); }


    // Dans notre cas pour pas faire trente boucles
    return EXIT_SUCCESS;
  }
}
