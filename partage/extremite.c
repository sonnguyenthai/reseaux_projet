/*  extremite
    Master Informatique 2017 -- Université Aix-Marseille  
    Emmanuel Godard
    modified by Alex ANDRIAMAHALEO
    for education use
    extremite.c
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <unistd.h>
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <netdb.h>

/* taille maximale des lignes */
#define MAXLIGNE 80

void ext_out (){

    int s,n; /* descripteurs de socket */
    int len,on; /* utilitaires divers */
    struct addrinfo * resol; /* résolution */
    struct addrinfo indic = {AI_PASSIVE, /* Toute interface */
                            PF_INET,SOCK_STREAM,0, /* IP mode connecté */
                            0,NULL,NULL,NULL};
    struct sockaddr_in client; /* adresse de socket du client */
    int err; /* code d'erreur */

    char msg[MAXLIGNE+1]; /* tampons pour les communications */
    char tampon[MAXLIGNE+1];
    ssize_t lu; /* nb d'octets reçus */
    int compteur=0;

    err = getaddrinfo(NULL, "123", &indic, &resol);
    if (err<0){
        fprintf(stderr,"Résolution: %s\n",gai_strerror(err));
        exit(2);
    }

    if ((s=socket(resol->ai_family,resol->ai_socktype,resol->ai_protocol))<0) {
        perror("allocation de socket");
        exit(3);
    }
    fprintf(stderr,"le n° de la socket est : %i\n",s);

    on = 1;
    if (setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on))<0) {
        perror("option socket");
        exit(4);
    }
    fprintf(stderr,"Option(s) OK!\n");

    if (bind(s,resol->ai_addr,sizeof(struct sockaddr_in))<0) {
        perror("bind");
        exit(5);
    }
    freeaddrinfo(resol); /* /!\ Libération mémoire */
    fprintf(stderr,"bind!\n");

    if (listen(s,SOMAXCONN)<0) {
        perror("listen");
        exit(6);
    }
    fprintf(stderr,"listen!\n");

    int pid = getpid(); /* pid du processus */

    while(1) {
        /* attendre et gérer indéfiniment les connexions entrantes */
        len=sizeof(struct sockaddr_in);
        if( (n=accept(s,(struct sockaddr *)&client,(socklen_t*)&len)) < 0 ) {
        perror("accept");
        exit(7);
        }
        /* Nom réseau du client */
        char hotec[NI_MAXHOST];  char portc[NI_MAXSERV];
        err = getnameinfo((struct sockaddr*)&client,len,hotec,NI_MAXHOST,portc,NI_MAXSERV,0);
        if (err < 0 ){
        fprintf(stderr,"résolution client (%i): %s\n",n,gai_strerror(err));
        }else{ 
        fprintf(stderr,"accept! (%i) ip=%s port=%s\n",n,hotec,portc);
        }
        /* traitement */
        do { /* afficher sur la sortie standard */
            lu = recv(n,tampon,MAXLIGNE,0);
            if (lu > 0 )
            {
                compteur++;
                tampon[lu] = '\0';
                /* log */
                fprintf(stderr,"[%s:%s](%i): %3i :%s",hotec,portc,pid,compteur,tampon);
                snprintf(msg,MAXLIGNE,"> %s",tampon);
            } else {
                break;
            }
        } while ( 1 );
    }

    close(n);

    return;
}

int ext_in(int *port){
    return -1 ;
}

int main (int argc, char** argv){

  if(argc != 2)
  {
    printf("Usage: %s tun_name\n",argv[0]);
    exit(1);
  }


  return 0;
}