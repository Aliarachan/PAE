/*
 * sensor.h
 * Fitxer capcalera de les funcions que demanen per les dades del sensor.
 *  Created on: 28/04/2016
 *      Author: Aina Ferrà i Alicia Morales.
 */

#ifndef SENSOR_H_
#define SENSOR_H_


int obstacleDetected(void);
void obstacleDistance(byte bID);
int readLeftSensor(byte bID);
int readRightSensor(byte bID);
int closeRight();
int closeLeft();
void escribir(char String[], unsigned char Linea);
void borrar(unsigned char Linea);



#endif /* SENSOR_H_ */
