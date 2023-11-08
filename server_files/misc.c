#include "misc.h"

void erreur(char message[]){
    if(AFFICHER_ERREURS==1)
        printf("%s",message);
    exit(-1);
}


void alerte(char message[]){
    if(AFFICHER_ALERTES==1)
        printf("%s",message);
    
}