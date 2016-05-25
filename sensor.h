/*
 * sensor.h
 * Fitxer capcalera de les funcions que demanen per les dades del sensor.
 *  Created on: 28/04/2016
 *      Author: Aina Ferra i Alicia Morales.
 */

#ifndef SENSOR_H_
#define SENSOR_H_


int obstacleDetected(void);
void obstacleDistance(byte bID);
int readLeftSensor(byte bID);
int readRightSensor(byte bID);
int closeRight();
int closeLeft();
void setMaxDistance(void);
void setMinDistance(void);
int isFire(void);
void playBuzzMelody(void);
void playNyanMelody(int position);
char clapCount(void);

#endif /* SENSOR_H_ */
