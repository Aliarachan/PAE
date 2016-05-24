/*
 * Practica4.c
 *
 * PAE Practica 4: CONNEXIO AL ROBOT. Creacio de les Funcions Basiques de Comunicacio i d'una Llibreria de Funcions per Controlar el Robot.
 *
 *  Created on: 31/03/2016
 *      Author: Aina Ferra i Alicia Morales
 */

#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "init.h"
#include "comunication.h"
#include "sensor.h"
#include "motor.h"
#include "definedValues.h"
#include "pantalla.h"
#include "extres.h"


char corner = 0;
int currIndexMelody = 0;
char setMin = 0;
char setMax = 0;
char doneMin = 0;
char doneMax = 0;
char wall = 0;
char move = 0;
char string[17];

void followTheRightWall(){
	volatile int f, c;
	c = 0;
	f = obstacleDetected();

	if(f == ERROR){
		__no_operation();
	} else if ((f & 2) == 2){
		borrar(1);
		escribir("FRONT", 1);
		turnHLeft(0);
	} else if ((f & 4) == 4){
		borrar(1);
		escribir("RIGHT", 1);
		c = closeRight();
		if (c && c!= ERROR){
			borrar(1);
			escribir("RIGHT CLOSE", 1);
			turnLeft(300);
			moveForward(500);
		}
		moveForward(0);
	} else {
		borrar(1);
		escribir("ELSE", 1);
		borrar(2);
		escribir("No detecto nada", 4);
		turnRight(300);
		moveForward(500);
	}
	borrar(4);

}

void followTheLeftWall(){
	volatile int f, c;
	c = 0;
	f = obstacleDetected();

	if(f == ERROR){
		__no_operation();
	} else if ((f & 2) == 2){
		borrar(1);
		escribir("FRONT", 1);
		turnHRight(0);
	} else if ((f & 1) == 1){
		borrar(1);
		escribir("LEFT", 1);
		c = closeLeft();
		if (c && c!= ERROR){
			borrar(1);
			escribir("LEFT CLOSE", 1);
			turnRight(300);
			moveForward(500);
		}
		moveForward(0);
	} else {
		borrar(1);
		escribir("ELSE", 1);
		borrar(2);
		escribir("No detecto nada", 4);
		turnLeft(300);
		moveForward(500);
	}
	borrar(4);

}

void searchWall(){
	int f;
	while (!wall){
		f = obstacleDetected();
		if (f == ERROR){
			moveForward(0);
		} else if ((f & 2) == 2){
			turnHLeft(500);
			wall = RIGHT_WALL;
		} else if ((f& 4) == 4){
			moveForward(500);
			wall = RIGHT_WALL;
		} else if ((f&1) == 1){
			moveForward(500);
			wall = LEFT_WALL;
		}
		moveForward(0);
	}
}

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;       	// Paramos el watchdog timer

	/*Inicialitzem tot el necessari*/
	init_UCS();
	init_UART();
	init_timer_A1();
	init_timer_B0();
	init_LCD();
	init_botons();
	config_P4_LEDS();
	enableInterruptTimerB0();
 	_enable_interrupt();


 	__delay_cycles(1000000); // Donem un poc de temps d'espera a que tot s'hagi inicialitzat correctament
	endLessTurn(); // Settejem les rodes en mode continu
	__delay_cycles(1000000);
	escribir("hola",2);
	while(1){

	playNyanMelody(currIndexMelody++);
		if(currIndexMelody > 454)
		{
			currIndexMelody = 0;
		}
	}
	obstacleDistance(100);

	while(!(setMin && setMax)){
		int c;
		c = readRightSensor(100);
		sprintf(string, "Distance %.3d", c);
		escribir(string, 1);
		if (setMin && !doneMin){
			setMinDistance();
			doneMin = 1;
			borrar(3);
			escribir("Min distance", 2);
		}
		if (setMax && !doneMax){
			setMaxDistance();
			doneMax = 1;
			borrar(2);
			escribir ("Max distance", 3);
		}
	}


	while(!move){
		__no_operation();
	}
	borrar(2);
	borrar(3);
	escribir("Buscando...", 2);
	searchWall();
	if (wall == RIGHT_WALL){
		borrar(2);
		escribir("Right wall", 2);
		while(move){
			followTheRightWall();
			playBuzzMelody();
		}
	} else if (wall == LEFT_WALL){
		borrar(2);
		escribir("Left wall", 2);
		while(move){
			followTheLeftWall();
			playBuzzMelody();
		}
	}

	stopMoving();

	/*
	while(1){

		// Com a mostra de tot l'implementat a la practica executem la funcio que fa avancar el robot cap endavant evitant tot obstacle.
		playNyanMelody(currIndexMelody++);
		if(currIndexMelody > 454)
		{
			currIndexMelody = 0;
		}
	} */


}


/*-------------------------------------------------------------------------
 *                  Rutines d'atencio a la interrupcio (ISR)
 * ------------------------------------------------------------------------*/

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) { //interrupcion de recepcion en la uart A0
	UCA0IE &= ~UCRXIE; //Interrupciones desactivadas en RX
	readDataUART = UCA0RXBUF; //Agafem del bus la data que hem de llegir
	receivedByte = 1; //Marquem al flag que ja hem rebut el byte
	UCA0IE |= UCRXIE; //Interrupciones reactivadas en RX
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void timerA1_ISR(void) {
	timeNow += 1;
}

#pragma vector=TIMERB0_VECTOR
__interrupt void timerB0_ISR(void) {
	timeMove += 1;
}

#pragma vector=PORT2_VECTOR  //interrupción de los botones. Actualiza el valor de la variable global estado.
__interrupt void Port2_ISR(void)
{
	P2IE &= 0xC0; 	//interrupciones botones S1 y S2 (P2.6 y P2.7) desactivadas
	P2IE &= 0x3E;   //interrupciones joystick (2.1-2.5) desactivadas
	switch(P2IFG){
		case (BUTTONS1):
			setMax = 1;
		break;
		case (BUTTONS2):
			setMin = 1;
		break;
		case (JOYSTICK_CENTER):

		break;
		case (JOYSTICK_LEFT):
		break;
		case (JOYSTICK_RIGHT):
		break;
		case (JOYSTICK_UP)://Joystick up
		if (move){
			move = 0;
		} else {
			move = 1;
		}
		break;
		case (JOYSTICK_DOWN):
		break;
		default:
		break;
	}

	P2IFG = 0;		//limpiamos todas las interrupciones
	P2IE |= 0xC0; 	//interrupciones botones S1 y S2 (P2.6 y P2.7) reactivadas
	P2IE |= 0x3E;  //interrupciones joystick (2.1-2.5) reactivadas
 return;
}
