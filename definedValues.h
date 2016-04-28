/*
 * definedValues.h
 *
 *  Created on: 28/04/2016
 *      Author: mat.aules
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

//--- Instruction ---
#define INST_PING 0x01
#define INST_READ 0x02
#define INST_WRITE 0x03
#define INST_REG_WRITE 0x04
#define INST_ACTION 0x05
#define INST_RESET 0x06
#define INST_SYNC_WRITE 0x83
#define LEFT 0
#define FRONT 1
#define RIGHT 2


#define TXD0_READY (UCA0IFG & UCTXIFG)
#define RXD0_READY (UCA0IFG & UCRXIFG)


typedef unsigned char byte;
typedef struct RxReturn {
	byte StatusPacket[20];
	byte error;
} RxReturn;

#endif /* DEFINEDVALUES_H_ */
