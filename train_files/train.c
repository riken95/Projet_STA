



/**
 * @brief  Fonction principale
 * @note    D'abord lit les arguments
 *          Ensuite initialise la connexion au serveur
 *          Ensuite avec un thread maintient la connexion avec le serveur
 *          Ensuite démarre la logique pour avancer
 *              Si la connexion au serveur s'arrête on relance une tentative de connexion
 *              ...
 * @param  argc: longueur des arguments
 * @param  argv[]: port du serveur - IP du serveur - nom du train
 * @retval 
 *
*/
int main(int argc, char * argv[]);