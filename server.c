


//Importation des librairies
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close , pipe...*/
#include <netdb.h> /* gethostbyname */
#include <pthread.h>
#include <string.h>


//Constantes utiles pour le code
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;
#define MAX_BUFFER 256





/**
 * @brief  Définit une structure qui contient toutes les variables utiles
 *         pour gérer un train.
 * @note   Il faut y mettre:
 *          → le socket de la connexion
 *          → la position / vitesse du train
 *          → les pointeurs de communication avec le programme principal
 *              → Un pointeur du programme principal vers le Train
 *              → Un pointeur du train vers le programme principal
 *          → les pipe_fd pour les mutex_lock
 */
struct Train;

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
 * @param  sock_cible: pointeur vers un socket de retour
 * @retval NULL
 */
void * accepter_connexions_tcp(void * arg);



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
