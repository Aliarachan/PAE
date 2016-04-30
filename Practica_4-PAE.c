/*
 * Practica4.c
 *
 * PAE Practica 4: CONNEXIÓ AL ROBOT. Creació de les Funcions Bàsiques de Comunicació i d'una Llibreria de Funcions per Controlar el Robot.
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



/**
 * Funció que apaga el led del motor amb l'ID passat com a paràmetre.
 */
void ledOFF(byte bID){

	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 2; // Adreca inicial + Data-encendre-led(1)
	byte gbpParameter[20];
	gbpParameter[0] = P_LED;
	gbpParameter[1] = 0; //OFF

	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	RxPacket();


}

void ledON(byte bID){

	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 2; // Adreca inicial + Data-encendre-led(1)
	byte gbpParameter[20];
	gbpParameter[0] = P_LED;
	gbpParameter[1] = 1;

	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	RxPacket();


}
void main(void) {
	int f;
	WDTCTL = WDTPW + WDTHOLD;       	// Paramos el watchdog timer
	
	/*Inicialitzem tot el necessari*/
	init_UCS();
	init_UART();
	init_timer_A1();
	init_timer_B0();
	enableInterruptTimerB0();
 	_enable_interrupt();


 	__delay_cycles(1000000); // Donem un poc de temps d'espera a que tot s'hagi inicialitzat correctament
	endLessTurn(); // Settejem les rodes en mode continu
	__delay_cycles(1000000);

	while(1){
		// Com a mostra de tot l'implementat a la pràctica executem la funció que fa avançar el robot cap endavant evitant tot obstacle.
		moveObstacle();
	}

}


/*-------------------------------------------------------------------------
 *                  Rutines d'atenció a la interrupció (ISR)
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

