#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <string.h>

#include "marvelmind.h"


/**
 * @brief  Enregistre la position du marvel mind
 * @note   variable globale
 *
*/
struct PositionValue position = {0};

/**
 * @brief  Détermine si oui ou non la variable position a été actualisée
 * @note   variable globale
 *
*/
bool ecriture_update = false;
//détermine quand il faut terminer le programme
bool terminateProgram=false;

//port du serveur (connexion TCP)
int port = 0;
char adresse[20] = "127.0.0.1";
/**
 * @brief  Permet d'empêcher la lecture et l'écriture simultanée dans la variable position
 * @note   variable globale
 *
*/
pthread_mutex_t mutex_lock_communication;

/**
 * @brief  Fonction définie dans la doc du marvel mind qui arrête le programme
 * @note   
 * @param  signum: 
 * @retval None
 *
*/
void CtrlHandler(int signum)
{
    terminateProgram=true;
}


//variable définie dans l'exemple de marvel mind
static sem_t *sem;
//variable définie dans l'exemple de marvel mind
struct timespec ts;
/**
 * @brief  Fonction définie dans la doc du Marvel Mind
 * @note   
 * @retval None
 *
*/
void semCallback()
{
	sem_post(sem);
}


/**
 * @brief  Tente de capturer la position du marvel mind.
 * @note   Si la position est capturée, indique au thread d'envoie de messages d'envoyer un message
 * @param  hedge: variable du marvel mind
 * @retval true si le marvel mind a capté une position
 * @throw  false si le marvel mind a échoué à capturer une position
 *
*/
bool get_position(struct MarvelmindHedge * hedge){
    //blocage du mutex
    pthread_mutex_lock(&mutex_lock_communication);

    
    if(getPositionFromMarvelmindHedge (hedge, &position)){//Si la position du marvel
                                                          // mind est indentifiée

        
        //indique au thread de l'envoie de messages qu'il y a une position à envoyer
        ecriture_update = true;
        //Affiche la position à l'écran
        //printf("Nouvelle position :\nX: %d, Y: %d,Z: %d\n\n",position.x,position.y,position.z);
        pthread_mutex_unlock(&mutex_lock_communication);
        return true;
    }
    pthread_mutex_unlock(&mutex_lock_communication);
    //Sinon, affiche un message d'erreur
    printf("Erreur lors de l'acquisition de la position !\n");
    return false;
}

void * reception_message(void * p_client_socket){
    int client_socket = *((int *) p_client_socket);
    char buffer[1024];
    while(1){
        int bytes_received = read(client_socket, buffer, sizeof(buffer));
        // Traitement les données reçues
        printf("Message recu : %s\n",buffer);
        memset(buffer,0,strlen(buffer));
    }
    
    return NULL;

}



/**
 * @brief  envoie un message si la position a été mise à jour
 * @note   
 * @param  client_socket: socket du client
 * @retval true si la position a été actualisée, false sinon
 *
*/
void handler_envoie_message(int client_socket){
    //blocage du mutex
    pthread_mutex_lock(&mutex_lock_communication);
    if(ecriture_update){//si la position a été actualisée

        
        //envoie des données
        write(client_socket,&position,sizeof(position));

        
        //spécification que l'info a été envoyée
        ecriture_update = false;
        //déblocage du mutex
        pthread_mutex_unlock(&mutex_lock_communication);

        return;
    }
    //déblocage du mutex
    pthread_mutex_unlock(&mutex_lock_communication);
    return;
}


/**
 * @brief  Communique avec le serveur
 * @note   A chaque actualisation de la position, envoie la position au serveur
 * @param  arg: NULL
 * @retval NULL
 *
*/
void * communiquer(void * arg){
    //Création socket connexion

    //variable socket qui gère la connexion avec le serveur
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Erreur lors de la création du socket client\n");
        exit(EXIT_FAILURE);
    }

    //adresse du serveur
    struct sockaddr_in addr_serv;
    addr_serv.sin_family = AF_INET;
    addr_serv.sin_port = htons(port);
    addr_serv.sin_addr.s_addr = inet_addr(adresse); //adresse locale

    printf("Tentative de connexion...\n");
    //tentative de connexion au serveur
    if (connect(client_socket, (struct sockaddr*)&addr_serv, sizeof(addr_serv)) == -1) {
        //si la tentative a échouée
        perror("Erreur lors de la connexion au serveur\n");
        close(client_socket);
        exit(EXIT_FAILURE);
    }
    printf("Succès de la connexion !\n");
    pthread_t threadReception;
    int socket_thread_communication = client_socket;
    pthread_create(&threadReception,NULL,reception_message,(void *) &client_socket);
    
    while(1){
        //Pour une raison étrange, je n'arrive pas à utiliser les signaux
        //pour cette fonction... J'utilise donc usleep
        handler_envoie_message(client_socket);
        usleep(1000 * 300);
    }

    return NULL;
    
}


void handler_pause(){

}


int main (int argc, char *argv[])
{
    // get port name from command line arguments (if specified)
    const char * ttyFileName;
    if (argc >= 2) ttyFileName = argv[1];
    else ttyFileName = DEFAULT_TTY_FILENAME;

    //lecture de l'argument du port
    if(argc >= 3)port = atoi(argv[2]);
    else{
        printf("Aucun port spécifié (2e position en argument !\n)");
    }
    //lecture de l'adresse:
    if(argc >= 4)
    strcpy(adresse,argv[3]);
    // Init

    //Variable du marvel mind
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

    // Set Ctrl-C handlergnome-terminal

    //signal (SIGINT, CtrlHandler);
    //signal (SIGQUIT, CtrlHandler);
	sem = sem_open(DATA_INPUT_SEMAPHORE, O_CREAT, 0777, 0);
    
    
    //Lancement de la communication

    //Variable thread qui gère la communication
    pthread_mutex_init(&mutex_lock_communication,NULL);
    pthread_t thread_communication;
    pthread_create(&thread_communication,NULL,communiquer,NULL);

    
    // Main loop
    while ((!terminateProgram) && (!hedge->terminationRequired))
    {
        

        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
		{
			printf("clock_gettime error\n");
			return -1;
		}
		ts.tv_sec += 2;
		sem_timedwait(sem,&ts);

        ecriture_update = true;
        //Capture de la position et cas échéant envoie de l'information au thread de communication
        get_position(hedge);
        
        //Attente de 100ms
        usleep(100 * 1000);
    }
    
    // Exit
    stopMarvelmindHedge (hedge);
    destroyMarvelmindHedge (hedge);
    return 0;
}
