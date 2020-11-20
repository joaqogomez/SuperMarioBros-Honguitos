#ifndef SRC_UTILS_HPP_
#define SRC_UTILS_HPP_

const int MAX_IMAGEN_ENEMIGOS = 30,MAX_IMAGEN_BLOQUE = 30;
const int MAX_BLOQUES=200,MAX_ENEMIGOS=70,MAX_MONEDAS=70,MAX_IMAGEN_NIVELES_POSIBLES = 30;

const int MAX_JUGADORES = 4;
const int MAX_MENSAJE = 75;

typedef struct mensaje_log{
	char tipo;
	char mensajeParaElLog[MAX_MENSAJE];
}mensaje_log_t;

typedef struct enemigo{
	int posX;
	unsigned short numeroRecorte;
	unsigned short tipoEnemigo; // 1 GOOMBA - 2 KOOPA
	string direccionImagen;
}enemigo_t;

typedef struct bloque{
	int posX;
	int posY;
	string dirImagen;
}bloque_t;

typedef struct mario{
	unsigned short idImagen; //1 ROJO - 2 VERDE - 3 VIOLETA - 4 CELESTE
	int posX;
	int posY;
	int recorteImagen; // Si el recorte de la imagen viene en un valor (-1?) se indica que el jugador se desconecto y el recorte correspondiente es el gris
}mario_t;

typedef struct moneda{
	int posX;
	int posY;
	unsigned short numeroRecorte;
}moneda_t;

typedef struct jugador{
	string nombreJugador;
	int puntos;
	mario_t mario;
}jugador_t;


typedef struct usuario{
	string nombre;
	string contrasenia;
	bool usado;
}usuario_t;

typedef struct entrada_usuario{
	bool A;
	bool S;
	bool D;
	bool W;
}entrada_usuario_t;

typedef struct entrada_usuario_id{
	entrada_usuario_t entradas;
	unsigned short id;
}entrada_usuario_id_t;

typedef struct info_partida{

	jugador_t jugadores[MAX_JUGADORES];
	string direccionesFondoNiveles[MAX_IMAGEN_NIVELES_POSIBLES];

	//string direccionesImagenEnemigos[MAX_IMAGEN_ENEMIGOS];
	//string direccionesImagenBloques[MAX_IMAGEN_BLOQUE];

	bool iniciadoCorrectamente; // puede volar
	unsigned short cantidadJugadores;
	unsigned short cantidadFondosNiveles;
	unsigned short anchoVentana;
	unsigned short altoVentana;
	unsigned short idPropio;
}info_partida_t;

typedef struct ronda{
	unsigned short mundo;
	unsigned short posXCamara;
	unsigned short tiempoFaltante;
	unsigned short topeBloques;
	unsigned short topeEnemigos;
	unsigned short topeMonedas;
	bloque_t bloques[MAX_BLOQUES];
	enemigo_t enemigos[MAX_ENEMIGOS];
	moneda_t monedas[MAX_MONEDAS];
	jugador_t jugadores[MAX_JUGADORES];
}info_ronda_t;



#endif /* SRC_UTILS_HPP_ */
