/*
 * sensor.c
 * Fitxer que conté les funcions que demanen la resposta dels transductors (sensor)
 *  Created on: 28/04/2016
 *      Author: Aina Ferrà i Alicia Morales
 */

#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "comunication.h"
#include "definedValues.h"

/**
* Funció que llegeix el flag del modul sensor indicador de si hi ha obstacle i on es troba.
* Retorna un enter indicant 
* -1 si no hi ha res (el byte rebut acaba amb 000), 
* 0 si ha trobat un obstacle a l'esquerra (el byte rebut acaba amb 100),
* 2 si ha trobat un obstacle a la dreta (el byte rebut acaba amb 010),
* 1 si ha trobat un obstacle a front (el byte rebut acaba amb 100),
* 4 si s'ha rebut un error en el packet.
*
**/
int obstacleDetected(void){
	struct RxReturn r; // l'estructura que rebrem 
	byte aux = 0; // byte on es guardara el parametre de l'estructura que conté la informació de si hi ha o no obstacles i on
	int res = -1; // enter que indica on es l'obstacle, per defecte no n'hi ha cap

	byte bID = 100; // l'ID del sensor
	byte bInstruction = INST_READ; // volem llegir el flag d'obstacle
	byte bParameterLength = 2; // Registre a llegir + longitud de les dades
	byte gbpParameter[20];
	gbpParameter[0] = 0x20; // Obstacle detector reg direction
	gbpParameter[1] = 0x01; //length
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	r = RxPacket(); // Paquet retornat

	// ERROR HANDLEING
	if(r.StatusPacket[4]){
		res = 4; // s'ha trobat un error en el packet
	}

	aux = r.StatusPacket[5]; // dada del paquet que conté la informació de si hi ha obstacle i on
	//Si les ultimes posicions son 0, llavors es que no ha trobat res
	if(!aux){ // OBSTACLE FOUND?
		res = 3;
	} else {
		//Si a la ultima posicio hi ha un 1, llavors
		//Ha trobat un obstacle a l'esquerra
		if (aux & 0x01){ //YES!!! LOOK LEFT!
			res = LEFT;
			//Si a les ultimes posicions hi ha un 10, llavors
			//Ha trobat un obstacle al mig
		} else if (aux & 0x02){// ¬¬!!! IT'S IN FRONT OF YOU!!!
			res = FRONT;
			//Si a les ultimes posicions hi ha un 100, llavors
			//Ha trobat un obstacle a la dreta
		} else if (aux & 0x04){//YES!! AT YOUR 3!!
			res = RIGHT;
		}
	}
	return res;
}

