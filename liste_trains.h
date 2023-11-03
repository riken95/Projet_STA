#ifndef LISTE_TRAIN_H
#define LISTE_TRAIN_H
#define MAX_TAILLE_NOM_TRAIN 30              //Taille maximale du nom d'un train
#define MAX_TRAINS_LISTE MAX_CLIENTS         //Nombre maximum de trains dans la liste
#include "server.h"

struct Train{
    double positionX, positionY, velocite, distanceSecurite;
    bool estActif;
    char nom[MAX_TAILLE_NOM_TRAIN];
    ThreadData * DonneesThread;
};

typedef struct Train Train;


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

// ======================== FONCTIONS LISTE_TRAINS ========================


/**
 * @brief  Initialise la liste des trains
 * @note   
 * @retval Le pointeur vers le tableau statique alloué sur la pile.
 *         Le tableau a une taille de MAX_TRAINS_LISTE
 *
*/
Train * Train_Liste_Initialiser();

void Train_Liste_Free(Train * * liste);

bool Train_Liste_Ajouter_Train(Train * * liste, Train * train);

Train * Train_Liste_Retirer_Train(Train * * liste, int i);

void Train_Liste_Calculer_Distances_Securite(Train * * liste);

#endif