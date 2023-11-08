#include "train.h"

// Fonction qui permet d'obtenir la position du train
bool get_position(struct MarvelmindHedge * hedge){
    //blocage du mutex
    pthread_mutex_lock(&mutex_lock_communication);


    if(getPositionFromMarvelmindHedge (hedge, &position)){//Si la position du marvel
                                                          // mind est indentifiée


        //indique au thread de l'envoie de messages qu'il y a une position à envoyer
        ecriture_update = true;
        //Affiche la position à l'écran
        //printf("Nouvelle position :\nX: %d, Y: %d,Z: %d\n\n",position.x,position.y,position.z);
        return true;
    }

    //Sinon, affiche un message d'erreur
    printf("Erreur lors de l'acquisition de la position !\n");
    return false;
}

/**
 * @brief  Fonction principale
 * @note    D'abord lit les arguments
 *          Ensuite initialise la connexion avec le marvelmind
 *          Ensuite initialise la connexion au serveur
 *          Ensuite avec un thread maintient la connexion avec le serveur
 *          Ensuite démarre la logique pour avancer : 
 *          - demande d'EOA, de distance à parcourir et de consigne de vitesse au serveur
 *          
 *              Si la connexion au serveur s'arrête on relance une tentative de connexion et 
 *              surtout on s'arrête
 * @param  argc: longueur des arguments
 * @param  argv[]: port du serveur - IP du serveur - nom du train
 * @retval 
 *
*/
int main(int argc, char * argv[]){

  struct Train train;
  train.name = "Train A";
  train.distance_a_parcourir = 0.0;
  train.V_actu = 0.0;
  train.V_consigne = 0.0;
  train.code_erreur = 0;

  struct MarvelmindHedge * hedge=createMarvelmindHedge();
    if (hedge==NULL)
    {
        puts ("Error: Unable to create MarvelmindHedge");
        return -1;
    }
    hedge->ttyFileName=ttyFileName;
    hedge->verbose=true; // show errors and warnings
    hedge->anyInputPacketCallback= semCallback;
    startMarvelmindHedge (hedge);

    // Set Ctrl-C handler

    signal (SIGINT, CtrlHandler);
    signal (SIGQUIT, CtrlHandler);


  sem = sem_open(DATA_INPUT_SEMAPHORE, O_CREAT, 0777, 0);

  // Main loop
  while ((!terminateProgram) && (!hedge->terminationRequired))
  {


      if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
  {
    printf("clock_gettime error");
    return -1;
  }
  ts.tv_sec += 2;
  sem_timedwait(sem,&ts);

      //Capture de la position et cas échéant envoie de l'information au thread de communication
      get_position(hedge);

  }
  
  // Création de la socket
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1) {
    perror("Erreur lors de la création de la socket");
    return -1;
  }    


  // Configuration de l'adresse du serveur
  struct sockaddr_in server_address;
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[1]));
  server_address.sin_addr.s_addr = inet_addr(argv[2]);

  // Connexion au serveur
  if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) ==  -1) {
    perror("Erreur lors de la connexion au serveur");
    close(client_socket);
    return -1;
  }
  // Création du thread de communication
  pthread_t thread;
  if (pthread_create(&thread, NULL, communicationAvecServeur, NULL) != 0) {
    perror("Erreur lors de la création du thread de communication");
    close(client_socket);
    return -1;
  }

  
  pthread_join(thread, NULL);
  return 0
}