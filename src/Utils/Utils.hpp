#ifndef SRC_UTILS_HPP_
#define SRC_UTILS_HPP_

using namespace std;
#include <string>

/* PALABRAS RESERVADAS */
/*
 * I: Inicio
 * V: Verificacion
 * U: Actualizacion sobre la cantidad de jugadores.
 * L: Mensaje para el log.
 * E: Entrada del usuario
 * C: Credenciales que nos manda el usuario
 * P: Informacion necesaria para iniciar la partida
 */


const int MAX_NOMBRE = 20,MAX_CONTRASENIA = 25;

const int MAX_CANT_NIVELES = 10,MAX_LARGO_NOMBRE_NIVEL= 30; // Solo el nombre, nosotros concatenamos la direccion
															// correspondiente a la carpeta en la que tiene que estar esta imagen

const int MAX_IMAGEN_ENEMIGOS = 30,MAX_IMAGEN_BLOQUE = 30;
const int MAX_BLOQUES=200,MAX_ENEMIGOS=70,MAX_MONEDAS=70,MAX_IMAGEN_NIVELES_POSIBLES = 30;

const int MAX_JUGADORES = 4;
const int MAX_MENSAJE = 75;


#define TIPO_ERROR 'E'
#define INFO 'I'
#define MENSAJE_LOG 'L'
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
	unsigned short posX;
	unsigned short posY;
	short recorteImagen; // Si el recorte de la imagen viene en un valor (-1?) se indica que el jugador se desconecto y
						//el recorte correspondiente es el gris (DE 0 A 14/18? SON LOS ESTADOS)
}mario_t;

typedef struct moneda{
	unsigned short posX;
	unsigned short posY;
	unsigned short numeroRecorte;
}moneda_t;

typedef struct jugador{
	char nombreJugador[MAX_NOMBRE];
	int puntos;
	mario_t mario;
}jugador_t;


#define CREDENCIAL 'C'
typedef struct credencial{
	char nombre[MAX_NOMBRE];
	char contrasenia[MAX_CONTRASENIA];
}credencial_t;

typedef struct usuario{
	string nombre;
	string contrasenia;
	bool usado;
}usuario_t;

#define ENTRADA 'E'
typedef struct entrada_usuario{
	bool A;
	bool S;
	bool D;
	bool W;
}entrada_usuario_t;


#define INICIO 'I'
typedef struct info_inicio{
	unsigned short cantidadJugadores;
	unsigned short cantidadJugadoresActivos;
}info_inicio_t;

#define VERIFICACION 'V'
typedef bool verificacion_t;

typedef struct entrada_usuario_id{
	entrada_usuario_t entradas;
	unsigned short id;
}entrada_usuario_id_t;

#define PARTIDA 'P'
typedef struct info_partida{//~438 bytes enviados al inicio
	jugador_t jugadores[MAX_JUGADORES];
	unsigned short cantidadJugadores;
	char direccionesFondoNiveles[MAX_CANT_NIVELES][MAX_LARGO_NOMBRE_NIVEL];

	unsigned short cantidadFondosNiveles;
	unsigned short anchoVentana;
	unsigned short altoVentana;
	unsigned short idPropio;
}info_partida_t;

#define ACTUALIZACION_JUGADORES 'U'
typedef struct actualizacion_cantidad_jugadores{
	unsigned short cantidadJugadoresActivos;
}actualizacion_cantidad_jugadores_t;


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