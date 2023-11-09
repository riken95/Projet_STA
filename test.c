#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MAX_TAILLE_NOM_TRAIN 30              //Taille maximale du nom d'un train
#define MAX_TRAINS_LISTE MAX_CLIENTS  

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
#include "server.h"

//Librairies en plus
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

//Importation des headers
#include "misc.h"

typedef struct {
    double positionX, positionY, velocite, distanceSecurite;
    bool estActif;
    char nom[MAX_TAILLE_NOM_TRAIN];
    ThreadData * DonneesThread;
}Train;

Train * * Train_Liste_Initialiser(){
    return (Train * *) malloc(sizeof(Train *)*MAX_TRAINS_LISTE);
}

int Train_Liste_Ajouter_Train(Train ** liste, Train * train, int i){
    if(!liste) return -1;
    if(i>=MAX_TRAINS_LISTE) return -1;

    if(!liste[i]){
        liste[i] = train;
        return i;
    }

    return -1;
}


Train * Train_Initialiser(char * NomTrain){
    Train * NouveauTrain = (Train *) malloc(sizeof(Train));

    NouveauTrain->DonneesThread = (ThreadData*) malloc((sizeof(ThreadData)));
    struct Train_Communication_Messages * Donnees = 
    (struct Train_Communication_Messages *) malloc(sizeof(struct Train_Communication_Messages));
    NouveauTrain->DonneesThread->shared_data = Donnees;
    NouveauTrain->estActif = false;
    strcpy(NouveauTrain->nom,NomTrain);


    NouveauTrain->velocite,     NouveauTrain->distanceSecurite,
    NouveauTrain->positionX,    NouveauTrain->positionY 
        = 0.0;

    return NouveauTrain;
}


int main(int argc, char * argv[]){
  Train ** liste= Train_Liste_Initialiser();
  Train *train1=Train_Initialiser("train1");
  Train *train2=Train_Initialiser("train2");
  Train *train3=Train_Initialiser("train3");
  train1->positionX=10;
  train1->positionY=25;
  train1->velocite=5;
  train1->distanceSecurite=5;
  train1->estActif=true;

  train1->DonneesThread=malloc(sizeof(ThreadData));

  train2->positionX=8;
  train2->positionY=12;
  train2->velocite=3;
  train2->distanceSecurite=3;
  train2->estActif=true;

  train2->DonneesThread=malloc(sizeof(ThreadData));

  Train_Liste_Ajouter_Train(liste, train1, 0);
  printf("train1 : %s\n",liste[0]->nom);
  printf("train1 : %f\n",liste[0]->positionX);
  printf("train1 : %f\n",liste[0]->positionY);
  printf("train1 : %f\n",liste[0]->velocite);
  printf("train1 : %f\n",liste[0]->distanceSecurite);

  Train_Liste_Ajouter_Train(liste, train2, 1);
  printf("train2 : %s\n",liste[1]->nom);
  printf("train2 : %f\n",liste[1]->positionX);
  printf("train2 : %f\n",liste[1]->positionY);
  printf("train2 : %f\n",liste[1]->velocite);
  printf("train2 : %f\n",liste[1]->distanceSecurite);

  Train_Liste_Ajouter_Train(liste, train3, 2);
  printf("train3 : %s\n",liste[2]->nom);
  printf("train3 : %f\n",liste[2]->positionX);
  printf("train3 : %f\n",liste[2]->positionY);
  printf("train3 : %f\n",liste[2]->velocite);
  printf("train3 : %f\n",liste[2]->distanceSecurite);
  return 1;
}
