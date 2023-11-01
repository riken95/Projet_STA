#include "server.h"


void initialiser_socket_connexions_tcp_entrantes(SOCKET * sock, SOCKADDR_IN * csin){

    //Initialisation de l'adresse d'écoute
    socklen_t sinsize = sizeof *csin;
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

void * accepter_connexions_tcp(void * arg){
  
    struct ThreadData * Donnees_Thread = (struct ThreadData *) arg;
    struct ArgConnexionsEntrantes * SocketsEtAdresse = Donnees_Thread->shared_data;

    SOCKET SocketNouvelleConnexion_Temporaire;
    socklen_t sinsize = sizeof *(SocketsEtAdresse->csin);
    
    while(1){
      SocketNouvelleConnexion_Temporaire = accept(SocketsEtAdresse->sock, (SOCKADDR *) SocketsEtAdresse->csin,&sinsize);
      if(SocketNouvelleConnexion_Temporaire == INVALID_SOCKET){
        printf("Erreur socket entrante\n");
      }else{
        printf("Nouvelle connexion entrante !\n");
      }
      pthread_mutex_lock(&Donnees_Thread->mutex); //On bloque la mémoire aux autres threads avant écriture
      *(SocketsEtAdresse->sock_cible) = SocketNouvelleConnexion_Temporaire;
      printf("Socket : %d\n",SocketNouvelleConnexion_Temporaire);
      pthread_mutex_unlock(&Donnees_Thread->mutex);
      
    }
    
}

int main(int argc, char * argv[]){

    //Assignation du port
    
    lecture_arguments(argc,argv);


    //Initialisation de la lecture des entrées utilisateur

    char * EntreesUtilisateur = (char *) malloc(sizeof(char)*MAX_BUFFER);
    ThreadData dataThreadEntreesUtilisateur;
    dataThreadEntreesUtilisateur.shared_data = EntreesUtilisateur;
    pthread_create(&(dataThreadEntreesUtilisateur.thread),NULL,entrees_utilisateur,
    (void *) &dataThreadEntreesUtilisateur);


    //Initialisation du socket d'écoue et de l'adresse d'écoute
    struct ArgConnexionsEntrantes socket_source_cible;
    initialiser_socket_connexions_tcp_entrantes(socket_source_cible.sock,socket_source_cible.csin);


    //Socket qui va recevoir les nouvelles connexions
    SOCKET * SocketNouvellesConnexions;
    socket_source_cible.sock_cible = SocketNouvellesConnexions;


    //Lancement du processus d'écoute permanente de nouvelles connexions
    ThreadData dataAccepterConnexions;
    dataAccepterConnexions.shared_data = &socket_source_cible;
    
    pthread_create(&(dataAccepterConnexions.thread),NULL,accepter_connexions_tcp,
    (void *) &dataAccepterConnexions);
    



}