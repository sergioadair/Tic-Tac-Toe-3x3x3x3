/*	Para dibujar con GDI el proyecto necesita las
	configuraciones de graphics.h incluyendo los comandos en Linker
*/

#include <conio.h>
#include <iostream>
#include <graphics.h>
/*	_WIN32_WINNT no está definido en algunos IDEs así que lo
	descomentamos si es el caso (definir antes de include windows.h)
*/
//#define _WIN32_WINNT 0x0500
#include <windows.h>

using namespace std;

#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
#define ESPACIO 32

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void gotoxy(int x, int y);
void Controles(char caracter[], int cant_mover);
void Dibujar();

int limiteX;

int main(){
	
	// Maximiza el tamaño de la consola
	system("mode 650");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	// Obtiene el ancho de la pantalla
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	limiteX = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    
    Dibujar();
    Controles("X", 5);
    
    //cin.ignore();
    system("pause");
    return 0;
}

void gotoxy(int x, int y){
    COORD coord = {x-1,y-1};
    SetConsoleCursorPosition(hConsole,coord);
}

void Controles(char caracter[], int cant_mover){
    int c, ordenX = 1, ordenY = 1;
    limiteX -= cant_mover;
    
    // Unbuffer
	DWORD  mode;
	GetConsoleMode( hConsole, &mode );
	SetConsoleMode( hConsole, ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT );
    cout << flush;

    while(true){
    	c = 0;
        switch(c = _getch()){
            case ARRIBA:
                if(ordenY > cant_mover) ordenY -= cant_mover;
                break;
            case ABAJO:
                ordenY += cant_mover;
                break;
            case IZQUIERDA:
                if(ordenX > cant_mover) ordenX -= cant_mover;
                break;
            case DERECHA:
                if(ordenX < limiteX) ordenX += cant_mover;
                break;
            case ESPACIO:
                cout << caracter;
                // Insertar aqui orden de salida
                break;
        }
        gotoxy(ordenX, ordenY);
        Dibujar();
    }
    SetConsoleMode(hConsole, mode);
}

void Dibujar(){
    /*	Obtener la consola y el device context
	necesarios para dibujar */
    HWND console_handle = GetConsoleWindow();
    HDC device_context = GetDC(console_handle);

	// Linea roja de 5px de ancho
	HPEN pen = CreatePen(PS_SOLID,5,RGB(255,0,0));
	SelectObject(device_context,pen);
	MoveToEx(device_context, 300, 10, NULL);
	LineTo(device_context,300, 200);
	// Liberamos el device context
    ReleaseDC(console_handle, device_context);
}
