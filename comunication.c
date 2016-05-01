/*
 * comunication.c
 * Fitxer amb les funcions de comunicaci� del robot per tal d'enviar i rebre paquets, indicar el sentit de les dades.
 *  Created on: 28/04/2016
 *      Author: mat.aules
 */

#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "definedValues.h"

int timeNow = 0;
byte receivedByte;
byte readDataUART;

/**
* Funci� que comprova si s'ha superat o no el temps d'espera per a una operaci�.
* Rep com a entrada el temps maxim per a l'operaci� i retorna si s'ha superat el temps des de l'inici de l'operaci�.
**/
byte timeOut(byte temps) {
	return (timeNow >= temps);
}

/**
 * Funci� que fixa el sentit de les dades per a la recepci�.
**/
void sentit_Dades_Rx(void) {
	//Configuraci� del Half Duplex dels motors: Recepci�
	P3OUT &= ~0x80; //El pin P3.7 (DIRECTION_PORT) el posem a 0 (Rx)
}

/**
 * Funci� que fixa el sentit de les dades per a l'enviament.
**/
void sentit_Dades_Tx(void) {
	//Configuraci� del Half Duplex dels motors: Transmissi�
	P3OUT |= 0x80; //El pin P3.7 (DIRECTION_PORT) el posem a 1 (Tx)
}

/* Envia un byte de dades per la UART 0 */
void TxUAC0(byte bTxdData) {
	while (!TXD0_READY); // Espera a que estigui preparat el buffer de transmissi�
	UCA0TXBUF = bTxdData; // Introducci� del byte al buffer
}

/* Funci� d'enviament de dades byte a byte, rep com a parametres l'ID del Dynamixel, Mida dels par�metres, Parametres i Instruction byte. 
 * Genera la trama i l'envia.
 * Retorna la mida del "Return packet" des del Dynamixel.
 */
byte TxPacket(byte bID, byte bParameterLength, byte *gbpParameter, byte bInstruction) {
	byte bCount, bCheckSum, bPacketLength; 
	byte gbpTxBuffer[32]; // Vector de mida fixa que emmagatzemar� la trama a enviar.
	volatile int i = 50;
	char error[20] = "adr. no permitida";

	if ((gbpParameter[0] < 5) && (bInstruction == 3)) { //si se intenta escribir en una direccion <= 0x05,
		//emitir mensaje de error de direccion prohibida:
		halLcdPrintLine(error, 8, OVERWRITE_TEXT);
		//y salir de la funcion sin mas:
		return 0;
	}

	sentit_Dades_Tx(); //El pin P3.7 (DIRECTION_PORT) el posem a 1 (Tx)

	gbpTxBuffer[0] = 0xff; //Primers 2 bytes que indiquen inici de trama FF, FF.
	gbpTxBuffer[1] = 0xff;
	gbpTxBuffer[2] = bID; //ID del m�dul al que volem enviar el missatge
	gbpTxBuffer[3] = bParameterLength + 2; //Length(Parameters,Instruction,Checksum)
	gbpTxBuffer[4] = bInstruction; //Instrucci� que enviem al m�dul

	for (bCount = 0; bCount < bParameterLength; bCount++) //Comencem a generar la trama
			{
		gbpTxBuffer[bCount + 5] = gbpParameter[bCount]; //* Pot haver m�s d'un par�metre
	}

	bCheckSum = 0; //* Si ha hagut algun error.
	bPacketLength = bParameterLength + 4 + 2; // Longitud del paquet = ID+ longitud de parametres + inici trama+ checksum 

	for (bCount = 2; bCount < bPacketLength - 1; bCount++) //C�lcul del Checksum
			{
		bCheckSum += gbpTxBuffer[bCount];
	}

	gbpTxBuffer[bCount] = ~bCheckSum; //Escriu el Checksum (complement a 1)

	for (bCount = 0; bCount < bPacketLength; bCount++) //Aquest bucle �s el que envia la trama
			{
		TxUAC0(gbpTxBuffer[bCount]);
	}

	while ((UCA0STAT & UCBUSY)); //Espera fins s�ha transm�s el �ltim byte

	sentit_Dades_Rx(); //Posem la l�nia de dades en Rx perqu� el m�dul Dynamixel envia resposta

	return (bPacketLength);
}

/**
 * Funci� de recepci� de paquets byte a byte. Assegura que no s'empra massa temps esperant la recepci�.
 * Retorna l'estructura amb les dades rebudes.
**/
struct RxReturn RxPacket(void) {
	struct RxReturn answer;
	byte bCount, bLength, bChecksum;
	byte Rx_time_out = 0; // Boolea que indica si ha ocurregut un timeOut
	answer.error = 0;  //de moment no hem rebut cap error

	sentit_Dades_Rx(); //Ponemos la linea half duplex en Rx
	enableInterruptTimerA1(); // habilitem el timer que usa el timeOut

	/**
	 * Llegim els quatre primers parametres que sabem exactament el que som.
	 * Per cada parametre, reiniciarem el timer i prepararem la funcio timeOut.
	 * Si en algun dels parametres d�na un timeOut, sortirem de la funcio.
	 */
	for (bCount = 0; bCount < 4; bCount++) { //4 primers parametres

		timeNow = 0; // reset del timeOut
		receivedByte = 0; //No hem rebut cap byte encara
		/**
		 * Mentre no haguem rebut cap byte ens esperem
		 * Nomes esperem un temps determinat (2 milisegons)
		 * Si se'ns produeix un timeOut hem de sortir de la funcio i mostrar error
		 */
		while (!receivedByte) {
			Rx_time_out = timeOut(20);
			if (Rx_time_out) {
				break; //sale del while
			}
		}
		//Si hem tingut un timeOut sortim directament de la funcio sense fer res.
		if (Rx_time_out) {
			answer.error = 1;
			break;
		}
		//Si no, es que todo ha ido bien, y leemos un dato:
		answer.StatusPacket[bCount] = readDataUART;
	}

	if (!Rx_time_out) {
		bLength = answer.StatusPacket[3] + 4; // longitud total a llegir
		for (bCount = 4; bCount < bLength; bCount++ ) {
			timeNow = 0;
			receivedByte = 0; //No hem rebut cap byte encara

			while (!receivedByte) {
				Rx_time_out = timeOut(20);
				if (Rx_time_out) {
					break;
				}
			}
			//Si hem tingut un timeOut sortim directament de la funcio sense fer res.
			if (Rx_time_out) {
				answer.error = 1;
				break;
			}
			//Si no, es que todo ha ido bien, y leemos un dato:
			answer.StatusPacket[bCount] = readDataUART;
		}
	}

	// Si no s'ha donat error de timeOut passem a calcula el Checksum
	if (!Rx_time_out) {
		bChecksum = 0;
		for (bCount = 2; bCount < bLength - 1; bCount++) {
			bChecksum += answer.StatusPacket[bCount];
		}

		bChecksum = ~bChecksum; // Una alternativa seria sumar ambd�s valors (el calculat i el rebut i comprovar que el resultat �s 0)

		// S�n els checksum rebut i calculat iguals?
		if (bChecksum != answer.StatusPacket[bLength- 1]) {
			answer.error = 2; // Error de checksum
		}
	}

	return (answer);

}


