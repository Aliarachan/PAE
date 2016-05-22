
/** Libreria de funciones de pantalla y menu. **/
#include <msp430x54xA.h>
#include <stdio.h>
#include <stdlib.h>
#include "hal_lcd.h"
#include "comunication.h"
#include "definedValues.h"
#include "pantalla.h"

char cadena[17];
unsigned char contraste = 0x64;
unsigned char iluminacion = 30;
unsigned char linea = 1;
char borrado[] = "                 ";

/**************************************************************************
 * INICIALIZACIÓN DE PANTALLA.
 * Inicializamos la pantallita LCD:
 *
 * Sin datos de entrada
 *
 * Sin datos de salida
 *
 **************************************************************************/

void init_LCD(void) {

	halLcdInit();  //Programa interno para iniciar la pantalla
	halLcdBackLightInit(); // Inicio de la retroiluminación de la pantalla
	halLcdSetBackLight(iluminacion); // Determinación del nivel de retroiluminación de la pantalla
	halLcdSetContrast(contraste); //Establecimiento del contraste
	halLcdClearScreen();  //Limpiar (borrar) la pantalla
}




void escribir(char String[], unsigned char Linea)

{
	halLcdPrintLine(String, Linea, OVERWRITE_TEXT); //superponemos la nueva palabra introducida, haya o no algo.
}

void borrar(unsigned char Linea) {
	halLcdPrintLine(borrado, Linea, OVERWRITE_TEXT); //incluimos una linea en blanco
}


//void pintaMenu( byte linea, byte eleccion_menu)
//{
//    char *lista_menu[] = {"Modo_Testespacioscomocolumnas", "Modo_Pared"};
//    int numOpciones = 2;
//    byte opcion;
//    halLCDPrintLine("  ROBOT PAE   ", linea++, OVERWRITE_TEXT);
//    for(int opcion = 1, opcion < numOpciones, opcion++){
//        if(eleccion == opcionMenu)
//        {
//            halLCDPrintLine(lista_menu[opcion-1], INVERT_TEXT | OVERWRITE_TEXT);
//
//        }
//        else halLCDPrintLine(lista_menu[opcion-1], linea++, OVERWRITE_TEXT);
//    }
//}
