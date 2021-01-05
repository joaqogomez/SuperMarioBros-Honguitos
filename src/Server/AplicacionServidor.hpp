#ifndef SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_
#define SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_

#include <SDL2/SDL.h>

#include <list>
#include <queue>
#include <map>

#include "../Utils/Contador.hpp"
#include "modelo/Camara/Camara.hpp"
class Servidor;
#include "Servidor.hpp"

#include "../Utils/log/Log.hpp"
#include "../Utils/Utils.hpp"

#include "modelo/Nivel.hpp"
#include "modelo/Moneda.hpp"

#include "modelo/Juego.hpp"
#include "lector/ArchivoLeido.hpp"

class AplicacionServidor{
	public:
		AplicacionServidor(Servidor* server,list<Nivel*> niveles,int cantidadJugadores,int ancho_pantalla ,int  alto_pantalla);
		~AplicacionServidor();

		void iniciarJuego();

		static void* gameLoop_helper(void* ptr){
			((AplicacionServidor*) ptr)->gameLoop();
			return NULL;
		}

		void desconectarJugador(int idJugador);

		void encolarEntradaUsuario(entrada_usuario_id_t entradaUsuario);
		info_partida_t obtenerInfoPartida(map<int,string> mapaIDNombre, int IDJugador);
		info_ronda_t obtenerInfoRonda(map<int,string> mapaIDNombre);
		void gameLoop();
		void activarJugador(int idMarioConectandose);
		bool empezoElJuego(){
			return comenzoElJuego;
		}

	private:
		Servidor* servidor;
		Log* log;
		Juego* juego;
		Camara* camara;
		int cantJugadores;
		bool terminoElJuego;
		bool comenzoElJuego;
		bool juegoInicializadoCorrectamente;
		queue<entrada_usuario_id_t> colaDeEntradasUsuario;
};



#endif /* SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_ */
