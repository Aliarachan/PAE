/*
 * comunication.h
 *	Fitxer capcalera de definició de les funcions de comunicació del robot.
 *  Created on: 28/04/2016
 *      Author: Aina Ferrà i Alicia Morales
 */

#ifndef COMUNICATION_H_
#define COMUNICATION_H_

typedef unsigned char byte;

//Variables comunes 
extern byte readDataUART;
extern byte receivedByte;
extern int timeNow;

void sentit_Dades_Rx(void);
void sentit_Dades_Tx(void);
void TxUAC0(byte bTxdData);
byte TxPacket(byte bID, byte bParameterLength, byte *gbpParameter, byte bInstruction);
struct RxReturn RxPacket(void);



#endif /* COMUNICATION_H_ */
