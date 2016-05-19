#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>

/*------------------------------------------------------------
        CONFIGURACIONS DELS BOTONS I ELS LEDS
-------------------------------------------------------------*/
void init_botons(void){
  //Configuramos botones
  P2DIR &= ~0xC0;	//Puertos P2.6 y P2.7 como entradas (botones S1 y S2)
  P2SEL &= ~0xC0;	//Puertos P2.6 y P2.7 como I/O digitales,
  P2REN |= 0xC0;	//con resistencia activada
  P2OUT |= 0xC0;	// de pull-up
  P2IE |= 0xC0; 	//Interrupciones activadas en P2.6 y P2.7,
  P2IES &= ~0xC0;	// con transicion L->H


  //Configuramos el joystick:
  P2DIR &= ~0x3E;	//Puertos P2.1 a P2.5 como entradas (joystick)
  P2SEL &= ~0x3E;	//Puertos P2.1 y P2.5 como I/O digitales,
  P2REN |= 0x3E;	//con resistencia activada
  P2OUT |= 0x3E;	// de pull-up
  P2IE |= 0x3E; 	//Interrupciones activadas en P2.1 a P2.5,
  P2IES &= ~0x3E;	// con transicion L->H

}

void config_P4_LEDS (void) {
	P4SEL &= 0x00; //Configuracio entrada/sortida
	P4DIR |= 0xFF; //Configuracio de sortida
	P4OUT &= 0x00; //Per començar estan tots apagats
}


/*---------------------------------------------
        FUNCIONS PER ENCENDRE ELS LEDS COM VOLGUEM
----------------------------------------------*/
void lightLeftLeds(void){
    P4OUT &= 0x00; //Ens assegurem que tots estan apagats
    P4OUT |= 0x03; //Nomes encenem els que ens interessen
                  //Encenem els dos de l'esquerra (menys pes)
}


void lightCenterLeds(void){
    P4OUT &= 0x00; //Ens assegurem que tots estan apagats
    P4OUT |= 0x18; //Nomes encenem els que ens interessen
                  //Encenem dos del centre
}

void lightRightLeds(void){
    P4OUT &= 0x00; //Ens assegurem que tots estan apagats
    P4OUT |= 0xC0; //Nomes encenem els que ens interessen
                  //Encenem els dos de la dreta (mes pes)
}
