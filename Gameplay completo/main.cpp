/*	Para dibujar con GDI el proyecto necesita las
	configuraciones de graphics.h incluyendo los comandos en Linker
*/

#include <conio.h>
#include <iostream>
#include <graphics.h>
#include <windows.h>

using namespace std;

// Código de las teclas
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
#define ESPACIO 32

#define REFY_METAGATO 10
#define TAM_GATITOS 150

void ConfigConsola();
void gotoxy(int x, int y);
void LibreSelec();
void Controlador(char caracter);
void EvaluarGatito(int indiceGatote, char caracter);
void VerificarGanador(char caracter);
int Px_A_Consola(int pixel, char modalidad);
void DibujarGato(int largoLinea, int puntoInicioX, int puntoInicioY, COLORREF color);
void DibujarGatitos();
void DibujarRect(int puntoInicioX, int puntoInicioY, COLORREF color);
void DibujarXO(COLORREF color);
void BorrarGatito(char caracter, int x_gato, int y_gato);

// Handle de la consola usado en gotoxy
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

// Pantalla
const double XREL_CONS_PX = 7.9761904;
const double YREL_CONS_PX = 11.875;
int ancho_consola = 0, alto_consola = 0, alto_px, ancho_px;

// Colores
COLORREF red = RGB(255,0,0);
COLORREF green = RGB(0,255,0);
COLORREF blue = RGB(0,0,255);
COLORREF cyan = RGB(0,255,255);
//COLORREF purple = RGB(255,0,255);
COLORREF yellow = RGB(255,255,0);
COLORREF black = RGB(0,0,0);
// Base para los dibujos
COLORREF color_meta = blue;
COLORREF color_gatitos = red;
COLORREF color_rectLibre = cyan;
COLORREF color_rectElegido = green;
COLORREF color_XO = yellow;

// Dibujos
int refX_metagato, margenGatitos, inicioX, inicioY, tamRectangulo, posXRect, posYRect;

// Controles
int cant_moverX, cant_moverY;

// Gameplay
bool game_over = false;
int indiceMeta = 0, indiceGatito = 0;
char celda[9][9], completado[9];

// ---------------------------------------------------------------------- //

int main(){
	char caracter;
	int orden_gato;
	
    ConfigConsola();
    
    Sleep(50); // Para que el dibujo no desaparezca al maximizar
    alto_px = alto_consola*YREL_CONS_PX;
    ancho_px = ancho_consola*XREL_CONS_PX;
    refX_metagato = (ancho_px - alto_px)/2;
    margenGatitos = (alto_px/3 - TAM_GATITOS)/2 + 1;
    DibujarGato(alto_px, refX_metagato, REFY_METAGATO, color_meta); // Meta gato
    DibujarGatitos();
    
    tamRectangulo = TAM_GATITOS + margenGatitos;
    LibreSelec();
    
    cant_moverX = Px_A_Consola(TAM_GATITOS/3, 'x');
	cant_moverY = Px_A_Consola(TAM_GATITOS/3, 'y');
	caracter = 'O'; // Aquí poner el caracter contrario al que se quiere empezar
    while(game_over != true){
    	if(caracter == 'O'){
    		caracter = 'X';
		    Controlador(caracter);
		}else{
			caracter = 'O';
		    Controlador(caracter);
		}
		if(completado[indiceMeta] == 'X' || completado[indiceMeta] == 'O' || completado[indiceMeta] == 'G')
			if(game_over != true) LibreSelec();
	}
    
    gotoxy(1,alto_consola);
    system("pause");
    return 0;
}

void ConfigConsola(){
	// Maximiza el tamaño de la consola
	system("mode 650");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	// Obtiene el ancho y alto de la consola
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	ancho_consola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	alto_consola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void gotoxy(int x, int y){
    COORD coord = {x-1,y-1};
    SetConsoleCursorPosition(hConsole,coord);
}

void LibreSelec(){
	int c, contX = 0, contY = 0;
	posXRect = refX_metagato+(margenGatitos/2);
	posYRect = REFY_METAGATO+(margenGatitos/2);
	indiceMeta = 0;
    
    while(true){
    	DibujarRect(posXRect, posYRect, color_rectLibre);
    	
    	c = 0;
    	switch(c = _getch()){
    		case ARRIBA:
                if(contY > 0){
                	DibujarRect(posXRect, posYRect, black);
                	posYRect -= alto_px/3;
                	indiceMeta -= 3;
                	contY--;
				}
                break;
            case ABAJO:
                if(contY < 2){
                	DibujarRect(posXRect, posYRect, black);
                	posYRect += alto_px/3;
                	indiceMeta += 3;
                	contY++;
				}
                break;
            case IZQUIERDA:
                if(contX > 0){
                	DibujarRect(posXRect, posYRect, black);
                	posXRect -= alto_px/3;
                	indiceMeta--;
                	contX--;
				}
                break;
            case DERECHA:
                if(contX < 2){
                	DibujarRect(posXRect, posYRect, black);
                	posXRect += alto_px/3;
                	indiceMeta++;
                	contX++;
				}
                break;
            case ESPACIO:
                if(completado[indiceMeta] != 'X' && completado[indiceMeta] != 'O' && completado[indiceMeta] != 'G'){
                	return;
				}else{
					// Efecto "No puedes poner aquí" del rectangulo
					DibujarRect(posXRect, posYRect, red);
					Sleep(100);
				}
                break;
		}
	}
	
}

void Controlador(char caracter){
	int c, ordenX, ordenY, contX = 0, contY = 0;
	
	inicioX = Px_A_Consola(posXRect + margenGatitos/2 + TAM_GATITOS/6, 'x')+1;
    inicioY = Px_A_Consola(posYRect + margenGatitos/2 + TAM_GATITOS/6, 'y')+1;
    ordenX = inicioX;
	ordenY = inicioY;
    gotoxy(inicioX, inicioY);
    
    DibujarRect(posXRect, posYRect, color_rectElegido);

    while(true){
    	c = 0;
        switch(c = _getch()){
            case ARRIBA:
                if(contY > 0){
                	ordenY -= cant_moverY;
                	gotoxy(ordenX, ordenY);
                	indiceGatito -= 3;
                	contY--;
				}
                break;
            case ABAJO:
                if(contY < 2){
                	ordenY += cant_moverY;
                	gotoxy(ordenX, ordenY);
                	indiceGatito += 3;
                	contY++;
				}
                break;
            case IZQUIERDA:
                if(contX > 0){
                	ordenX -= cant_moverX;
                	gotoxy(ordenX, ordenY);
                	indiceGatito--;
                	contX--;
				}
                break;
            case DERECHA:
            	if(contX < 2){
                	ordenX += cant_moverX;
                	gotoxy(ordenX, ordenY);
                	indiceGatito++;
                	contX++;
			    }
                break;
            case ESPACIO:
            	if(celda[indiceMeta][indiceGatito] == *""){
            		
            		celda[indiceMeta][indiceGatito] = caracter;
	                cout << celda[indiceMeta][indiceGatito];
	                
	                DibujarRect(posXRect, posYRect, black);
					
					EvaluarGatito(indiceMeta, celda[indiceMeta][indiceGatito]);
					// Si esta completo se dibuja el gatito completado
					if(completado[indiceMeta] == 'X' || completado[indiceMeta] == 'O' || completado[indiceMeta] == 'G'){
						BorrarGatito(completado[indiceMeta], posXRect + (margenGatitos/2), posYRect + (margenGatitos/2));
						VerificarGanador(completado[indiceMeta]); // Si funciona no lo toques
					}
					
					// Se manda el rectangulo a la posición elegida
					posXRect = refX_metagato + (margenGatitos/2) + (alto_px/3)*contX;
					posYRect = REFY_METAGATO + (margenGatitos/2) + (alto_px/3)*contY;
					indiceMeta = indiceGatito;
	                indiceGatito = 0;
	                
	                // Evita que se borren los gatos
	                DibujarGato(alto_px, refX_metagato, REFY_METAGATO, color_meta); // Meta gato
        			DibujarGatitos();
        			DibujarXO(color_XO);
	                
	                return;
	            }
                break;
        }
    }

}

void EvaluarGatito(int indiceGatote, char caracter){
	
	//----- Verificar si el jugador ha ganado el gatito -----//
	
	// Horizontal superior
	if(celda[indiceGatote][0] == caracter && celda[indiceGatote][1] == caracter && celda[indiceGatote][2] == caracter)
		completado[indiceGatote] = caracter;
	
	// Horizontal media
	if(celda[indiceGatote][3] == caracter && celda[indiceGatote][4] == caracter && celda[indiceGatote][5] == caracter)
		completado[indiceGatote] = caracter;
	
	// Horizontal inferior
	if(celda[indiceGatote][6] == caracter && celda[indiceGatote][7] == caracter && celda[indiceGatote][8] == caracter)
		completado[indiceGatote] = caracter;
	
	// Vertical izquierda
	if(celda[indiceGatote][0] == caracter && celda[indiceGatote][3] == caracter && celda[indiceGatote][6] == caracter)
		completado[indiceGatote] = caracter;
	
	// Vertical media
	if(celda[indiceGatote][1] == caracter && celda[indiceGatote][4] == caracter && celda[indiceGatote][7] == caracter)
		completado[indiceGatote] = caracter;

	// Vertical derecha
	if(celda[indiceGatote][2] == caracter && celda[indiceGatote][5] == caracter && celda[indiceGatote][8] == caracter)
		completado[indiceGatote] = caracter;
	
	// Diagonal 048
	if(celda[indiceGatote][0] == caracter && celda[indiceGatote][4] == caracter && celda[indiceGatote][8] == caracter)
		completado[indiceGatote] = caracter;
		
	// Diagonal 246
	if(celda[indiceGatote][2] == caracter && celda[indiceGatote][4] == caracter && celda[indiceGatote][6] == caracter)
		completado[indiceGatote] = caracter;
			
	//------ Verificar si todas las celdas están llenas -----//
	if(completado[indiceGatote] != caracter){
		int contMarcado = 0;
		for(int i=0; i<9; i++){
			if(celda[indiceGatote][i] == 'X' || celda[indiceGatote][i] == 'O') contMarcado++;
		}
		if(contMarcado == 9){
			completado[indiceGatote] = 'G';
		}
	}
	
}

void VerificarGanador(char caracter){
	
	// Horizontal superior
	if(completado[0] == caracter && completado[1] == caracter && completado[2] == caracter)
		game_over = true;
	
	// Horizontal media
	if(completado[3] == caracter && completado[4] == caracter && completado[5] == caracter)
		game_over = true;
	
	// Horizontal inferior
	if(completado[6] == caracter && completado[7] == caracter && completado[8] == caracter)
		game_over = true;
	
	// Vertical izquierda
	if(completado[0] == caracter && completado[3] == caracter && completado[6] == caracter)
		game_over = true;
	
	// Vertical media
	if(completado[1] == caracter && completado[4] == caracter && completado[7] == caracter)
		game_over = true;

	// Vertical derecha
	if(completado[2] == caracter && completado[5] == caracter && completado[8] == caracter)
		game_over = true;
	
	// Diagonal 048
	if(completado[0] == caracter && completado[4] == caracter && completado[8] == caracter)
		game_over = true;
		
	// Diagonal 246
	if(completado[2] == caracter && completado[4] == caracter && completado[6] == caracter)
		game_over = true;
		
	//------ Verificar si todas las celdas están llenas -----//
	if(game_over != true){
		int contMarcado = 0;
		for(int i=0; i<9; i++){
			if(completado[i] == 'X' || completado[i] == 'O' || completado[i] == 'G') contMarcado++;
		}
		if(contMarcado == 9){
			game_over = true;
		}
	}
	
}

int Px_A_Consola(int pixel, char modalidad){
	int resultado = 0;
	
	switch(modalidad){
		case 'x':	resultado = pixel / XREL_CONS_PX;
			break;
		case 'y':	resultado = pixel / YREL_CONS_PX;
			break;
	}
	return resultado;
}

void DibujarGato(int largoLinea, int puntoInicioX, int puntoInicioY, COLORREF color){
	int posX, posY, grosor = 5;
	
	/*	Obtiene la consola y el device context
		necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	HPEN pen = CreatePen(PS_SOLID,grosor,color);
	SelectObject(device_context,pen);
	
	// Línea horizontal superior
	posY = puntoInicioY + largoLinea/3;
	MoveToEx(device_context, puntoInicioX, posY, NULL);
	LineTo(device_context, puntoInicioX+largoLinea, posY);
	
	// Línea horizontal inferior
	posY += largoLinea/3;
	MoveToEx(device_context, puntoInicioX, posY, NULL);
	LineTo(device_context, puntoInicioX+largoLinea, posY);
	
	// Línea vertical izquierda
	posX = puntoInicioX + largoLinea/3;
	MoveToEx(device_context, posX, puntoInicioY, NULL);
	LineTo(device_context, posX, puntoInicioY+largoLinea);
	
	// Línea vertical derecha
	posX += largoLinea/3;
	MoveToEx(device_context, posX, puntoInicioY, NULL);
	LineTo(device_context, posX, puntoInicioY+largoLinea);
	
	// Liberamos el device context y eliminamos la pluma
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
}

void DibujarGatitos(){
	int ordenX_gato, ordenY_gato, c = 0;
	ordenX_gato = margenGatitos;
	ordenY_gato = ordenX_gato;
	
	for(int i=0; i<3; i++){
    	for(int j=0; j<3; j++){
    		// Si un gatito está completado entonces no lo re-dibuja
    		if(completado[c] != 'X' && completado[c] != 'O' && completado[c] != 'G'){
	    		DibujarGato(TAM_GATITOS, refX_metagato+ordenX_gato, REFY_METAGATO+ordenY_gato, color_gatitos);
	    	}
	    	ordenX_gato += alto_px/3;
	    	c++;
		}
		ordenX_gato = margenGatitos;
		ordenY_gato += alto_px/3;
	}
}

void DibujarRect(int puntoInicioX, int puntoInicioY, COLORREF color){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	int largoLinea = tamRectangulo;
	int grosor = 5;
	HPEN pen = CreatePen(PS_SOLID,grosor,color);
	SelectObject(device_context,pen);
	
	// Línea horizontal superior
	MoveToEx(device_context, puntoInicioX, puntoInicioY, NULL);
	LineTo(device_context, puntoInicioX+largoLinea, puntoInicioY);
	// Línea vertical derecha
	LineTo(device_context, puntoInicioX+largoLinea, puntoInicioY+largoLinea);
	// Línea horizontal inferior
	LineTo(device_context, puntoInicioX, puntoInicioY+largoLinea);
	// Línea vertical izquierda
	LineTo(device_context, puntoInicioX, puntoInicioY);
	
	// Liberamos el device context y eliminamos la pluma
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
}

void DibujarXO(COLORREF color){
	int ordenX, ordenY, c = 0;
	ordenX = refX_metagato+margenGatitos;
	ordenY = REFY_METAGATO+margenGatitos;
	
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	int grosor = 5;
	HPEN pen = CreatePen(PS_SOLID,grosor,color);
	SelectObject(device_context,pen);

	for(int i=0; i<3; i++){
    	for(int j=0; j<3; j++){
    		// Si un gatito está completado entonces re-dibuja las figuras
    		switch(completado[c]){
    			case 'X':
		    		// Diagonal 1
					MoveToEx(device_context, ordenX, ordenY, NULL);
					LineTo(device_context, ordenX+TAM_GATITOS, ordenY+TAM_GATITOS);
					// Diagonal 2
					MoveToEx(device_context, ordenX+TAM_GATITOS, ordenY, NULL);
					LineTo(device_context, ordenX, ordenY+TAM_GATITOS);
					break;
				case 'O':
					// Círculo
					Arc(device_context, ordenX, ordenY, ordenX+TAM_GATITOS, ordenY+TAM_GATITOS, 0, 0, 0, 0);
					break;
	    	}
	    	ordenX += alto_px/3;
	    	c++;
		}
		ordenX = refX_metagato+margenGatitos;
		ordenY += alto_px/3;
	}
	
	// Liberamos el device context y eliminamos la pluma
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
	
}

void BorrarGatito(char caracter, int x_gato, int y_gato){
	int aux_x = inicioX, aux_y = inicioY;
	
	DibujarGato(TAM_GATITOS, x_gato, y_gato, black);
	
	gotoxy(aux_x, aux_y);
	for(int i=0; i<3; i++){
		for(int j=0; j<3; j++){
			cout << " ";
			aux_x += cant_moverX;
			gotoxy(aux_x, aux_y);
		}
		aux_x = inicioX;
		aux_y += cant_moverY;
		gotoxy(aux_x, aux_y);
	}
	gotoxy(0, 0); // Solo por estética
	
}
