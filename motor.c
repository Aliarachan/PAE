/*
 * motor.c
 * Fitxer que inclou les funcions relacionades amb l'enviament i recepció de paquets als moduls motors Dynamixel AX-12
 * per tal d'ajustar el mode de gir de les rodes.
 *  Created on: 28/04/2016
 *      Author: mat.aules
 */
#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "comunication.h"
#include "definedValues.h"
#include "sensor.h"

int timeMove = 0; // Variable comptador del temps que s'ha estat movent el robot cap a una determinada direcció.


/**
* Funció que mou una roda.
* Rep l'ID del motor de la roda que es preten moure i la velocitat (conté el sentit).
* Demana per escriure al registre indicador de velocitat del motor i rep el paquet de resposta.
**/
void moveWheel(byte ID, byte low, byte high){
	RxReturn rx; // Paquet de retorn
	byte bID = ID; //Motor ID
	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 3; // Registre on escriure , Velocitat part baixa , Velocitat part baixa
	byte gbpParameter[20];
	gbpParameter[0] = P_GOAL_SPEED_L; //moving speed
	gbpParameter[1] = low;
	gbpParameter[2] = high;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction); // Petició d'enviament
	rx = RxPacket(); // Recepció
	if (rx.error & 0x01) // Comprovem l'error
		_nop();

}

/**
* Funció que estableix els motors en mode continu per tal de poder moure les rodes només indicant la velocitat objectiu.
* Demana per escriure un 0 en tots els registres de configuració dels angles limit de tots el moduls Dynamixel connectats.
**/
void endLessTurn(void){
	byte bID = BROADCASTING_ID; // Amb aquesta id els packets enviats s'aplicaran a totes les unitats de Dynamixel. No es retornarà cap Status Packet.
	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 5; // Número de paràmetres (Direcció del 1r registre on escriure, 0 per a tots el registres de configuracio dels angles limit (4))
	byte gbpParameter[20];
	//The wheel mode can be used to wheel-type operation robots since motors of the robots spin infinitely.
	gbpParameter[0] = P_CW_ANGLE_LIMIT_L;
	gbpParameter[1] = 0; // Dada a escruiure a P_CW_ANGLE_LIMIT_L 
	gbpParameter[2] = 0; // Dada a escruiure a P_CW_ANGLE_LIMIT_H 
	gbpParameter[3] = 0; // Dada a escriure a  P_CW_ANGLE_LIMIT_L
	gbpParameter[4] = 0; // Dada a escriure a  P_CCW_ANGLE_LIMIT_H
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	//RxPacket();


}

/**
* Funció NO ACABADA que configura la velocitat dels motors per tal de que el robot es mogui cap a endavant.
* Demana per escriure la velocitat corresponent als registres de configuració de la velocitat a cadascun del moduls Dynamixel motor.
* En comptes d'enviar a cada motor individualment es preten escriure a tots a la vegada per tal de que es comencin a moure simultaneament. 
**/
void walk(void){
	byte bID = BROADCASTING_ID; // a més d'un actuador
	byte bInstruction = INST_SYNC_WRITE; //instruction write sincrona, una unica instrucció per a més d'un motor
	byte bParameterLength = 16; // (L + 1) * N + 4 (L: Data length for each Dynamixel actuator, N: The number of Dynamixel actuators)
	byte gbpParameter[20];
	gbpParameter[0] = P_GOAL_SPEED_L ;
	gbpParameter[1] = 2; //The length of the data to be written (L)
	gbpParameter[2] = 1; //The ID of the 1st Dynamixel actuator
	gbpParameter[3] = 1;
	gbpParameter[4] = 3;
	gbpParameter[5] = 1;
	gbpParameter[6] = 1;
	gbpParameter[7] = 1;
	gbpParameter[8] = 1;
	gbpParameter[9] = 1;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
}

/**
* Funció NO ACABADA que configura la velocitat dels motors per tal de que el robot s'aturi.
* Demana per escriure velocitat 0 als registres de configuració de la velocitat a cadascun del moduls Dynamixel motor.
* En comptes d'enviar a cada motor individualment es preten escriure a tots a la vegada per tal de que s'aturin simultaneament. 
**/
void quiet(void){
	byte bID = BROADCASTING_ID; // a més d'un actuador
	byte bInstruction = INST_SYNC_WRITE; //instruction write sincrona, una unica instrucció per a més d'un motor
	byte bParameterLength = 16; // (L + 1) * N + 4 (L: Data length for each Dynamixel actuator, N: The number of Dynamixel actuators)
	byte gbpParameter[20];
	gbpParameter[0] = P_GOAL_SPEED_L;
	gbpParameter[1] = 2; //The length of the data to be written (L)
	gbpParameter[2] = 1; //The ID of the 1st Dynamixel actuator
	gbpParameter[3] = 1;
	gbpParameter[4] = 1;
	gbpParameter[5] = 1;
	gbpParameter[6] = 1;
	gbpParameter[7] = 1;
	gbpParameter[8] = 1;
	gbpParameter[9] = 1;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);

}

/**
* Funció que envia a cadascuna de les rodes la velocitat adient per tal de que el robot es mogui cap enrerre.
* Rep com a parametre el temps que es vol que el robot es mogui cap enrere.
**/
void moveBackward(int time){
	moveWheel(1, 0xFF, 0x00);
	moveWheel(2, 0xFF, 0x00);
	moveWheel(3, 0xFF, 0x04);
	moveWheel(4, 0xFF, 0x04);
	if (time != 0){
		timeMove = 0; // Variable externa que modifica el timer.
		while (timeMove <= time);
	}
}

/**
* Funció que envia a cadascuna de les rodes la velocitat adient per a que el robot es mogui cap endavant.
* Rep com a parametre el temps que es vol que el robot es mogui cap endavant.
**/
void moveForward(int time){
	timeMove = 0;
	moveWheel(1, 0xFF, 0x04);
	moveWheel(2, 0xFF, 0x04);
	moveWheel(3, 0xFF, 0x00);
	moveWheel(4, 0xFF, 0x00);
	if (time != 0){
		timeMove = 0;
		while (timeMove <= time);
	}

}

/**
* Funció que envia a cadascuna de les rodes la velocitat adient per a que el robot es mogui cap a la dreta.
* Rep com a parametre el temps que es vol que el robot es mogui cap a la dreta.
**/
void turnRight(int time){
	moveWheel(1, 0xFF, 0x00);
	moveWheel(2, 0xFF, 0x00);
	moveWheel(3, 0xFF, 0x00);
	moveWheel(4, 0xFF, 0x00);
	if (time != 0){
		timeMove = 0;
		while (timeMove <= time);
	}
}

/**
* Funció que envia a cadascuna de les rodes la velocitat adient per a que el robot es mogui cap a l'esquerra.
* Rep com a parametre el temps que es vol que el robot es mogui cap a l'esquerra.
**/
void turnLeft(int time){
	moveWheel(1, 0xFF, 0x04);
	moveWheel(2, 0xFF, 0x04);
	moveWheel(3, 0xFF, 0x04);
	moveWheel(4, 0xFF, 0x04);
	if (time != 0){
		timeMove = 0;
		while (timeMove <= time);
	}
}

/**
* Funció que envia a cadascuna de les rodes la velocitat adient per a que el robot s'aturi.
* Rep com a parametre el temps que es vol que el robot s'aturi.
**/
void stopMoving(){
	moveWheel(1, 0x00, 0x00);
	moveWheel(2, 0x00, 0x00);
	moveWheel(3, 0x00, 0x00);
	moveWheel(4, 0x00, 0x00);
}

/**
* Funció fa moure el robot cap endavant sempre que no s'hagi detectat cap obstacle. En cas de detectar quelcom gira a pertinentment. 
**/
void moveObstacle(){
	int f;
	f = obstacleDetected(); // Hi ha obstacle, on?

	switch(f){
		case (LEFT):
			turnRight(0);
			break;
		case (FRONT):
			moveBackward(0);
			break;
		case (RIGHT):
			turnLeft(0);
			break;
		default:
			moveForward(0);
	}

}


