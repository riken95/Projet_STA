#pragma once
#include <stdio.h>
#include <stdlib.h>

#ifndef MISC_H
#define MISC_H
#define AFFICHER_ERREURS 1
#define AFFICHER_ALERTES 1

/**
 * @brief  Permet d'afficher les erreur
 * @note    L'intérêt est qu'en modifiant la constante
 *          AFFICHER_ERREURS on peut directement actover 
 *          ou désactiver l'affichage des erreurs
 * @param  message[]: 
 * @retval None
 *
*/
void erreur(char message[]);

void alerte(char message[]);
#endif