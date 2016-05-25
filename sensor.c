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
#include "pantalla.h"
#include "pitches.h"

//notes in the melody:
const char melody[459] = {
  NOTE_DS5, NOTE_E5, NOTE_FS5, 0, NOTE_B5, NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_FS5, NOTE_B5, NOTE_DS6, NOTE_E6, NOTE_DS6, NOTE_AS5, NOTE_B5, 0,
  NOTE_FS5, 0, NOTE_DS5, NOTE_E5, NOTE_FS5, 0, NOTE_B5, NOTE_CS6, NOTE_AS5, NOTE_B5, NOTE_CS6, NOTE_E6, NOTE_DS6, NOTE_E6, NOTE_CS6,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_B3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_AS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_D4, NOTE_DS4, NOTE_FS2, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_CS4, NOTE_B3,
  NOTE_DS4, NOTE_FS4, NOTE_GS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_D4, NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4,
  NOTE_D4, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_FS4, NOTE_CS4, NOTE_D4, NOTE_CS4, NOTE_B3, NOTE_CS4, NOTE_B3, NOTE_B3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_AS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4,
  NOTE_B3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_E4, NOTE_DS4, NOTE_CS4, NOTE_B3, NOTE_E3, NOTE_DS3, NOTE_E3, NOTE_FS3,
  NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_B3, NOTE_CS4, NOTE_DS4, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_FS3,
  NOTE_B3, NOTE_B3, NOTE_AS3, NOTE_B3, NOTE_FS3, NOTE_GS3, NOTE_B3, NOTE_E4, NOTE_DS4, NOTE_E4, NOTE_FS4, NOTE_B3, NOTE_CS4
};

// note durations: 4 = quarter note, 8 = eighth note, etc
const char noteDurations[459] =  {
  3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  3,3,3,3,3,3,6,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,6,6,6,
  6,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,6,6,6,
  6,6,3,3,3,3,3,3,6,6,6,
  6,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,6,6,6,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,3,3,6,6,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,3,3,6,6,
  6,6,3,3,3,3,3,3,6,6,6,
  6,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,6,6,6,
  6,6,3,3,3,3,3,3,6,6,6,
  6,3,3,3,3,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,6,6,6,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,3,3,6,6,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,6,3,3,3,3,3,3,3,3,3,3,3,3,
  6,3,3,6,3,3,3,3,3,3,3,3,3,3,
  6,3,3,3,3,3,3,3,3,3,3,6,6
};

int minDistance = 0x50;

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
	//Es troba a la posicio 5, la seguent despres de la trama
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
		return ERROR;
	}
	//El valor que llegim es la reflexo de la llum sobre l'obstacle.
	//Si no es reflexa llum, llavors es que no hi ha obstacle.
	//Si reflexa molta llum, es que l'obstacle esta a prop.
	//(Sabent que aixo depen del material sobre el que incideix la llum)
	//Per tant, quant mes alt es el valor, mes aprop esta l'obstacle
	if (c > minDistance){
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
	if (c == ERROR){
		return ERROR;
	}
	if (c > minDistance){
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

void playBuzzMelody(void){
	RxReturn rx; // Paquet de retorn
	byte bID = 100;
	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 3; // Registre on escriure , BUZZER part baixa , BUZZER part baixa
	byte gbpParameter[20];
	gbpParameter[0] = 0X28; //BUZZER NOTES
	gbpParameter[1] = 0;
	gbpParameter[2] = 255;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	rx = RxPacket(); // Recepcio
}

void playNyanMelody(int position){
	RxReturn rx; // Paquet de retorn
	byte bID = 100;
	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 3; // Registre on escriure , BUZZER part baixa , BUZZER part baixa
	byte gbpParameter[20];
	gbpParameter[0] = 0X28; //BUZZER NOTES
	gbpParameter[1] = melody[position];
	gbpParameter[2] = noteDurations[position]; //(10/noteDurations[position])*10;
	//gbpParameter[2] = 254;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	rx = RxPacket(); // Recepcio
}

/*
* Funcio que seteja la distancia maxima a la qual podem estar de la pared.
* Per distancies mes grans que aquesta, el robot no detectara cap obstacle
*/
void setMaxDistance(void){
	int c;
	//Llegim el valor del sensor
	c = readRightSensor(100);
	//Si ens dona error ho reintentem.
	//Es poc probable que doni error dos cops seguits.
	if (c == ERROR){
			c = readRightSensor(100);
			obstacleDistance(c);
	} else {
		obstacleDistance(c);
	}
}

/*
* Funcio que seteja la distancia minima a la qual estarem de la pared.
* El robot voldra recolocarse una mica mes lluny quan estem mes a prop que
* aquesta distancia.
*/
void setMinDistance(void){
	int c;
	c = readRightSensor(100);
	//Aqui fem el mateix, ho provem dos cops perque es poc
	//probable que doni error dos cops seguits.
	if (c == ERROR){
			c = readRightSensor(100);
			minDistance = c;
	} else {
		minDistance = c;
	}

}


/**
* Funció que llegeix el flag del modul sensor indicador de si es supera un umbral de lluminositat.
* Retorna un enter indicant si hem passat el llindar.
* Tractem el cas de que hi hagi un error.
* En cas contrari, nomes volem saber si ha detectat foc en algun sensor. Ens es igual on.
**/
int isFire(void){
	struct RxReturn r; // l'estructura que rebrem
	byte aux = 0; // byte on es guardara el parametre de l'estructura que conte la informacio de si hi ha o no obstacles i on
	int res = -1; // enter que indica on es el foc, per defecte no n'hi ha cap

	byte bID = 100; // l'ID del sensor
	byte bInstruction = INST_READ; // volem llegir el flag d'obstacle
	byte bParameterLength = 2; // Registre a llegir + longitud de les dades
	byte gbpParameter[20];
	gbpParameter[0] = 0X21; // Luminosity detector reg direction
	gbpParameter[1] = 0x01; //length
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	r = RxPacket(); // Paquet retornat

	// ERROR HANDLEING
	if((r.StatusPacket[4] & 16) || (r.error)){
		return ERROR;
	}

	 return r.StatusPacket[5]; // dada del paquet que conté la informació de si hi ha foc i on
}

char clapCount(void){
  struct RxReturn r; // l'estructura que rebrem
	byte aux = 0;

	byte bID = 100; // l'ID del sensor
	byte bInstruction = INST_READ; //Volem llegir quants sons forts s'han sentit
	byte bParameterLength = 2; // Registre a llegir + longitud de les dades
	byte gbpParameter[20];
	gbpParameter[0] = CLAP_COUNTER; //Llegim la direccio de mem corresponent al comptador de sons.
	gbpParameter[1] = 0x01; //Nomes llegim un valor.
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	r = RxPacket(); // Paquet retornat

  if((r.StatusPacket[4] & 16) || (r.error)){
    return ERROR;
  }

  //Retornem el comptador que tingui
  return r.StatusPacket[5];
}
