
/** Libreria de funciones de pantalla y menu. **/


void pintaMenu( byte linea, byte eleccion_menu)
{
    char *lista_menu[] = {"Modo_Testespacioscomocolumnas", "Modo_Pared"};
    int numOpciones = 2;
    byte opcion;
    halLCDPrintLine("  ROBOT PAE   ", linea++, OVERWRITE_TEXT);
    for(int opcion = 1, opcion < numOpciones, opcion++){
        if(eleccion == opcionMenu)
        {
            halLCDPrintLine(lista_menu[opcion-1], INVERT_TEXT | OVERWRITE_TEXT);
            
        }
        else halLCDPrintLine(lista_menu[opcion-1], linea++, OVERWRITE_TEXT);
    }   
}