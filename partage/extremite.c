/*  extremite
    Master Informatique 2017 -- Université Aix-Marseille  
    Emmanuel Godard
    modified by Alex ANDRIAMAHALEO
    for education use
    extremite.c
*/

#include "extremite.h"

void ext_out(){

    printf("ext-out running...\n");

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

void ext_in(int tun_fd_in){
    printf("ext-in running...\n");

    int tun_fd = tun_fd_in ;
    int sock_fd = SOCK_STREAM ;
    char buffer[MAX_MTU] ;
    int stat, r_bytes, w_bytes ;
    int length = 0 ;

    fd_set read_set ;

    while(1){
        FD_ZERO(&read_set);
        FD_SET(tun_fd, &read_set);
        stat = select(tun_fd+1, &read_set, NULL, NULL, NULL);

        if(stat < 0){
            printf("Erreur : stat = __select__\n");
            exit(1);
        }

        if(FD_ISSET(tun_fd, &read_set)){
            w_bytes = 0 ;
            stat = 0 ;
            r_bytes = 0 ;

            bzero(buffer,MAX_MTU);

            r_bytes = read(tun_fd, buffer, MAX_MTU);
            if(r_bytes < 0){
                printf("Erreurr tun0 - __read__ échoué");
            }
            
            length = htons(r_bytes);

            w_bytes = write(sock_fd, &length, sizeof(length));
            if(w_bytes < 0){
                printf("Erreur __write__ sur la socket échoué\n");
            }

            w_bytes = write(sock_fd, buffer,r_bytes);
            if(w_bytes < r_bytes){
                printf("Erreur __ write__ (2) sur le socket échoué\n");
            }

            printf("Reçu: %d bytes en provenance de tun0 et %d bytes écrit sur la socket\n", r_bytes, w_bytes);
        }
    }
}

int main (int argc, char** argv){

  if(argc != 3)
  {
    printf("Usage: %s tun_name -[mode]\nServeur set: -s\nClient set: -c\n",argv[0]);
    exit(1);
  }

  char *mode = argv[2];
  char *tun_name = argv[1];

  printf("tun_name: %s\n",argv[1]);

  int tun_fd = tun_alloc(tun_name);

  switch(mode[1]){
    case 'c':
        printf("Client >>\n");
        ext_in(tun_fd);
        break;
    case 's':
        printf("Serveur >>\n");
        ext_out();
        break;
    default:
        printf("Erreur __mode__\n");
        break;
  }


  return 0;
}