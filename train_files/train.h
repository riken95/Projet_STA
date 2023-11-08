#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close , pipe...*/
#include <netdb.h> /* gethostbyname */
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "marvelmind.h"

struct Train {
  char * name;
  float distance_a_parcourir;
  int x;
  int y;
  float V_actu;
  float V_consigne;
  int code_erreur;
  
}; //struct avec tout les infos qu'on doit connaitre sur le train

struct CommunicationVersServeur {

  int x;
  int y;
  float V_actu;
  int code_erreur;
};

struct CommunicationServeur {

  float distance_a_parcourir;
  float V_consigne;
  int code_erreur;
};

