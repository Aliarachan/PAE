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


char borrado[] = "                 ";


void escribir(char String[], unsigned char Linea)

{
	halLcdPrintLine(String, Linea, OVERWRITE_TEXT); //superponemos la nueva palabra introducida, haya o no algo.
}

void borrar(unsigned char Linea) {
	halLcdPrintLine(borrado, Linea, OVERWRITE_TEXT); //incluimos una linea en blanco
}



/**
* Funció que llegeix el flag del modul sensor indicador de si hi ha obstacle i on es troba.
* Retorna un enter indicant 
* -1 si no hi ha res (el byte rebut acaba en 000),
* 0 si ha trobat un obstacle a l'esquerra (el byte rebut acaba en 100),
* 2 si ha trobat un obstacle a la dreta (el byte rebut acaba en 010),
* 1 si ha trobat un obstacle a front (el byte rebut acaba en 100),
* 4 si s'ha rebut un error en el packet.
*
**/
int obstacleDetected(void){
	struct RxReturn r; // l'estructura que rebrem 
	byte aux = 0; // byte on es guardara el parametre de l'estructura que conte la informacio de si hi ha o no obstacles i on
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
	if((r.StatusPacket[4] & 16) || (r.error)){
		res = -2; // s'ha trobat un error en el packet
		return res;
	}

	aux = r.StatusPacket[5]; // dada del paquet que conté la informació de si hi ha obstacle i on
	//Si les ultimes posicions son 0, llavors es que no ha trobat res
	if(!aux){ // OBSTACLE FOUND?
		res = NO_OBSTACLE;
	}

	aux &= 0x07;
	/**
	switch(aux){
		case (0x01):
			res = LEFT;
			break;
		case (0x02):
			res = FRONT;
			break;
		case (0x04):
			res = RIGHT;
			break;
		case (0x03):
			res = LEFT_FRONT;
			break;
		case (0x05):
			res = LEFT_RIGHT;
			break;
		case (0x06):
			res = FRONT_RIGHT;
			break;
		case (0x07):
			res = LEFT_FRONT_RIGHT;
			break;
	} **/
	return aux;
}

/**
* Funcio per poder llegir la distancia a la que es troba un obstacle a l'esquerra.
* Reb la ID del sensor d'on llegim i ens torna un valor numeric que interpretem com
* la reflexio de la llum a l'objecte que detectem.
**/
int readLeftSensor(byte bID){
	struct RxReturn r;
	int res;
	byte bInstruction = INST_READ;
	byte bParameterLength = 2;
	byte gbpParameter[20];
	gbpParameter[0] = LEFT_IR_SENSOR; //Quin sensor llegim
	gbpParameter[1] = 1; //Quantes dades llegim.

	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	r = RxPacket();

	// ERROR HANDLEING
	if((r.StatusPacket[4] & 16) || (r.error)){
		res = ERROR; // s'ha trobat un error en el packet
		return res;
	}

	//Retornem el valor que hem llegit
	//Nomes arribem aqui si no hi ha un error
	//Es troba a la posicio 5, la seg�ent despr�s de la trama
	//que ja coneixem
	return r.StatusPacket[5];
}


/**
* Funcio per poder llegir la distancia a la que es troba un obstacle a la dreta.
* Reb la ID del sensor d'on llegim i ens torna un valor numeric que interpretem com
* la reflexio de la llum a l'objecte que detectem.
**/
int readRightSensor(byte bID){
	struct RxReturn r;
	int res;
	byte bInstruction = INST_READ;
	byte bParameterLength = 2;
	byte gbpParameter[20];
	gbpParameter[0] = RIGHT_IR_SENSOR; //Quin sensor llegim
	gbpParameter[1] = 1; //Quantes dades llegim.

	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	r = RxPacket();

	// ERROR HANDLEING
	if((r.StatusPacket[4] & 16) || (r.error)){
		res = ERROR; // s'ha trobat un error en el packet
		return res;
	}

	return r.StatusPacket[5];

}

/**
* Funcio que mira si algun obstacle que detectem esta mes aprop que un valor definit.
* Sense parametres d'entrada.
* Retorna un char amb valor 1 si esta massa aprop, valor 0 si no.
**/
int closeRight(){
	int c;
	char cadena[17];
	c = readRightSensor(100);
	if (c == ERROR){
		return 0;
	}

	sprintf(cadena, "Valor: %d", c);
	escribir(cadena, 2);
	//El valor que llegim es la reflexo de la llum sobre l'obstacle.
	//Si no es reflexa llum, llavors es que no hi ha obstacle.
	//Si reflexa molta llum, es que l'obstacle esta a prop.
	//(Sabent que aixo depen del material sobre el que incideix la llum)
	//Per tant, quant mes alt es el valor, mes aprop esta l'obstacle
	if (c > TOOCLOSE){
		escribir("Estoy muy cerca", 3);
		return 1;
	}else{
		return 0;
	}
}

/**
* Funcio que mira si algun obstacle detectat a l'esquerra esta mes aprop que un valor definit.
* Sense parametres d'entrada.
* Retorna un char amb valor 1 si esta massa aprop i 0 si no.
**/
int closeLeft(){
	int c;
	char cadena[17];
	c = readLeftSensor(100);

	sprintf(cadena, "Valor: %d", c);
	borrar(2);
	escribir(cadena, 2);
	if (c == ERROR){
		return ERROR;
	}
	if (c > 200){
		escribir("Estoy muy cerca", 3);
		return 1;
	}else{
		return 0;
	}
}

void obstacleDistance(byte bID){

	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 2; // Adreca inicial + Data-encendre-led(1)
	byte gbpParameter[20];
	gbpParameter[0] = 0x34; //Obstacle detection compare
	gbpParameter[1] = 15; //OFF

	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	RxPacket();


}

int playBuzzMelody(void){
	RxReturn rx; // Paquet de retorn
	byte bID = 100; 
	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 3; // Registre on escriure , BUZZER part baixa , BUZZER part baixa
	byte gbpParameter[20];
	gbpParameter[0] = 0X28; //BUZZER NOTES
	gbpParameter[1] = 0;
	gbpParameter[2] = 255;
	rx = RxPacket(); // Recepcio
}

void setWallKind(void){
	obstacleDistance(readRightSensor()-50);
}

char clapWaiting(void){
	
}

