/*
 * dynamixel.h
 *
 *  Created on: 19/4/2016
 *      Author: Aina Ferra i Alicia Morales
 */

#ifndef DYNAMIXEL_H_
#define DYNAMIXEL_H_





typedef unsigned char byte;
typedef unsigned int word;
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

/*-------------------------------------------------------------------------
 *                  Funcions de recepció/transferència de packets
 * ------------------------------------------------------------------------*/
void TxUAC0(byte bTxdData);
byte TxPacket(byte bID, byte bParameterLength, byte *gbpParameter, byte bInstruction);
struct RxReturn RxPacket(void);

byte timeOut(byte temps);


/*-------------------------------------------------------------------------
 *                  Funcions del robot
 * ------------------------------------------------------------------------*/
void resetLimitAngles(void);
void moveWheel(void);
void endLessTurn(void);


#endif /* DYNAMIXEL_H_ */
