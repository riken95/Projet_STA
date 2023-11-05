#ifndef LISTE_TRAIN_H
#define LISTE_TRAIN_H
#define MAX_TAILLE_NOM_TRAIN 30              //Taille maximale du nom d'un train
#define MAX_TRAINS_LISTE MAX_CLIENTS         //Nombre maximum de trains dans la liste
#include "server.h"
#include <math.h>

// ======================== DEFINITION STRUCTURES ========================

struct Train{
    double positionX, positionY, velocite, distanceSecurite;
    bool estActif;
    char nom[MAX_TAILLE_NOM_TRAIN];
    ThreadData * DonneesThread;
};

typedef struct Train Train;

/**
 * @brief  Permet de communiquer entre le main et le thread de communication du train
 * @note   
 *
*/
struct Train_Communication_Messages{
    char InfosMain[MAX_BUFFER];
    char InfosThread[MAX_BUFFER];
};

// ======================== FONCTIONS TRAINS ========================


/**
 * @brief  Constructeur de la structure train
 * @note   
 * @param  NomTrain: Nom du train
 * @retval Pointeur vers le train (initialisé dynamiquement sur la pile)
 *
*/
Train * Train_Initialiser(char * NomTrain);

/**
 * @brief  Destructeur de la structure train
 * @note   
 * @param  Train: Pointeur vers le train à détruire
 * @retval None
 *
*/
void Train_Free(Train * Train);

double Train_distanceEuclidienne(Train * Train1, Train * Train2);

/**
 * @brief  Envoie un message au thread pour qu'il l'envoie au train
 * @note   Envoie depuis le main vers le thread
 * @param  Train: Pointeur vers le train
 * @param  message[]: Message à envoyer
 * @param  forcer_message: si est true, écrase un potentiel message en attente. Sinon, 
 * * la fonction ne fait rien s'il y a déjà un message en attente d'être envoyé
 * @retval true si le message a été écrit, false sinon
 *
*/
bool Train_envoyer_message(Train * Train, char message[], bool forcer_message);

/**
 * @brief  Vérifie s'il y a un nouveau message reçu. Si tel est le cas, le copie dans la variable dest
 * @note   Nettoie le message en attente une fois lu
 * @param  dest: Chaine de caractère où enregistrer le message
 * @param  Train: Pointeur vers le train
 * @retval True s'il y a un message non vide copié. False sinon
 *
*/
bool Train_recevoir_message(char * dest, Train * Train);

// ======================== FONCTIONS LISTE_TRAINS ========================


/**
 * @brief  Initialise la liste des trains
 * @note   
 * @retval Le pointeur vers le tableau statique alloué sur la pile.
 *         Le tableau a une taille de MAX_TRAINS_LISTE
 *
*/
Train * * Train_Liste_Initialiser();

void Train_Liste_Free(Train * * liste);

bool Train_Liste_Ajouter_Train(Train * * liste, Train * train);

Train * Train_Liste_Retirer_Train(Train * * liste, int i);

void Train_Liste_Calculer_Distances_Securite(Train * * liste);

#endif