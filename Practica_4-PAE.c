/*
* ---------------------------------PROJECTE FINAL PAE
* Programacio d'un robot que busca i segueix una pared.
* Extres: es comenca a moure quan sent dues palmades, s'atura amb una llum molt intensa,
* encen els leds segons la direccio en la que es mou, fa sonar una melodia.
*
*/

#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "init.h"
#include "comunication.h"
#include "sensor.h"
#include "motor.h"
#include "definedValues.h"
#include "pantalla.h"
#include "extres.h"



int currIndexMelody = 0;
//Aquestes variables ens serveixen de control per assegurar-nos de
//que hem posat la distancia minima i la distancia maxima.
char setMin = 0;
char setMax = 0;
char doneMin = 0;
char doneMax = 0;

//Aquesta variable ens serveix per saber quan hem trobat una referencia.
char wall = 0;
//Variable que ens serveix per a que el robot es comenci a moure o s'aturi.
char move = 0;

//String per poder escriure.
char string[17];


/*
* Funcio que fa que el robot segueixi una pared que te a la dreta.
* Sense dades d'entrada ni dades de sortida.
*/
void followTheRightWall(){
	volatile int f, c;
	c = 0;
	//Legim de la dtecció d'obstacles.
	f = obstacleDetected();

	//Si hi ha hagut un error no fem res, es una lectura erronia.
	if(f == ERROR){
			//Posem no_operation per facil.litar el debug.
		__no_operation();

		//Si trobem alguna cosa AL FRONT
	} else if ((f & 2) == 2){
		//Ho anunciem per pantalla
		escribir("FRONT            ", 1);
		//Girem a l'esquerra.
		turnHLeft(0);
		//Si trobem alguna cosa a la DRETA
	} else if ((f & 4) == 4){
		//Ho anunciem per pantalla.
		escribir("RIGHT              ", 1);
		//Mirem si estem massa aprop de la nostra referencia
		c = closeRight();
		if (c && c!= ERROR){
			//Si ho estem, ho anunciem per pantalla.
			escribir("TOO CLOSE          ", 1);
			//Girem una mica i continuem una mica endavant.
			turnLeft(200);
			moveForward(500);
		}
		//Si no estem massa aprop, simplement continuem endavant.
		moveForward(0);

		//Si no hem trobat res ni a la dreta ni a l'esquerra.
		//No tenim cap obstacle al front i no tenim la nostra referencia
		//a la dreta. Hem perdut la referencia. Hem de girar cap a la
		//dreta per poder recuperar la referencia.
	} else {
		//Ho enunciem per pantalla.
		escribir("NO REFERENCE             ", 1);
		turnRight(300);
		moveForward(500);
	}
}

/*
* Funcio que fa que el robot segueixi una pared que te a l'esquerra.
* Sense dades d'entrada ni de sortida.
* L'algorisme es l'analeg al de la pared per la dreta.
*/
void followTheLeftWall(){
	volatile int f, c;
	c = 0;
	//Llegim si hi ha un obstacle.
	f = obstacleDetected();

	//Si hem tingut un error no podem fer res.
	if(f == ERROR){
		__no_operation();

		//Si trobem un obstacle al FRONT.
	} else if ((f & 2) == 2){
		escribir("FRONT            ", 1);
		turnHRight(0);

		//Si tenim un obstacle a l'ESQUERRA.
	} else if ((f & 1) == 1){
		escribir("LEFT            ", 1);
		c = closeLeft();

		//Mirem que no estem massa aprop de la nostra referencia.
		if (c && c!= ERROR){
			borrar(1);
			escribir("LEFT CLOSE             ", 1);
			turnRight(300);
			moveForward(500);
		}
		moveForward(0);

		//Si no tenim res al front ni a la dreta, llavors hem perdut
		//la referencia, hem de girar a l'esquerra.
	} else {
		escribir("NO REFERENCE         ", 1);
		turnLeft(300);
		moveForward(500);
	}
}


/*
* Funcio que fa que el robot busqui una pared.
* El movem cap endavant fins que trobi un obstacle i ens posicionem
* com ens sembla mes adient.
*
* Sense dades d'entrada ni dades de sortida.
*/
void searchWall(){
	int f;

	//Si no hem trobat encara una referencia.
	while (!wall){
		//Llegim el detector d'obstacles.
		f = obstacleDetected();

		//Si ens dona un error, es una lectura erronia.
		//No hem de fer res.
		if (f == ERROR){
			__no_operation();

			//Si trobem l'obstacle al FRONT.
			//Podem decidir on posar-nos i quina pared seguir.
			//Girem a l'esquerra i per tant tenim la referencia dreta.
		} else if ((f & 2) == 2){
			turnHLeft(500);
			wall = RIGHT_WALL;

			//Si trobem l'obstacle a la DRETA
		} else if ((f& 4) == 4){
			moveForward(500);
			wall = RIGHT_WALL;

			//Si trobem l'obstacle a l'ESQUERRA.
		} else if ((f&1) == 1){
			moveForward(500);
			wall = LEFT_WALL;
		}

		//Si no trobem res, simplement continuem endavant.
		moveForward(0);
	}
}

void main(void) {
	WDTCTL = WDTPW + WDTHOLD;       	// Paramos el watchdog timer

	/*Inicialitzem tot el necessari*/
	/*
	* Necessitem UCS i UART per les comunicacions.
	* Timer A1 i timer B0 per les interrupcions de timer.
	* La pantalla per poder ensenyar missatges.
	* Els botons que usem per cal.librar les distancies.
	* Els LEDS que tambe volem encendre.
	*/
	init_UCS();
	init_UART();
	init_timer_A1();
	init_timer_B0();
	init_LCD();
	init_botons();
	config_P4_LEDS();

	/*
	* Habilitem les interrupcions un cop hem fet totes les inicialitzacions.
	* Aixi evitem que es configuri alguna cosa malament per culpa d'una
	* interrupcio.
	*/
	enableInterruptTimerB0();
 	_enable_interrupt();


 	__delay_cycles(1000000); // Donem un poc de temps d'espera a que tot s'hagi inicialitzat correctament
	endLessTurn(); // Settejem les rodes en mode continu
	__delay_cycles(1000000);
	escribir("hola",2);
	while(1){

	playNyanMelody(currIndexMelody++);
		if(currIndexMelody > 454)
		{
			currIndexMelody = 0;
		}
	}


	/*
	* Primer bucle d'espera. El nostre robot no s'iniciara si
	* abans no hem cal.librat la distancia maxima i minima.
	* Ens quedarem esperant aqui fins que ho haguem fet.
	*/
	while(!(setMin && setMax)){
		int c;
		//Ensenyem la distancia actual per pantalla
		//D'aquesta manera ens assegurem que podem cal.librar la distancia que volem.
		c = readRightSensor(100);
		sprintf(string, "Distance %.3d", c);
		escribir(string, 1);


		if (setMin && !doneMin){
			setMinDistance();
			doneMin = 1;
			//Quan haguem cal.librat ho anunciem per pantalla.
			escribir("Got min distance", 2);
		}
		if (setMax && !doneMax){
			setMaxDistance();
			doneMax = 1;
			//Quan haguem cal.librat, ho anunciem per pantalla.
			escribir ("Got max distance", 3);
		}
	}


	/*
	* Segon bucle d'espera. Un cop hem cal.librat les distancies, el nostre
	* robot espera a que nosaltres li indiquem que pot comencar a caminar.
	* Es pot fer o be amb el joystick o be fent palmades.
	*/
	while(!move){
		char n;
		//Llegim el comptador de palmades
		n = clapCount();
		//Si n'hem fet dues, el robot pot coninuar!
		if (n == 2){
			move = 1;
		}
	}

	//Escrivim per pantalla que estem buscant pared!
	borrar(3);
	escribir("Searching...         ", 2);
	searchWall();


	/*
	* Tercer bucle d'espera. El nostre robot esta en marxa. Sortira de la funcio
	* de cerca d'una pared amb una referencia. Llavors, ha de seguir aquesta referencia fins que nosaltres li diem
	* que pari (ho indiquem amb una llum molt intensa).
	*/
	if (wall == RIGHT_WALL){
		int fire;
		//Anunciem quina referencia seguim
		escribir("Right wall             ", 2);
		while(move){
			followTheRightWall();
			//Mirem si hi ha foc.
			fire = isFire();
			//si hi ha foc parem!!!
			if (fire > 0){
				move = 0;
			}
		}
	} else if (wall == LEFT_WALL){
		escribir("Left wall           ", 2);
		while(move){
			followTheLeftWall();
			//Mirem si hi ha foc.
			fire = isFire();
			//si hi ha foc parem!!!
			if (fire > 0){
				move = 0;
			}
		}
	}

	//Parem el robot!
	stopMoving();
}


/*-------------------------------------------------------------------------
 *                  Rutines d'atencio a la interrupcio (ISR)
 * ------------------------------------------------------------------------*/

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) { //interrupcion de recepcion en la uart A0
	UCA0IE &= ~UCRXIE; //Interrupciones desactivadas en RX
	readDataUART = UCA0RXBUF; //Agafem del bus la data que hem de llegir
	receivedByte = 1; //Marquem al flag que ja hem rebut el byte
	UCA0IE |= UCRXIE; //Interrupciones reactivadas en RX
}

/*
* El timer A1 ens controla el timeOut de les comunicacions.
*/
#pragma vector=TIMER1_A0_VECTOR
__interrupt void timerA1_ISR(void) {
	timeNow += 1;
}

/*
* El timer B0 ens serveix per fer funcions com la de delay.
*/
#pragma vector=TIMERB0_VECTOR
__interrupt void timerB0_ISR(void) {
	timeMove += 1;
}

/*
* Usem els botons per poder cal.librar la distancia maxima i minima.
*/
#pragma vector=PORT2_VECTOR
__interrupt void Port2_ISR(void)
{
	P2IE &= 0xC0; 	//interrupciones botones S1 y S2 (P2.6 y P2.7) desactivadas
	P2IE &= 0x3E;   //interrupciones joystick (2.1-2.5) desactivadas
	switch(P2IFG){
		case (BUTTONS1):
			setMax = 1;
		break;
		case (BUTTONS2):
			setMin = 1;
		break;
		case (JOYSTICK_CENTER):

		break;
		case (JOYSTICK_LEFT):
		break;
		case (JOYSTICK_RIGHT):
		break;
		case (JOYSTICK_UP)://Joystick up
		if (move){
			move = 0;
		} else {
			move = 1;
		}
		break;
		case (JOYSTICK_DOWN):
		break;
		default:
		break;
	}

	P2IFG = 0;		//limpiamos todas las interrupciones
	P2IE |= 0xC0; 	//interrupciones botones S1 y S2 (P2.6 y P2.7) reactivadas
	P2IE |= 0x3E;  //interrupciones joystick (2.1-2.5) reactivadas
 return;
}
