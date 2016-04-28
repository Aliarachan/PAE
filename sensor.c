/*
 * sensor.c
 *
 *  Created on: 28/04/2016
 *      Author: mat.aules
 */

#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "comunication.h"
#include "definedValues.h"

int obstacleDetected(void){
	struct RxReturn r;
	byte aux = 0;
	int res = -1;

	byte bID = 100;
	byte bInstruction = INST_READ;
	byte bParameterLength = 2;
	byte gbpParameter[20];
	gbpParameter[0] = 0x20; // Obstacle detector reg
	gbpParameter[1] = 0x01; //length
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	r = RxPacket();

	// ERROR HANDLEING
	if(r.StatusPacket[4]){
		res = 4;
	}

	aux = r.StatusPacket[5];
	//Si les ultimes posicions son 0, llavors es que no ha trobat res
	if(!aux){
		res = 3;
	} else {
		//Si a la ultima posicio hi ha un 1, llavors
		//Ha trobat un obstacle a l'esquerra
		if (aux & 0x01){
			res = LEFT;
			//Si a les ultimes posicions hi ha un 10, llavors
			//Ha trobat un obstacle al mig
		} else if (aux & 0x02){
			res = FRONT;
			//Si a les ultimes posicions hi ha un 100, llavors
			//Ha trobat un obstacle a la dreta
		} else if (aux & 0x04){
			res = RIGHT;
		}
	}
	return res;
}

