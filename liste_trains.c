#include "liste_trains.h"

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

void Train_Free(Train * Train){
    free(Train->DonneesThread->shared_data);
    free(Train->DonneesThread);
    free(Train);
}

double Train_distanceEuclidienne(Train * Train1, Train * Train2){
    if(!(Train1 && Train2)) return -1;
    
    return sqrt(pow(Train1->positionX-Train2->positionX,2)+pow(Train1->positionY-Train2->positionY,2));
}

Train * * Train_Liste_Initialiser(){
    return (Train * *) malloc(sizeof(Train *)*MAX_TRAINS_LISTE);
}

void Train_Liste_Free(Train * * liste){
    free(liste);
}

bool Train_Liste_Ajouter_Train(Train * * liste, Train * train){
    if(!liste) return false;

    for(int i=0;i<MAX_TRAINS_LISTE;i++)
    if(liste[i]){
        liste[i] = train;
        return true;
    }

    return false;
}

Train * Train_Liste_Retirer_Train(Train * * liste, int i){
    if(!liste || !liste[i]) return false;
    Train * Train = liste[i]; liste[i] = NULL;
    return Train;
}

void Train_Liste_Calculer_Distances_Securite(Train * * liste){
    if(!liste) return;

    for (int i=0;    i<MAX_TRAINS_LISTE-1;   i++)
    for (int j=i+1;  j<MAX_TAILLE_NOM_TRAIN; j++)
    if  (liste[i] && liste[j]){
        double distance = Train_distanceEuclidienne(liste[i],liste[j]);
        (liste[i]->distanceSecurite>distance)? liste[i]->distanceSecurite=distance: 0;
        (liste[j]->distanceSecurite>distance)? liste[j]->distanceSecurite=distance: 0;
    }
}

bool Train_envoyer_message(Train * Train, char message[], bool forcer_message){
    if(!Train) return false;

    pthread_mutex_lock(&Train->DonneesThread->mutex);
    char * message_actuel = ((struct Train_Communication_Messages *)Train->DonneesThread->shared_data)->InfosMain;
    
    //Si le message est déjà plein, renvoie false
    if(!forcer_message && strcmp(message_actuel,"")!=0)
        return false;
    
    strcpy(message_actuel,message);
    pthread_mutex_unlock(&Train->DonneesThread->mutex);
    return true;
}

bool Train_recevoir_message(char * dest, Train * Train){
    if(!Train) return false;

    pthread_mutex_lock(&Train->DonneesThread->mutex);
    char * message_actuel = ((struct Train_Communication_Messages *)Train->DonneesThread->shared_data)->InfosThread;
    
    if(strcmp(message_actuel,"")==0){
        pthread_mutex_unlock(&Train->DonneesThread->mutex);
        return false;
    }
    strcpy(dest, message_actuel);

    strcpy(message_actuel,"");//On vide le message une fois lu
    pthread_mutex_unlock(&Train->DonneesThread->mutex);

    return true;
}
