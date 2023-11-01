#include "server.h"


void initialiser_socket_connexions_tcp_entrantes(SOCKET * sock, SOCKADDR_IN * csin){

    //Initialisation de l'adresse d'écoute
    socklen_t sinsize = sizeof csin;
    csin->sin_family = AF_INET;
    csin->sin_addr.s_addr = INADDR_ANY;
    csin->sin_port = htons(port);


    //Affectation du socket d'écoute
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(*sock, (SOCKADDR*)&csin, sizeof(csin)) == SOCKET_ERROR) {
      perror("Erreur lors de la liaison du socket");
      exit(EXIT_FAILURE);
    }else{
      printf("Succès du bind !\n"); 
    }
    if(listen(*sock,MAX_CLIENTS)==SOCKET_ERROR){
      perror("listen()");
      exit(EXIT_FAILURE);
    }


}

void lecture_arguments(int argc, char * argv[]){

    if(argc<=2){
        port = atoi(argv[1]);
    }else{
        erreur("Veuillez renseigner un port pour le serveur !");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char * argv[]){

    //Assignation du port
    
    lecture_arguments(argc,argv);

    //Initialisation de la lecture des entrées utilisateur

    char * EntreesUtilisateur = (char *) malloc(sizeof(char)*MAX_BUFFER);
    ThreadData dataThreadEntreesUtilisateur;
    dataThreadEntreesUtilisateur.shared_data = EntreesUtilisateur;
    pthread_create(&(dataThreadEntreesUtilisateur.thread),NULL,entrees_utilisateur,(void *) dataThreadEntreesUtilisateur.shared_data);


    struct ArgConnexionsEntrantes socket_source_cible;
    initialiser_socket_connexions_tcp_entrantes(socket_source_cible.sock,socket_source_cible.csin);

    SOCKET * SocketNouvellesConnexions = (SOCKET *) malloc(sizeof(SOCKET));
    ThreadData dataAccepterConnexions;
    dataAccepterConnexions.shared_data = SocketNouvellesConnexions;
    

}