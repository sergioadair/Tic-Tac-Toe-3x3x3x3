/*	Para compilar con GDI el proyecto necesita las
	configuraciones de graphics.h incluyendo los comandos en Linker
*/

#include "funciones.h"

int main(){
	
    ConfigConsola();
    Sleep(50); // Para que el dibujo no desaparezca al maximizar
    
	MenuPrincipal();
    
    gotoxy(5,alto_consola-5);
    
    //system("pause");
    return 0;
}
