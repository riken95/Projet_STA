#include "server.h"


void initialiser_socket_connexions_tcp_entrantes(SOCKET * sock, SOCKADDR_IN * csin){

    //Initialisation de l'adresse d'écoute
    socklen_t sinsize = sizeof(*csin);
    csin->sin_family = AF_INET;
    csin->sin_addr.s_addr = INADDR_ANY;
    csin->sin_port = htons(port);


    //Affectation du socket d'écoute
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    if (bind(*sock, (SOCKADDR*)csin, sizeof(*csin)) == SOCKET_ERROR) {
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

    if(argc>=2){
        port = atoi(argv[1]);
    }else{
        //erreur("Veuillez renseigner un port pour le serveur !\n");
        port = 20000;
        //exit(EXIT_FAILURE);
    }
}

void * accepter_connexions_tcp(void * arg){
  
    //Les données du thread, qui contienent le thread lui-même, le mutex et les arguments
    ThreadData * Donnees_Thread = (ThreadData *) arg;
    struct ArgConnexionsEntrantes * SocketsEtAdresse = Donnees_Thread->shared_data;

    SOCKET SocketNouvelleConnexion_Temporaire;
    socklen_t sinsize = sizeof *(SocketsEtAdresse->csin);
    
    while(1){
      SocketNouvelleConnexion_Temporaire = accept(*(SocketsEtAdresse->sock), (SOCKADDR *) SocketsEtAdresse->csin,&sinsize);
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

void * entrees_utilisateur(void * arg){

  //Les données du thread, qui contienent le thread lui-même, le mutex et les arguments
  struct ThreadData * Donnes_Thread = (struct ThreadData *) arg;

  //Chaine de caractère qui contient les entrées de l'utilisateur à renvoyer au programme principal
  char * EntreesUtilisateur = Donnes_Thread->shared_data;

  //Chaine de caractères qui va écouter à l'aide de getline les entrées utilisateur
  char * EntreesUtilisateurTemp = NULL;

  ssize_t read;
  size_t len;
  while(1){
    read = getline(&EntreesUtilisateur,&len,stdin);

    if(read != -1){

      if(len < MAX_BUFFER){
        pthread_mutex_lock(&Donnes_Thread->mutex);
        strcpy(EntreesUtilisateur,EntreesUtilisateurTemp);
        pthread_mutex_unlock(&Donnes_Thread->mutex);
      }else{
        erreur("L'entrée est trop longue !");
      }
      

    }else{

      erreur("Erreur lors de la lecture de l'entrée.");
    }
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

    
    //Initialisation du socket d'écoute et de l'adresse d'écoute
    struct ArgConnexionsEntrantes * socket_source_cible = initArgConnexionsEntrantes();
    initialiser_socket_connexions_tcp_entrantes(socket_source_cible->sock,socket_source_cible->csin);

    
    //Socket qui va recevoir les nouvelles connexions
    SOCKET * SocketNouvellesConnexions;
    socket_source_cible->sock_cible = SocketNouvellesConnexions;


    //Lancement du processus d'écoute permanente de nouvelles connexions
    ThreadData dataAccepterConnexions;
    dataAccepterConnexions.shared_data = socket_source_cible;
    
    pthread_create(&(dataAccepterConnexions.thread),NULL,accepter_connexions_tcp,
    (void *) &dataAccepterConnexions);
    
    while(1){
      sleep(1);
    }



}