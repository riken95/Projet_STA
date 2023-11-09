#include "misc.h"
#include <stdio.h>

//Normalement ça ne sert à rien, c'est juste pour éviter une erreur dans VScode
#ifndef AFFICHAGE_ERREUR
#define AFFICHAGE_ERREURS 1
#endif



void erreur(char message[]){
    if(AFFICHAGE_ERREURS==1){
        printf("%s",message);
    }exit(-1);
}