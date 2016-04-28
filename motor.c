/*
 * motor.c
 *
 *  Created on: 28/04/2016
 *      Author: mat.aules
 */
#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "comunication.h"
#include "definedValues.h"
#include "sensor.h"

int timeMove = 0;

void moveWheel(byte ID, byte low, byte high){
	RxReturn rx;
	byte bID = ID; //Motor ID
	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 3;
	byte gbpParameter[20];
	gbpParameter[0] = P_GOAL_SPEED_L; //moving speed
	gbpParameter[1] = low;
	gbpParameter[2] = high;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	rx = RxPacket();
	if (rx.error & 0x01)
		_nop();

}


void endLessTurn(void){
	byte bID = BROADCASTING_ID; // Amb aquesta id els packets enviats s'aplicaran a totes les unitats de Dynamixel. I no es retornarà cap Status Packet.
	byte bInstruction = INST_WRITE; //instruction write
	byte bParameterLength = 5; // Número de paràmetres
	byte gbpParameter[20];
	//The wheel mode can be used to wheel-type operation robots since motors of the robots spin infinitely.
	gbpParameter[0] = P_CW_ANGLE_LIMIT_L;
	gbpParameter[1] = 0;
	gbpParameter[2] = 0;
	gbpParameter[3] = 0;
	gbpParameter[4] = 0;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
	//RxPacket();


}

void walk(void){
	byte bID = BROADCASTING_ID; // a més d'un actuador
	byte bInstruction = INST_SYNC_WRITE; //instruction write sincrona, una unica instrucció per a més d'un motor
	byte bParameterLength = 16; // (L + 1) * N + 4 (L: Data length for each Dynamixel actuator, N: The number of Dynamixel actuators)
	byte gbpParameter[20];
	gbpParameter[0] = P_GOAL_SPEED_L ;
	gbpParameter[1] = 2; //The length of the data to be written (L)
	gbpParameter[2] = 1; //The ID of the 1st Dynamixel actuator
	gbpParameter[3] = 1;
	gbpParameter[4] = 3;
	gbpParameter[5] = 1;
	gbpParameter[6] = 1;
	gbpParameter[7] = 1;
	gbpParameter[8] = 1;
	gbpParameter[9] = 1;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);
}

void quiet(void){
	byte bID = BROADCASTING_ID; // a més d'un actuador
	byte bInstruction = INST_SYNC_WRITE; //instruction write sincrona, una unica instrucció per a més d'un motor
	byte bParameterLength = 16; // (L + 1) * N + 4 (L: Data length for each Dynamixel actuator, N: The number of Dynamixel actuators)
	byte gbpParameter[20];
	gbpParameter[0] = P_GOAL_SPEED_L;
	gbpParameter[1] = 2; //The length of the data to be written (L)
	gbpParameter[2] = 1; //The ID of the 1st Dynamixel actuator
	gbpParameter[3] = 1;
	gbpParameter[4] = 1;
	gbpParameter[5] = 1;
	gbpParameter[6] = 1;
	gbpParameter[7] = 1;
	gbpParameter[8] = 1;
	gbpParameter[9] = 1;
	TxPacket(bID, bParameterLength, gbpParameter, bInstruction);

}

void moveBackward(int time){
	moveWheel(1, 0xFF, 0x00);
	moveWheel(2, 0xFF, 0x00);
	moveWheel(3, 0xFF, 0x04);
	moveWheel(4, 0xFF, 0x04);
	if (time != 0){
		timeMove = 0;
		while (timeMove <= time);
	}
}

void moveForward(int time){
	timeMove = 0;
	moveWheel(1, 0xFF, 0x04);
	moveWheel(2, 0xFF, 0x04);
	moveWheel(3, 0xFF, 0x00);
	moveWheel(4, 0xFF, 0x00);
	if (time != 0){
		timeMove = 0;
		while (timeMove <= time);
	}

}

void turnRight(int time){
	moveWheel(1, 0xFF, 0x00);
	moveWheel(2, 0xFF, 0x00);
	moveWheel(3, 0xFF, 0x00);
	moveWheel(4, 0xFF, 0x00);
	if (time != 0){
		timeMove = 0;
		while (timeMove <= time);
	}
}

void turnLeft(int time){
	moveWheel(1, 0xFF, 0x04);
	moveWheel(2, 0xFF, 0x04);
	moveWheel(3, 0xFF, 0x04);
	moveWheel(4, 0xFF, 0x04);
	if (time != 0){
		timeMove = 0;
		while (timeMove <= time);
	}
}

void stopMoving(){
	moveWheel(1, 0x00, 0x00);
	moveWheel(2, 0x00, 0x00);
	moveWheel(3, 0x00, 0x00);
	moveWheel(4, 0x00, 0x00);
}

void moveObstacle(){
	int f;
	f = obstacleDetected();
	switch(f){
	case (LEFT):
		turnRight(0);
		break;
	case (FRONT):
		moveBackward(0);
		break;
	case (RIGHT):
		turnLeft(0);
		break;
	default:
		moveForward(0);
	}

}


