#ifndef SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_
#define SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_

#include <SDL2/SDL.h>

#include <list>
#include <queue>

#include "../src/log/Log.hpp"
#include "../src/Utils.hpp"

#include "../src/lector/ArchivoLeido.hpp"
#include "../src/modelo/Nivel.hpp"
#include "../src/modelo/Juego.hpp"

class AplicacionServidor{
	public:
		AplicacionServidor(list<Nivel*> niveles,int cantidadJugadores,int ancho_pantalla ,int  alto_pantalla);
		~AplicacionServidor();

		void iniciarJuego();

		static void* gameLoop_helper(void* ptr){
			((AplicacionServidor*) ptr)->gameLoop();
			return NULL;
		}

		void encolarEntradaUsuario(entrada_usuario_id_t entradaUsuario);

		void gameLoop();

	private:
		void revisarSiTerminoNivel(map<int,Mario*> jugadores);
		void actualizarPosicionDeJugador(Mario* jugador,entrada_usuario_t entrada);
		SDL_Rect* obtenerRectCamara();
		void moverCamara(map<int,Mario*> jugadores);
		Log* log;
		Juego* juego;
		SDL_Rect rectanguloCamara;
		int tiempoDeInicio;
		int tiempoFaltante;
		int cantJugadores;
		int ancho_pantalla;
		bool terminoElJuego;
		bool comenzoElJuego;
		bool ganaron;
		bool juegoInicializadoCorrectamente;
		queue<entrada_usuario_id_t> colaDeEntradasUsuario;
};



#endif /* SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_ */
