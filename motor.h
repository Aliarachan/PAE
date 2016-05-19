/*
 * motor.h
 * Fitxer cap�alera de definici� de les funcions que utilitzen l'enviament i recepci� de paquets dels 4 moduls motors. ( Dynamixel AX-12).
 *  Created on: 28/04/2016
 *      Author: Aina Ferr� i Alicia Morales
 */

#ifndef MOTOR_H_
#define MOTOR_H_

extern int timeMove; // Variable comptador del temps que s'ha mogut en una determinada direcci�, s'incrementa a la interrupci� del timer B0 i s'usa a les funcions dels motors.


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
void turnHRight(int time);
void turnHLeft(int time);
void stopMoving();
void moveObstacle();
void followWall();
void followTheLeftWall();

#endif /* MOTOR_H_ */
