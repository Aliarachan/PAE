/*
 * motor.h
 *
 *  Created on: 28/04/2016
 *      Author: mat.aules
 */

#ifndef MOTOR_H_
#define MOTOR_H_

extern int timeMove;


/*-------------------------------------------------------------------------
 *                  Funcions del robot
 * ------------------------------------------------------------------------*/

void endLessTurn(void);
void moveWheel(byte ID, byte low, byte high);
void walk(void);
void quiet(void);
void moveBackward(int time);
void moveForward(int time);
void turnRight(int time);
void turnLeft(int time);
void stopMoving();
void moveObstacle();

#endif /* MOTOR_H_ */
