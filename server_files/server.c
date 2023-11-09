#include "server.h"

//Port d'écoute du serveur
int port;

struct ArgConnexionsEntrantes * initArgConnexionsEntrantes()
{
    struct ArgConnexionsEntrantes * arg = (struct ArgConnexionsEntrantes *) malloc(sizeof(struct ArgConnexionsEntrantes));
    arg->csin = (SOCKADDR_IN *) malloc(sizeof(SOCKADDR_IN));
    arg->sock = (SOCKET *) malloc(sizeof(SOCKET));
    
    //On l'initialise à NULL pour indiquer qu'il n'y a pas de nouvelle connexion entrante
    arg->sock_cible = NULL;
    //arg->sock_cible = (SOCKET *) malloc(sizeof(SOCKET));
    return arg;
}

void initialiser_socket_connexions_tcp_entrantes(SOCKET * sock, SOCKADDR_IN * csin)
{

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
    }
    printf("Succès du bind !\n"); 
    
    if(listen(*sock,MAX_CLIENTS)==SOCKET_ERROR){
      perror("listen()");
      exit(EXIT_FAILURE);
    }

}

void lecture_arguments(int argc, char * argv[])
{
    if(argc<2)
      erreur("Veuillez renseigner un port pour le serveur !\n");
      
    port = atoi(argv[1]);
}

void * accepter_connexions_tcp(void * arg){
  
    //Les données du thread, qui contienent le thread lui-même, le mutex et les arguments
    ThreadData * Donnees_Thread = (ThreadData *) arg;
    struct ArgConnexionsEntrantes * SocketsEtAdresse = Donnees_Thread->shared_data;

    SOCKET SocketNouvelleConnexion_Temporaire;
    socklen_t sinsize = sizeof *(SocketsEtAdresse->csin);
    
    //Boucle principale
    while(1){
      SocketNouvelleConnexion_Temporaire = accept(*(SocketsEtAdresse->sock), (SOCKADDR *) SocketsEtAdresse->csin,&sinsize);
      
      if(SocketNouvelleConnexion_Temporaire == INVALID_SOCKET) alerte("Erreur socket entrante\n");
      else{
        alerte("Nouvelle connexion entrante !\n");
        pthread_mutex_lock(&Donnees_Thread->mutex); //On bloque la mémoire aux autres threads avant écriture
        *(SocketsEtAdresse->sock_cible) = SocketNouvelleConnexion_Temporaire;
        printf("Socket : %d\n",SocketNouvelleConnexion_Temporaire);
        pthread_mutex_unlock(&Donnees_Thread->mutex);
      }
    }
    return NULL;
}

void * lecture_tcp(void * arg){
  struct ArgCommunication * client = (struct ArgCommunication *) arg;
  Train * train = client->train;
  SOCKET socket = client->socket;
  int n_train = client->n_train;
  struct CommunicationTrain position;
  while(1){
      read(socket,&position,sizeof(position));
      pthread_mutex_lock(&train->DonneesThread->mutex);
      train->positionX = position.x;
      train->positionY = position.y;
      train->velocite = position.vitesse;
      printf("Position client : %d, %d, %f",position.x,position.y,position.vitesse);
      pthread_mutex_unlock(&train->DonneesThread->mutex);

    }
    /*struct CommunicationVersTrain * communication_train = malloc(sizeof(struct CommunicationVersTrain));
    communication_train->code_erreur = -1;
    communication_train->distance_a_parcourir=train->distanceSecurite;
    communication_train->vitesse_consigne = train->velocite;
    write(socket,&communication_train,sizeof(communication_train));
    pthread_mutex_unlock(&train->DonneesThread->mutex);*/
    
}

void * entrees_utilisateur(void * arg){

  //Les données du thread, qui contienent le thread lui-même, le mutex et les arguments
  struct ThreadData * Donnes_Thread = (struct ThreadData *) arg;

  //Chaine de caractère qui contient les entrées de l'utilisateur à renvoyer au programme principal
  char * EntreesUtilisateur = Donnes_Thread->shared_data;

  //Chaine de caractères qui va écouter à l'aide de getline les entrées utilisateur
  char * EntreesUtilisateurTemp = NULL;

  ssize_t len = 0;

  //Boucle principale
  while(1)

  if(getline(&EntreesUtilisateurTemp,&len,stdin) != -1)

    if(len < MAX_BUFFER){

      EntreesUtilisateurTemp[strcspn(EntreesUtilisateurTemp, "\n")] = '\0';
      pthread_mutex_lock(&Donnes_Thread->mutex);
      strcpy(EntreesUtilisateur,EntreesUtilisateurTemp);
      pthread_mutex_unlock(&Donnes_Thread->mutex);

    }else alerte("L'entrée est trop longue !\n");
    
  else alerte("Erreur lors de la lecture de l'entrée.\n");

}


int main(int argc, char * argv[]){

    //Assignation du port
    
    lecture_arguments(argc,argv);


    //Initialisation de la lecture des entrées utilisateur
    
    char * EntreesUtilisateur = (char *) malloc(sizeof(char)*MAX_BUFFER);
    strcpy(EntreesUtilisateur,""); //On l'initialise à vide
    ThreadData dataThreadEntreesUtilisateur;
    pthread_mutex_init(&dataThreadEntreesUtilisateur.mutex,NULL);
    dataThreadEntreesUtilisateur.shared_data = EntreesUtilisateur;

    pthread_create(&(dataThreadEntreesUtilisateur.thread),NULL,entrees_utilisateur,
    (void *) &dataThreadEntreesUtilisateur);

    
    //Initialisation du socket d'écoute et de l'adresse d'écoute
    struct ArgConnexionsEntrantes * socket_source_cible = initArgConnexionsEntrantes();
    initialiser_socket_connexions_tcp_entrantes(socket_source_cible->sock,socket_source_cible->csin);



    //Lancement du processus d'écoute permanente de nouvelles connexions
    ThreadData dataAccepterConnexions;
    pthread_mutex_init(&dataAccepterConnexions.mutex,NULL);
    dataAccepterConnexions.shared_data = socket_source_cible;
    
    pthread_create(&(dataAccepterConnexions.thread),NULL,accepter_connexions_tcp,
    (void *) &dataAccepterConnexions);
    

    Train * * listeClients = Train_Liste_Initialiser();

    //Boucle principale du main
    while(1){
      usleep(CLOCK_US);//horloge

      // ============== Entrées utililsateur ==============
      pthread_mutex_lock(&dataThreadEntreesUtilisateur.mutex);
      if(strcmp(EntreesUtilisateur,"")){
        if(!strcmp(EntreesUtilisateur,"quit"))
          exit(EXIT_SUCCESS);



        pthread_mutex_lock(&dataAccepterConnexions.mutex);
        if(isnumber(EntreesUtilisateur)&&socket_source_cible->sock_cible != NULL){//Nouvelle connexion
        int numero = atoi(EntreesUtilisateur);
        struct ArgCommunication * comm = malloc(sizeof(struct ArgCommunication));
        Train * NouveauTrain = Train_Initialiser("Nouveau train");
        comm->train = NouveauTrain;
        comm->socket = *socket_source_cible->sock_cible;
        Train_Liste_Ajouter_Train(listeClients,NouveauTrain,numero);
        comm->n_train = numero;
        socket_source_cible->sock_cible = NULL;
        pthread_create(comm->train->DonneesThread->thread,NULL,lecture_tcp,(void *) comm);
      }
      pthread_mutex_unlock(&dataAccepterConnexions.mutex);


        printf("Vous avez écrit : %s\n",EntreesUtilisateur);
        strcpy(EntreesUtilisateur,"");//Réinitialisation de la chaine de caractères
      }
      pthread_mutex_unlock(&dataThreadEntreesUtilisateur.mutex);


      // ============== Nouvelles connexions ==============
      pthread_mutex_lock(&dataAccepterConnexions.mutex);
      if(socket_source_cible->sock_cible != NULL){//Nouvelle connexion
        printf("Nouvelle connexion, veuillez rentrer un numéro\n");
      }
      pthread_mutex_unlock(&dataAccepterConnexions.mutex);


      // ============== Connexions existantes ==============
      Train_Liste_Calculer_Distances_Securite(listeClients);

      for(int i=0; i<MAX_TRAINS_LISTE;i++)

      //On vérifie que le client est non vide
      if(listeClients[i]){
        pthread_mutex_lock(&listeClients[i]->DonneesThread->mutex);
        struct Train_Communication_Messages * com;
        com = listeClients[i]->DonneesThread->shared_data;
        pthread_mutex_unlock(&listeClients[i]->DonneesThread->mutex);
        
      }
      
    }

}