/*
 * comunication.c
 *
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

byte timeOut(byte temps) {
	return (timeNow >= temps);
}


void sentit_Dades_Rx(void) {
	//Configuració del Half Duplex dels motors: Recepció
	P3OUT &= ~0x80; //El pin P3.7 (DIRECTION_PORT) el posem a 0 (Rx)
}

void sentit_Dades_Tx(void) {
	//Configuració del Half Duplex dels motors: Transmissió
	P3OUT |= 0x80; //El pin P3.7 (DIRECTION_PORT) el posem a 1 (Tx)
}

/* Envia un byte de dades per la UART 0 */
void TxUAC0(byte bTxdData) {
	while (!TXD0_READY)
		; // Espera a que estigui preparat el buffer de transmissió
	UCA0TXBUF = bTxdData;
}

/* ID del Dynamixel, Mida dels paràmetres, Instruction byte.
 Retorna la mida del "Return packet" des del Dynamixel.
 */
byte TxPacket(byte bID, byte bParameterLength, byte *gbpParameter, byte bInstruction) {
	byte bCount, bCheckSum, bPacketLength;
	byte gbpTxBuffer[32];
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
	gbpTxBuffer[2] = bID; //ID del mòdul al que volem enviar el missatge
	gbpTxBuffer[3] = bParameterLength + 2; //Length(Parameter,Instruction,Checksum)
	gbpTxBuffer[4] = bInstruction; //Instrucció que enviem al mòdul

	for (bCount = 0; bCount < bParameterLength; bCount++) //Comencem a generar la trama
			{
		gbpTxBuffer[bCount + 5] = gbpParameter[bCount]; //* Pot haver més d'un paràmetre
	}

	bCheckSum = 0; //* Si ha hagut algun error.
	bPacketLength = bParameterLength + 4 + 2;

	for (bCount = 2; bCount < bPacketLength - 1; bCount++) //Càlcul del Checksum
			{
		bCheckSum += gbpTxBuffer[bCount];
	}

	gbpTxBuffer[bCount] = ~bCheckSum; //Escriu el Checksum (complement a 1)

	for (bCount = 0; bCount < bPacketLength; bCount++) //Aquest bucle és el que envia la trama
			{
		TxUAC0(gbpTxBuffer[bCount]);
	}

	while ((UCA0STAT & UCBUSY))
		; //Espera fins s’ha transmès el últim byte

	sentit_Dades_Rx(); //Posem la línia de dades en Rx perquè el mòdul Dynamixel envia resposta

	return (bPacketLength);
}

struct RxReturn RxPacket(void) {
	struct RxReturn answer;
	byte bCount, bLength, bChecksum;
	byte Rx_time_out = 0; // Boolea que indica si ha ocurregut un timeOut
	answer.error = 0;  //de moment no hem rebut cap error

	sentit_Dades_Rx(); //Ponemos la linea half duplex en Rx
	enableInterruptTimerA1();

	/**
	 * Llegim els quatre primers parametres que sabem exactament el que som.
	 * Per cada parametre, reiniciarem el timer i prepararem la funcio timeOut.
	 * Si en algun dels parametres dóna un timeOut, sortirem de la funcio.
	 */
	for (bCount = 0; bCount < 4; bCount++) { //bRxPacketLength; bCount++)

		timeNow = 0;
		receivedByte = 0; //No hem rebut cap byte encara
		/**
		 * Mentre no haguem rebut cap byte ens esperem
		 * Nomes esperem un temps determinat (600 microsegons)
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
		bLength = answer.StatusPacket[3] + 4;
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


	if (!Rx_time_out) {
		bChecksum = 0;
		for (bCount = 2; bCount < bLength - 1; bCount++) {
			bChecksum += answer.StatusPacket[bCount];
		}

		bChecksum = ~bChecksum;
		if (bChecksum != answer.StatusPacket[bLength- 1]) {
			answer.error = 2;
		}
	}

	return (answer);

}


