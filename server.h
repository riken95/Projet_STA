
// ====================== IMPORTATION ======================

//Importation des librairies
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close , pipe...*/
#include <netdb.h> /* gethostbyname */
#include <pthread.h>
#include <string.h>


//Librairies en plus
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>

//Importation des headers
#include "misc.h"
#include "liste_trains.h"

#ifndef SERVER_H
#define SERVER_H
// ====================== CONSTANTES ET VARIABLES GLOBALES ======================

//Constantes utiles pour le code
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
#define MAX_BUFFER 256          //Taille du buffer de communication
#define MAX_CLIENTS 32          //Nombre maximum de clients simultanés
#define CLOCK_US 1000*10        //Temps d'attente en microsecondes avant une nouvelle itération de la boucle principale




// ====================== Définition des structures ======================

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

/**
 * @brief  Définit une structure qui gère les threads
 * @note   
 *
*/
struct ThreadData{
    pthread_t thread;
    pthread_mutex_t mutex;
    void * shared_data;
};
typedef struct ThreadData ThreadData;

/**
 * @brief  Recu depuis le train vers le serveur
 * @note   
 *
*/
struct CommunicationTrain{
    int x;
    int y;
    float vitesse;
    int code_erreur;
};

/**
 * @brief  Recu depuis le serveur vers le train
 * @note   
 *
*/
struct CommunicationVersTrain{
    float distance_a_parcourir;
    float vitesse_consigne;
    int code_erreur;
};

/**
 * @brief  Utile à la fonction accepter_connexions_tcp
 * @note   
 *
*/
struct ArgConnexionsEntrantes{
    SOCKADDR_IN * csin;
    SOCKET * sock;
    SOCKET * sock_cible;
};
struct ArgConnexionsEntrantes * initArgConnexionsEntrantes();

struct ArgCommunication{
    Train * train;
    SOCKET socket;
    int n_train;
};

// ====================== Définition des fonctions ======================

/**
 * @brief  Lit les arguments et les transpose dans les variables globales
 * @note   
 * @param  argc: nombre d'arguments
 * @param  argv[]: chaîne des arguments
 * @retval None
 *
*/
void lecture_arguments(int argc, char * argv[]);

/**
 * @brief  Accepte les connexions entrantes
 * @note   L'objectif est d'assigner à sock_cible la valeur du socket
 *         de la connexion qu'on a établit. Le programme principal va ensuite
 *         repérer que ce socket n'est pas nul et va l'enregistrer de son côté.
 *         Dès que le socket_cible n'est plus nul, on peut lui réassigner une
 *         valeur.
 * @param  arg: le pointeur void * vers la structure contenant les variables
 *         suivantes:
 * @param  sock: socket qui accepte toutes les connexions
 * @param  csin: l'adresse qui accepte toutes les connexions
 * @param  sock_cible: pointeur vers un socket de retour
 * @retval NULL
 */
void * accepter_connexions_tcp(void * arg);

/**
 * @brief  Initialise le socket des connexions entrantes
 * @note   
 * @param  sock: pointeur vers le socket qui va être initialisé
 * @param  csin: pointeur vers l'adresse d'écoute qui va être initialisée
 * @retval None
*/
void initialiser_socket_connexions_tcp_entrantes(SOCKET * sock, SOCKADDR_IN * csin);

/**
 * @brief  Gère la connexion tcp avec un client
 * @note   S'arrête quand on reçoit un message de fin.
 *         S'occupe à la fois de l'émission et de la réception de messages
 * @param  arg: le pointeur void * vers la structure contenant 
 *              les variables suivantes:
 * @param  Train: la structure train du train
 * @retval NULL
 *
*/
void * maintenir_connexion_tcp(void * arg);

/**
 * @brief  Accepte les entrées de l'utilisateur pour les envoyer au programme principal
 * @note   Utiliser une fonction comme getline() ou getchar() qui tourne en boucle dans une 
 *         boucle while. A chaque information, on l'envoie grâce au pointeur au programme principal.
 * @param  arg: le pointeur vers un char *, qui permet de communiquer les entrées utilisateur
 *               au programme principal
 * @retval NULL
 *
*/
void * entrees_utilisateur(void * arg);


/**
 * @brief  Gère l'affichage
 * @note   Prends un pointeur vers un 
 * @param  arg: 
 * @retval 
 *
*/
void * affichage(void * arg);

/**
 * @brief  Fonction principale
 * @note    1) Lit les arguments et les traite
 *          2) Lance le thread entrees_utilisateur après avoir initialisé un char *
 *                  la taille du char * est définie à l'aide la constante MAX_BUFFER
 *          3) Ouvre le socket de connexions entrantes
 *          4) Ouvre un thread pour la fonction accepter_connexions_tcp()
 *          5) Lance une boucle while qui:
 *              → vérifie s'il y a une connexion entrante (int * socket_cible != NULL)
 *                  Initialise une nouvelle structure Train
 *                  Lance un nouveau thread de maintenir_connexion_tcp()
 *                  Remets le int * socket_cible à NULL
 *              → vérifie s'il y a des informations envoyées depuis les trains (dans les structures Train)
 *                  S'il y a du nouveau, éventuellement envoie des actions à effectuer aux threads
 *                  maintenir_connexion_tcp()
 *              → vérifie s'il y a une entrée de l'utilisateur (opérateur) entrees_utilisateur()
 *                  Réalise une action en conséquence
 * @param  argc: port du serveur –
 * @param  argv[]: 
 * @retval Code erreur
 *
*/
int main(int argc, char * argv[]);

#endif