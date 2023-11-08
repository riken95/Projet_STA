/**
  @Generated PIC24 / dsPIC33 / PIC32MM MCUs Source File

  @Company:
    Centrale Lille

  @File Name:
    MESCAN1_Utilitaire.h

  @Summary:
    This is the MESCAN1_Utilitaire.h file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  @Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : 1.75.1
        Device            :  dsPIC33FJ128MC802
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.35
        MPLAB             :  MPLAB X v5.05

 * Created on 21 novembre 2018
 */


#ifndef UTILITAIRECAN_H
#define	UTILITAIRECAN_H

#ifndef MC802
	#define MC802 1
#endif

#if MC802 ==1
	#define CAN_SFF_MASK 0x7FF
#endif

/**
  Section: Included Files
 */
#include <stdbool.h>
#include <stdint.h>
#include "MESCAN1_DefinitionVariable.h"


#define HIGH    1
#define LOW     0

#define OFFSET_DATA0_TO_7   7

#define TailleFIFO 	20
    
typedef	struct fileReceiveMesCAN 
{
	int tete_FileCAN;
	int queue_FileCAN;
	int	NbTransaction_FileCAN;
	uCAN1_MSG fileValue_FileCAN[TailleFIFO];
} iFILE_MesCAN;



/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description: Positionne "High" un bit particulier d'une data d'une trame CAN
 * MC_Bit est definit de la fa�on suivante
 *      typedef struct {
            uint32_t IdValue;       //  Identifiant de la trame CAN
            uint16_t IndiceData;    //  Num�ro de la data dans le message CAN (de 0 � 7)
            uint16_t IndiceBit;     //  Num�ro du bit dans la data message CAN (de 0 � 7)
        } MC_Bit; 
 * Cette fonction v�rifie la coh�rence de MC_Bit (bien �t� d�finit dans la bonne trame CAN)
 * C�d qu'il v�rifie que l'Id correspond bien � la variable bit d�clar�e
 * Dans le cas contraire, une variable globale "cptErreurDefinitionVar" sera
 * incr�ment�e.
 *    Arguments:		message CAN "mc_xxxx"  et la structure data du bit 
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_SetBitHigh(uCAN1_MSG *CanMsg, MC_Bit DefBit);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_SetBitLow(uCAN1_MSG *CanMsg, MC_Bit DefBit);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_SetBit(uCAN1_MSG *CanMsg, MC_Bit DefBit, bool valbit);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
bool MESCAN_GetBit(uCAN1_MSG *CanMsg, MC_Bit DefBit);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
uint8_t MESCAN_GetData8(uCAN1_MSG *CanMsg, MC_Data8 DefData);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_SetData8(uCAN1_MSG *CanMsg, MC_Data8 DefData, uint8_t Data);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_DecrementeDataToZero(uCAN1_MSG *CanMsg, MC_Data8 DefData);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_SetData16(uCAN1_MSG *CanMsg, MC_Data16 DefData16, uint16_t Data16);

/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
uint16_t MESCAN_GetData16(uCAN1_MSG *CanMsg, MC_Data16 DefData16);

/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_SetData32(uCAN1_MSG *CanMsg, MC_Data32 DefData32, uint32_t Data32);

/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
uint32_t MESCAN_GetData32(uCAN1_MSG *CanMsg, MC_Data32 DefData32);

/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_SetDataFloat32(uCAN1_MSG *CanMsg, MC_Datafloat32 DefDataFloat32, float ValFloat);

/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
float MESCAN_GetDataFloat32(uCAN1_MSG *CanMsg, MC_Datafloat32 DefDataFloat32);

/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_InitAndRAZ_Alldatas(uCAN1_MSG * MesCAN);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_DupliqueMessageWithDLC(uCAN1_MSG * SrcMesCAN, uCAN1_MSG * DestMesCAN);


/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_DupliqueMessageSaufDLC(uCAN1_MSG * SrcMesCAN, uCAN1_MSG * DestMesCAN);



/******************************************************************************
 *                                                                             
 *    Function:		
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
bool MESCAN_isDataTrame(uCAN1_MSG * MesCAN);



/*  Exemple d'utilisation de la FIFO.
 * On d�clare une variable type FILE "maFIFO_essai": 
 *      iFILE_MesCAN maFIFO_essai;
 * 
 * Il faut l'initialiser:
 *      MESCAN_InitFIFO(&maFIFO_essai);
 * 
 * Pour enfiler les messages:
 *      MESCAN_enfiler(&maFIFO_essai, msgReceiveFIFO); //Avec par exemple msgReceiveFIFO
 *      comme message CAN re�u.
 * 
 * Avant de d�filer, il faut s'assurer que la file n'est pas vide: Exemple
 *         if (! MESCAN_isFileVide(&maFIFO_essai)) {
            DesabledInterrupt();
            MESCAN_defiler(&maFIFO_essai, &msgCAN_essai);
            EnabledInterrupt();
        }
 * On a r�cup�r� un message CAN nomm� "msgCAN_essai" de la file
 */

/******************************************************************************
 *                                                                             
 *    Function:	void MESCAN_InitFIFO (iFILE_MesCAN *F);	
 *    Description:   Initialisation FILE
 *    Arguments:	 Nom de la file � initialiser.  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
void MESCAN_InitFIFO (iFILE_MesCAN *F);


/******************************************************************************
 *                                                                             
 *    Function:	int MESCAN_isFileVide(iFILE_MesCAN *F);	
 *    Description:   
 *    Arguments:		None  
 *    Return Value:  None
 *                                                                                                   
 ******************************************************************************/
int MESCAN_isFileVide(iFILE_MesCAN *F);


/******************************************************************************
 *                                                                             
 *    Function:bool MESCAN_enfiler(iFILE_MesCAN *F, uCAN1_MSG myReceiveMesCAN);		
 *    Description:   
 *    Arguments:		Nom de la file
 *                      Nom du message CAN � enfiler  
 *    Return Value:     true, Operation OK
 *                      false quand on a atteint la taille maximum de la file, donc
 *                      le message CAN n'est pas enfil�.
 *                                                                                                   
 ******************************************************************************/
bool MESCAN_enfiler(iFILE_MesCAN *F, uCAN1_MSG myReceiveMesCAN);


/******************************************************************************
 *                                                                             
 *    Function:bool MESCAN_defiler(iFILE_MesCAN *F, uCAN1_MSG *myDefileMesCAN);		
 *    Description:   
 *    Arguments:		Nom de la file
 *                      Nom du message CAN � d�filer  
 *    Return Value:     true, Operation OK
 *                      false quand il n'y a plus rien � d�filer
 *                                                                                                   
 ******************************************************************************/
bool MESCAN_defiler(iFILE_MesCAN *F, uCAN1_MSG *myDefileMesCAN);


uint8_t MESCAN_generateChecksum(uint8_t* messageArray, int16_t messageLength);



	/// Reconstruit un message plus grand a partir de trames CAN.
	/** Le message re�u est stock� dans un tableau, et v�rifie son int�grit� selon la longueur donn�e et le checksum re�u <BR>
	Retourne:<BR> 
		\li 0 si la reconstruction n'est pas finie
		\li 1 quand c'est fini
		\li -1 Si la trame CAN n'appartient pas a la plage correspondante a la longueur de message (a traiter selon contexte)
		\li -2 Si la taille specifiee et les identifiants et dlc de trame CAN calcules ne correspondent pas
		\li -3 Si la trame CAN a deja ete recue (a traiter selon contexte)
		\li -4 Si le checksum calcule et recu ne correspondent pas<BR>
	Traite une seule trame CAN a la fois, elles peuvent arriver dans le desordre */
int16_t MESCAN_reconstructMessage(uCAN1_MSG *recCanMsg, uint8_t* destinationArray, uint32_t startIDFrame);

	/// Reconstruit un message plus grand a partir de trames CAN.
	/** Fait appel a MESCAN_reconstructMessage, le nom sert a differencier 2 streams de trames CAN differents */
int16_t MESCAN_reconstructMessageA(uCAN1_MSG *recCanMsg, uint8_t* destinationArray, uint32_t startIDFrame);

	/// Reconstruit un message plus grand a partir de trames CAN.
	/** Comportement exact a  MESCAN_reconstructMessage, destinee a gerer 2 streams de trames CAN independants*/
int16_t MESCAN_reconstructMessageB(uCAN1_MSG *recCanMsg, uint8_t* destinationArray, uint32_t startIDFrame);

	///Decoupage d'un tableau en trames CAN
	/** Genere les trames CAN qui transmettront les differentes parties du tableau, avec calcul automatique de leur format (ID, dlc et donnees)<BR>
		Retourne true quand le decoupage du message est fini (arrive a la fin du tableau)<BR>
		a chaque appel a la fonction, remplit la valeur de sendCanMsg pour l'envoyer par la suite */
bool MESCAN_sliceArray(uCAN1_MSG *sendCanMsg, uint8_t* dataArray, uint32_t startIDFrame, int16_t messageLength, uint8_t id_Standard_or_Extend);

uint16_t MESCAN_Get_cptErreurDefinition(void);

void MESCAN_Inc_cptErreurDefinition(void);



#endif	/* UTILITAIRECAN_H */


/**
 End of File
 */

