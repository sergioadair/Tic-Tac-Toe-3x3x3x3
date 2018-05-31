#include <conio.h>
#include <iostream>
#include <wchar.h>
#include <locale.h>
#include <graphics.h>
#include <windows.h>

using namespace std;

// Código de las teclas
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
#define ESPACIO 32

void ConfigConsola();
void MenuPrincipal();
void ControlMenu();
void Jugar();
void Instrucciones();
void Creditos();
void gotoxy(int x, int y);
void LibreSelec();
void Controlador(char caracter);
void EvaluarGatito(int indiceGatote, char caracter);
void VerificarGanador(char caracter);
int Px_A_Consola(int pixel, char modalidad);
void DibujarTurno(int tamLetra, int x, int y, char anterior_jugador[], char actual_jugador[], char ant_car[], char act_car[], COLORREF color);
void DibujarGato(int largoLinea, int puntoInicioX, int puntoInicioY, COLORREF color);
void DibujarGatitos();
void DibujarRect(int puntoInicioX, int puntoInicioY, COLORREF color);
void DibujarXO(COLORREF color);
void BorrarGatito(char caracter, int x_gato, int y_gato);
void DibujarLineaGanador(int inicioX, int inicioY, int finX, int finY, COLORREF color);
void DibujarMenu();
void DibujarCirculoMenu(int inicialX, int inicialY, int finX, int finY, COLORREF color);
void BorrarDibujos();
void DibujarGanador(char ganador[]);

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
COLORREF purple = RGB(255,0,255);
COLORREF yellow = RGB(255,255,0);
COLORREF black = RGB(0,0,0);
COLORREF white = RGB(255,255,255);
// Base para los dibujos
COLORREF color_meta = blue;
COLORREF color_gatitos = red;
COLORREF color_rectLibre = cyan;
COLORREF color_rectElegido = green;
COLORREF color_XO = yellow;
COLORREF color_lineaGanador = purple;
COLORREF color_letraTurno = purple;

// Dibujos
const double REL_GATITOS_META = 665.0/150.0;
const int REFY_METAGATO = 10;
int refX_metagato, margenGatitos, tam_gatitos, inicioX, inicioY, tamRectangulo, posXRect, posYRect;
int tam_letra = 150;

// Controles
int cant_moverX, cant_moverY;
// Nombres
char jugador1[20], jugador2[20];

// Gameplay
bool game_over = false, ganoGato = false, terminarProg = false;
int indiceMeta = 0, indiceGatito = 0;
char celda[9][9], completado[9];

void ConfigConsola(){
	// Maximiza el tamaño de la consola
	system("mode 650");
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	// Obtiene el ancho y alto de la consola
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	ancho_consola = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	alto_consola = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	alto_px = alto_consola*YREL_CONS_PX;
    ancho_px = ancho_consola*XREL_CONS_PX;
    
    // Permitir acentos
    setlocale(LC_ALL, "");
}

void gotoxy(int x, int y){
    COORD coord = {x-1,y-1};
    SetConsoleCursorPosition(hConsole,coord);
}

void Jugar(){
	char caracter, *ant_jugador, *sig_jugador, *ant_caracter, *sig_caracter;
	
	tam_gatitos = alto_px/REL_GATITOS_META;
    refX_metagato = 3*(ancho_px - alto_px)/4;
    margenGatitos = (alto_px/3 - tam_gatitos)/2 + 1;
    DibujarGato(alto_px, refX_metagato, REFY_METAGATO, color_meta); // Meta gato
    DibujarGatitos();
    
    cant_moverX = Px_A_Consola(tam_gatitos/3, 'x');
	cant_moverY = Px_A_Consola(tam_gatitos/3, 'y');
	caracter = 'O'; // Aquí poner el caracter contrario al que se quiere empezar
	ant_jugador = " ";
    sig_jugador = jugador1;
    ant_caracter = "O";
    sig_caracter = "X";
	DibujarTurno(48, 3*(ancho_px - alto_px)/8, REFY_METAGATO, ant_jugador, sig_jugador, ant_caracter, sig_caracter, color_letraTurno);
	
	tamRectangulo = tam_gatitos + margenGatitos;
    LibreSelec();
	
    while(game_over != true){
    	if(caracter == 'O'){
    		caracter = 'X';
		    Controlador(caracter);
		    // Jugó jugador1
    		ant_jugador = jugador1;
    		sig_jugador = jugador2;
    		ant_caracter = "X";
    		sig_caracter = "O";
		}else{
			caracter = 'O';
		    Controlador(caracter);
		    // Jugó jugador2
			ant_jugador = jugador2;
    		sig_jugador = jugador1;
    		ant_caracter = "O";
    		sig_caracter = "X";
		}
		if(game_over != true)
			DibujarTurno(48, 3*(ancho_px - alto_px)/8, REFY_METAGATO, ant_jugador, sig_jugador, ant_caracter, sig_caracter, color_letraTurno);
		
		if(completado[indiceMeta] == 'X' || completado[indiceMeta] == 'O' || completado[indiceMeta] == 'G')
			if(game_over != true) LibreSelec();
	}
	gotoxy(2,alto_consola-2);
	system("pause");
	BorrarDibujos();
	//Sleep(50); // Para que no borre al ganador
	gotoxy(1,1);
    system("cls");
    if(ganoGato != true)
		switch(caracter){
			case 'X':
				DibujarGanador(jugador1);
				break;
			case 'O':
				DibujarGanador(jugador2);
				break;
		}
	else DibujarGanador("Don Gato");
	
	// Limpia las variables usadas
	game_over = false;
	if(ganoGato == true) ganoGato = false;
	for(int i=0; i<9; i++){
		completado[i] = ' ';
		for(int j=0; j<9; j++) celda[i][j] = *"";
	}
	indiceGatito = 0;
	caracter = ' '; // Se limpia caracter para que no escriba en el menú principal
}

void Instrucciones(){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	// Escribe todo
    RECT rect_inst;
	HFONT tituloFont, textoFont;
	
	int tamLetra = 40;
	bool subrayado = TRUE, italica = FALSE;
	
	SetBkColor(device_context,black);
	
	// Título
	tituloFont = CreateFont(tamLetra,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Impact"));
    SelectObject(device_context, tituloFont);
	
	SetRect(&rect_inst, 15,2,100,100);
	SetTextColor(device_context,white);
	DrawText(device_context, TEXT("INSTRUCCIONES"), -1,&rect_inst, DT_NOCLIP | DT_LEFT | DT_SINGLELINE);
	
	// Texto
	subrayado= FALSE;
	textoFont = CreateFont(tamLetra/2,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Arial"));
    SelectObject(device_context, textoFont);
	
	SetRect(&rect_inst, 15,tamLetra+22,100,100);
	DrawText(device_context, TEXT({
		"CONTROLES\n"
		"Flechas: Desplazarte por las opciones/casillas.\n"
		"Espacio: Seleccionar.\n"
		"Enter: Aceptar el nombre introducido.\n\n"
		"REGLAS DEL JUEGO\n"
		"El tablero se conforma por un gato grande 'metagato' el cual consta de 9 casillas, las cuales también cuentan con gatitos de 9 casillas cada uno. El primer jugador elegirá\n"
		"dónde colocar su primer movimiento (en la casillas de un gatito de la casillas del metagato) y, según el espacio elegido del gatito, el siguiente jugador será enviado a la respectiva\n"
		"celda del metagato. Igualmente, el primer jugador será enviado a la respectiva casilla del metagato cuando el segundo jugador haya seleccionado una casilla del gatito al que\n"
		"fue enviado, así sucesivamente.\n"
		"Una celda del metagato se da por completada cuando uno de los jugadores gana en este gatito, entonces se apodera de la celda del metagato; también cuando ya no hay\n"
		"más lugares dónde jugar dentro del gatito.\n"
		"Si el jugador es mandado a una casilla completada, este elegirá libremente la casilla del metagato donde hará su movimiento (que no esté completada).\n"
		"El juego termina cuando no queden más movimientos por hacer o cuando algún jugador haya completado una linea de 3 casillas del metagato (de las cuales se apoderó), en cuyo\n"
		"caso este jugador GANA."
		}), -1,&rect_inst, DT_NOCLIP | DT_LEFT);
	
	ReleaseDC(console_handle, device_context);
    DeleteObject(tituloFont);
    DeleteObject(textoFont);
}

void Creditos(){
		/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	// Escribe todo
    RECT rect_inst;
	HFONT tituloFont, textoFont;
	
	int tamLetra = 70;
	bool subrayado = TRUE, italica = FALSE;
	
	SetBkColor(device_context,black);
	
	// Título
	tituloFont = CreateFont(tamLetra,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Impact"));
    SelectObject(device_context, tituloFont);
	
	SetRect(&rect_inst, ancho_px-20,10,100,100);
	SetTextColor(device_context,white);
	DrawText(device_context, TEXT("CRÉDITOS"), -1,&rect_inst, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	
	// Texto
	subrayado= FALSE;
	textoFont = CreateFont(tamLetra/2,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Impact"));
    SelectObject(device_context, textoFont);
	
	SetRect(&rect_inst, ancho_px-20,tamLetra+30,100,100);
	DrawText(device_context, TEXT({
		"Licenciatura en Ciencias de la Computación\n"
		"Clase de programación de computadoras\n\n"
		"Desarrolladores:\n"
		"Sergio Adair López Sánchez\n"
		"Vesna Camile Pivac Alcaraz\n"
		"Esteban Rafael Estrada Corona\n\n"
		"Maestra:\n"
		"Irene Rodriguez Castillo"
		}), -1,&rect_inst, DT_NOCLIP | DT_CENTER);
	
	ReleaseDC(console_handle, device_context);
    DeleteObject(tituloFont);
    DeleteObject(textoFont);
}

void LibreSelec(){
	int c, contX = 0, contY = 0;
	posXRect = refX_metagato+(margenGatitos/2);
	posYRect = REFY_METAGATO+(margenGatitos/2);
	indiceMeta = 0;
	
	gotoxy(1,1);
    
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
	
	inicioX = Px_A_Consola(posXRect + margenGatitos/2 + tam_gatitos/6, 'x')+1;
    inicioY = Px_A_Consola(posYRect + margenGatitos/2 + tam_gatitos/6, 'y')+1;
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
						// Se dibujan antes de Verificar para no sobreescribir la linea del ganador
						DibujarGato(alto_px, refX_metagato, REFY_METAGATO, color_meta); // Meta gato
	        			DibujarGatitos();
						DibujarXO(color_XO);
						
						VerificarGanador(completado[indiceMeta]);
					}else{
						// Evita que se borren los dibujos
		                DibujarGato(alto_px, refX_metagato, REFY_METAGATO, color_meta); // Meta gato
	        			DibujarGatitos();
	        			DibujarXO(color_XO);
					}
					
					// Se manda el rectangulo a la posición elegida
					posXRect = refX_metagato + (margenGatitos/2) + (alto_px/3)*contX;
					posYRect = REFY_METAGATO + (margenGatitos/2) + (alto_px/3)*contY;
					indiceMeta = indiceGatito;
	                indiceGatito = 0;
	                
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
	int posX = refX_metagato+(margenGatitos/2);
	int posY = REFY_METAGATO+(margenGatitos/2);
	
	// Horizontal superior
	if(completado[0] == caracter && completado[1] == caracter && completado[2] == caracter){
		DibujarLineaGanador(posX, REFY_METAGATO+(alto_px/6), posX+alto_px-margenGatitos, REFY_METAGATO+(alto_px/6), color_lineaGanador);
		game_over = true;
	}
	
	// Horizontal media
	if(completado[3] == caracter && completado[4] == caracter && completado[5] == caracter){
		DibujarLineaGanador(posX, REFY_METAGATO+(3*alto_px/6), posX+alto_px-margenGatitos, REFY_METAGATO+(3*alto_px/6), color_lineaGanador);
		game_over = true;
	}
	
	// Horizontal inferior
	if(completado[6] == caracter && completado[7] == caracter && completado[8] == caracter){
		DibujarLineaGanador(posX, REFY_METAGATO+(5*alto_px/6), posX+alto_px-margenGatitos, REFY_METAGATO+(5*alto_px/6), color_lineaGanador);
		game_over = true;
	}
	
	// Vertical izquierda
	if(completado[0] == caracter && completado[3] == caracter && completado[6] == caracter){
		DibujarLineaGanador(refX_metagato+(alto_px/6), posY, refX_metagato+(alto_px/6), posY+alto_px-margenGatitos, color_lineaGanador);
		game_over = true;
	}
	
	// Vertical media
	if(completado[1] == caracter && completado[4] == caracter && completado[7] == caracter){
		DibujarLineaGanador(refX_metagato+(3*alto_px/6), posY, refX_metagato+(3*alto_px/6), posY+alto_px-margenGatitos, color_lineaGanador);
		game_over = true;
	}

	// Vertical derecha
	if(completado[2] == caracter && completado[5] == caracter && completado[8] == caracter){
		DibujarLineaGanador(refX_metagato+(5*alto_px/6), posY, refX_metagato+(5*alto_px/6), posY+alto_px-margenGatitos, color_lineaGanador);
		game_over = true;
	}
	
	// Diagonal 048
	if(completado[0] == caracter && completado[4] == caracter && completado[8] == caracter){
		DibujarLineaGanador(posX, posY, posX+alto_px-margenGatitos, posY+alto_px-margenGatitos, color_lineaGanador);
		game_over = true;
	}
		
	// Diagonal 246
	if(completado[2] == caracter && completado[4] == caracter && completado[6] == caracter){
		DibujarLineaGanador(posX+alto_px-margenGatitos, posY, posX, posY+alto_px-margenGatitos, color_lineaGanador);
		game_over = true;
	}
		
	//------ Verificar si todas las celdas están llenas -----//
	if(game_over != true){
		int contMarcado = 0;
		for(int i=0; i<9; i++){
			if(completado[i] == 'X' || completado[i] == 'O' || completado[i] == 'G') contMarcado++;
		}
		if(contMarcado == 9){
			game_over = true;
			ganoGato = true;
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

void DibujarTurno(int tamLetra, int x, int y, char anterior_jugador[], char actual_jugador[], char ant_car[], char act_car[], COLORREF color){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	RECT rect;
	HFONT hFont;
	
	bool subrayado = FALSE, italica = FALSE;
	hFont = CreateFont(tamLetra,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Impact"));
    SelectObject(device_context, hFont);
    SetBkColor(device_context,black);
	
	// Se borra el nombre anterior
	SetRect(&rect, x,y+tamLetra,100,100);
	SetTextColor(device_context,black);
	DrawText(device_context, TEXT(anterior_jugador), -1,&rect, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	
	// Se borra el caracter anterior
	SetRect(&rect, x,y+2*tamLetra,100,100);
	SetTextColor(device_context,black);
	DrawText(device_context, TEXT(ant_car), -1,&rect, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	
	// Se escribe el nuevo nombre
	SetRect(&rect, x,y,100,100);
	SetTextColor(device_context,color);
	// Al usar DT_NOCLIP no importa el tamaño del rectángulo
	DrawText(device_context, TEXT("Turno de"), -1,&rect, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	SetRect(&rect, x,y+tamLetra,100,100);
	DrawText(device_context, TEXT(actual_jugador), -1,&rect, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	
	// Se escribe el nuevo caracter
	SetRect(&rect, x,y+2*tamLetra,100,100);
	DrawText(device_context, TEXT(act_car), -1,&rect, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	
	ReleaseDC(console_handle, device_context);
    DeleteObject(hFont);
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
	    		DibujarGato(tam_gatitos, refX_metagato+ordenX_gato, REFY_METAGATO+ordenY_gato, color_gatitos);
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
					LineTo(device_context, ordenX+tam_gatitos, ordenY+tam_gatitos);
					// Diagonal 2
					MoveToEx(device_context, ordenX+tam_gatitos, ordenY, NULL);
					LineTo(device_context, ordenX, ordenY+tam_gatitos);
					break;
				case 'O':
					// Círculo
					Arc(device_context, ordenX, ordenY, ordenX+tam_gatitos, ordenY+tam_gatitos, 0, 0, 0, 0);
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
	
	DibujarGato(tam_gatitos, x_gato, y_gato, black);
	
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
}

void BorrarDibujos(){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	int grosor = alto_px*2;
	HPEN pen = CreatePen(PS_SOLID,grosor,black);
	SelectObject(device_context,pen);
	
	// Borra todo
	MoveToEx(device_context, 1, 1, NULL);
	LineTo(device_context, ancho_px+50, 1);
	MoveToEx(device_context, 1, alto_px/2, NULL);
	LineTo(device_context, ancho_px+50, alto_px/2);
	
	// Liberamos el device context y eliminamos la pluma
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
}

void DibujarLineaGanador(int inicioX, int inicioY, int finX, int finY, COLORREF color){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	int grosor = 5;
	HPEN pen = CreatePen(PS_SOLID,grosor,color);
	SelectObject(device_context,pen);
	
	MoveToEx(device_context, inicioX, inicioY, NULL);
	LineTo(device_context, finX, finY);
	
	// Liberamos el device context y eliminamos la pluma
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
}

void DibujarMenu(){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	BorrarDibujos();
    
    // Escribe todo
    RECT rect_menu;
	HFONT tituloFont, opcionesFont;
	int ordenY = tam_letra+30;
	bool subrayado = FALSE, italica = FALSE;
	
	SetBkColor(device_context,black);
	
	tituloFont = CreateFont(tam_letra,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Impact"));
    SelectObject(device_context, tituloFont);
	
	// Título
	SetRect(&rect_menu, ancho_px,10,100,100);
	SetTextColor(device_context,purple);
	DrawText(device_context, TEXT("GATO 3x3x3x3"), -1,&rect_menu, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	// Jugar
	opcionesFont = CreateFont(tam_letra/2,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Impact"));
    SelectObject(device_context, opcionesFont);
    
	SetRect(&rect_menu, ancho_px,ordenY,100,100);
	SetTextColor(device_context,white);
	DrawText(device_context, TEXT("JUGAR"), -1,&rect_menu, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	// Instrucciones
	ordenY += tam_letra/2 + 20;
	SetRect(&rect_menu, ancho_px,ordenY,100,100);
	DrawText(device_context, TEXT("INSTRUCCIONES"), -1,&rect_menu, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	// Créditos
	ordenY += tam_letra/2 + 20;
	SetRect(&rect_menu, ancho_px,ordenY,100,100);
	DrawText(device_context, TEXT("CRÉDITOS"), -1,&rect_menu, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	// Salir
	ordenY += tam_letra/2 + 20;
	SetRect(&rect_menu, ancho_px,ordenY,100,100);
	DrawText(device_context, TEXT("SALIR"), -1,&rect_menu, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	
	ReleaseDC(console_handle, device_context);
    DeleteObject(tituloFont);
    DeleteObject(opcionesFont);
    
}

void DibujarCirculoMenu(int inicialX, int inicialY, int finX, int finY, COLORREF color){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	
	int grosor = 10;
	HPEN pen = CreatePen(PS_SOLID,grosor,color);
	SelectObject(device_context,pen);
	
	Arc(device_context, inicialX, inicialY, finX, finY, 0, 0, 0, 0);
	
	// Liberamos el device context y eliminamos la pluma
    ReleaseDC(console_handle, device_context);
    DeleteObject(pen);
}

void DibujarGanador(char ganador[]){
	/*	Obtiene la consola y el device context
	necesarios para dibujar  --No sacar de la función-- */
	HWND console_handle = GetConsoleWindow();
	HDC device_context = GetDC(console_handle);
	RECT rect;
	HFONT hFont;
	int tamLetra = 180;
	bool subrayado = FALSE, italica = FALSE;
	
	hFont = CreateFont(tamLetra,0,0,0,FW_DONTCARE,italica,subrayado,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
            CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT("Impact"));
    SelectObject(device_context, hFont);
    SetBkColor(device_context,black);
	SetTextColor(device_context,yellow);
	
	SetRect(&rect, ancho_px,75,100,100);
	DrawText(device_context, TEXT("EL GANADOR ES"), -1,&rect, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	SetRect(&rect, ancho_px,75+tamLetra,100,100);
	DrawText(device_context, TEXT(ganador), -1,&rect, DT_NOCLIP | DT_CENTER | DT_SINGLELINE);
	
	ReleaseDC(console_handle, device_context);
    DeleteObject(hFont);
}

void ControlMenu(){
	int c, contY = 0, tamCirculo = 10;
	int posX = ancho_px/3;
	int posY = 5*tam_letra/4 + 30;
	
	gotoxy(1,1);
    
    while(true){
    	switch(contY){ // Pone el circulito al lado de la opcion del menú
    		case 0:
    			posX = ancho_px/2 - 60;
    			break;
    		case 1:
    			posX = ancho_px/2 - 175;
    			break;
    		case 2:
    			posX = ancho_px/2 - 100;
    			break;
    		case 3:
    			posX = ancho_px/2 - 50;
    			break;
    	}
    	DibujarCirculoMenu(posX, posY, posX+tamCirculo, posY+tamCirculo, green);
    	
    	c = 0;
    	switch(c = _getch()){
    		case ARRIBA:
                if(contY > 0){
                	DibujarCirculoMenu(posX, posY, posX+tamCirculo, posY+tamCirculo, black);
                	posY -= tam_letra/2 + 20;
                	contY--;
				}
                break;
            case ABAJO:
                if(contY < 3){
                	DibujarCirculoMenu(posX, posY, posX+tamCirculo, posY+tamCirculo, black);
                	posY += tam_letra/2 + 20;
                	contY++;
				}
                break;
            case ESPACIO:
            	switch(contY){
            		case 0:
            			// JUGAR
            			BorrarDibujos();
            			gotoxy(ancho_consola/2-26, alto_consola/2-4);
            			cout << "Introduzca el nombre del jugador 1: ";
            			cin.getline(jugador1, 20);
            			cout << endl;
            			gotoxy(ancho_consola/2-26, alto_consola/2-3);
            			cout << "Introduzca el nombre del jugador 2: ";
            			cin.getline(jugador2, 20);
            			
            			gotoxy(1,1);
            			system("cls");
            			
            			Jugar();
            			system("pause"); // Pausa de la pantalla de ganador
            			return;
            		case 1:
            			// INSTRUCCIONES
            			BorrarDibujos();
            			Instrucciones();
            			gotoxy(2,alto_consola-2);
            			system("pause");
            			return;
            		case 2:
            			// CRÉDITOS
            			BorrarDibujos();
            			Creditos();
            			gotoxy(2,alto_consola-2);
            			system("pause");
            			return;
            		case 3:
            			// SALIR
            			//BorrarDibujos();
            			terminarProg = true;
            			return;
            	}
            	
                break;
		}
	}
}

void MenuPrincipal(){
	gotoxy(1,1);
	system("cls");
	
	while(terminarProg != true){
		DibujarMenu();
		ControlMenu();
	}
	return;
	
}
