/*
 * init.c
 * Fitxer amb les funcions d'inicialització dels moduls de comunicació i timers.
 *  Created on: 28/04/2016
 *      Author: Aina Ferra i Alicia Morales
 */

#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "definedValues.h"

/**
* Funció d'inicialització del UCS.
* Setteja el SMCLK i el MCLK a 16MHZ i el ACLK a 2^5 Hz i configura el pins 11.0,11.1 i 11.2 com a sortida de test.
* 
**/
void init_UCS(void) {
	//inicialització de les freqüències de rellotge del microcontrolador
	unsigned long DCORSEL = DCORSEL_7; //DCORSEL_7 selecciona rango de DCO de 8.5 a 19.6 MHz
	unsigned short FLLN = 487; //Parametro N
	__bis_SR_register(SCG0); // Desactiva el FLL control loop
	UCSCTL0 = 0x00; // Posa DCOx, MODx al mínim possible
	UCSCTL1 = DCORSEL; // Seleccionem el rang de DCO 16 MHz
	UCSCTL2 = FLLN + FLLD_1; //Selecciona el factor multiplicador del DCO
	UCSCTL3 = 0; //Referència FLL SELREF = XT1, divisor =1;
	/* Selecció de la font de rellotge: ACLK el Clock extern de 215, SMCLK i MCLK el DCO intern de 16MHz */
	UCSCTL4 = SELA__XT1CLK | SELS__DCOCLKDIV | SELM__DCOCLKDIV;
	UCSCTL5 = DIVA__1 | DIVS__1; //Divisor per SMCLK: f(SMCLK)/1 i ACLK: f(ACLK)/1
	__bic_SR_register(SCG0); // Enable the FLL control loop
	P11DIR = 0x07; //Configurem els pins 11.0, 11.1 y 11.2 com sortida
	P11SEL = 0x07; //ACLK, MCLK y SMCLK; activem els pins de test
}

/**
* Funció de configuració de l'USCI en mode UART.
**/
void init_UART(void) {

	UCA0CTL1 |= UCSWRST; //Fem un reset de la USCI i es desactiva
	UCA0CTL0 = 0x00; //UCSYNC=0 mode asíncron
	//UCMODEx=0 seleccionem mode UART
	//UCSPB=0 nomes 1 stop bit
	//UC7BIT=0 8 bits de dades
	//UCMSB=0 bit de menys pes primer
	//UCPAR=x no es fa servir bit de paritat
	//UCPEN=0 sense bit de paritat
	UCA0CTL1 |= UCSSEL__SMCLK; //Triem SMCLK (16MHz) com a font del clock BRCLK
	UCA0BR0 = 1; //Prescaler de BRCLK fixat a 1 (LO byte = 1, ...
	UCA0BR1 = 0; //... HI byte = 0).
	UCA0MCTL = UCOS16; // No necessitem modulació (divisió entera), però
// si oversampling => bit 0 = UCOS16 = 1.
			// El Baud rate és BRCLK/16=1MBps
	P3SEL |= 0x30; //I/O funcion: P3.4 = UART0TX transmitir, P3.5 = UART0RX recibir
	P3REN |= 0x20; //amb resistència activada de pull-up l’entrada P3.5
	P3OUT |= 0x20; //
	P3DIR |= 0x80; //Port P3.7 com sortida (Data Direction: selector Tx/Rx)
	P3SEL &= ~0x80; //Port 3.7 com I/O digital
	P3OUT &= ~0x80; //Inicialitzem el port P3.7 a 0 (Rx) per defecte esta per rebre dades
	UCA0CTL1 &= ~UCSWRST; //Reactivem la línia de comunicacions sèrie
	UCA0IE |= UCRXIE; //Això s’ha d’activar només quan fem la recepció

}

/**
 * Funció d'inicialització del timer A1 per tal de que provoqui una interrupció cada 10 microsegons
**/
void init_timer_A1(void) {
	TA1CTL &= 0x0000; //Inicialitzem tots els bits a 0
	TA1CTL |= MC_1; //Mode de treballar up MC 01
	TA1CTL |= TBSSEL__SMCLK; //Seleccionem la font de rellotge SMCLK
	//TB0CTL |= TBIE; //Interrupt enabled modificant TBIE	Avui no cal en principi

	TA1CCTL0 &= 0x0000; //Inicialitzem tots els bits a 0
	//Si no posem res, per defecte hem deixat el compare mode.

	TA1CCR0 = 1600; //Cada 10 microsegons
}

/**
 * Funció d'inicialització del timer B0 per tal de que provoqui una interrupció cada 100 microsegons
**/
void init_timer_B0(void) {
	TB0CTL &= 0x0000; //Inicialitzem tots els bits a 0
	TB0CTL |= MC_1; //Mode de treballar up MC 01
	TB0CTL |= TBSSEL__SMCLK; //Seleccionem la font de rellotge SMCLK
	//TB0CTL |= TBIE; //Interrupt enabled modificant TBIE	Avui no cal en principi

	TB0CCTL0 &= 0x0000; //Inicialitzem tots els bits a 0
	//Si no posem res, per defecte hem deixat el compare mode.

	TB0CCR0 = 16000; //Cada 100 microsegons
}

void enableInterruptTimerA1(void) {
	TA1CCTL0 |= CCIE; //Interrupt enabled modificant CCIE
}

void enableInterruptTimerB0(void) {
	TB0CCTL0 |= CCIE; //Interrupt enabled modificant CCIE
}



