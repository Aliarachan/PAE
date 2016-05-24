/*
 * definedValues.h
 *	Fitxer de definici� de constants comunes a la majoria de fitxers.
 *  Created on: 28/04/2016
 *      Author: Aina Ferr� i Alicia Morales
 */

#ifndef DEFINEDVALUES_H_
#define DEFINEDVALUES_H_

#define ON 1
#define OFF 0
#define BROADCASTING_ID 0xFE


//--- Control Table Address ---
//EEPROM AREA
#define P_CW_ANGLE_LIMIT_L 6
#define P_CW_ANGLE_LIMIT_H 7
#define P_CCW_ANGLE_LIMIT_L 8
#define P_CCW_ANGLE_LIMIT_H 9

//RAM AREA
#define P_LED (25)
#define P_GOAL_SPEED_L (32)
#define P_GOAL_SPEED_H (33)

//--- Sensors ---
#define LEFT_IR_SENSOR 26
#define CENTER_IR_SENSOR 27
#define RIGHT_IR_SENSOR 28

//--- Instruction ---
#define INST_PING 0x01
#define INST_READ 0x02
#define INST_WRITE 0x03
#define INST_REG_WRITE 0x04
#define INST_ACTION 0x05
#define INST_RESET 0x06
#define INST_SYNC_WRITE 0x83
#define LEFT 1
#define FRONT 2
#define RIGHT 3
#define NO_OBSTACLE -1
#define LEFT_FRONT 4
#define LEFT_RIGHT 5
#define FRONT_RIGHT 6
#define LEFT_FRONT_RIGHT 7
#define ERROR -2


//---- Wall values
#define RIGHT_WALL 1
#define LEFT_WALL 2


//--- Botons i joystick ---
#define BUTTONS1 0x40
#define BUTTONS2 0x80
#define JOYSTICK_CENTER 0x08
#define JOYSTICK_LEFT 0x02
#define JOYSTICK_RIGHT 0x04
#define JOYSTICK_UP 0x10
#define JOYSTICK_DOWN 0x20



#define TXD0_READY (UCA0IFG & UCTXIFG)
#define RXD0_READY (UCA0IFG & UCRXIFG)

#define TOOCLOSE 200
#define TOOCLOSE2 170


typedef unsigned char byte;
typedef struct RxReturn {
	byte StatusPacket[20];
	byte error;
	byte receivedError;
} RxReturn;

#endif /* DEFINEDVALUES_H_ */
