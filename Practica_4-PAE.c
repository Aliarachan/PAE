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


char corner = 0;


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
		if (c){
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

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;       	// Paramos el watchdog timer

	/*Inicialitzem tot el necessari*/
	init_UCS();
	init_UART();
	init_timer_A1();
	init_timer_B0();
	init_LCD();
	enableInterruptTimerB0();
 	_enable_interrupt();


 	__delay_cycles(1000000); // Donem un poc de temps d'espera a que tot s'hagi inicialitzat correctament
	endLessTurn(); // Settejem les rodes en mode continu
	__delay_cycles(1000000);

	obstacleDistance(100);

	while(1){
		// Com a mostra de tot l'implementat a la practica executem la funcio que fa avancar el robot cap endavant evitant tot obstacle.
		followTheRightWall();
	}

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
		break;
		case (BUTTONS2):
		break;
		case (JOYSTICK_CENTER):
		break;
		case (JOYSTICK_LEFT):
		break;
		case (JOYSTICK_RIGHT):
		break;
		case (JOYSTICK_UP)://Joystick up
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
