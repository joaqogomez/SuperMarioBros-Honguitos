#ifndef SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_
#define SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_

#include <SDL2/SDL.h>

#include <list>
#include <queue>
#include <map>

#include "Juego/Juego.hpp"
#include "Juego/Nivel/Nivel.hpp"

class Servidor;
#include "src/Utils/Contador.hpp"
#include "src/Utils/log/Log.hpp"
#include "src/Utils/Utils.hpp"
#include "src/Utils/Thread.hpp"

class AplicacionServidor : public Thread{
	public:
		AplicacionServidor(Servidor* server,list<Nivel*> niveles,int cantidadJugadores,int ancho_pantalla ,int  alto_pantalla);
		~AplicacionServidor() override;
		void iniciarJuego();
		void desconectarJugador(int idJugador);
        nivel_t obtenerInfoNivel();
		void encolarEntradaUsuario(entrada_usuario_id_t entradaUsuario);
		info_partida_t obtenerInfoPartida(map<int,string> mapaIDNombre, int IDJugador);
		info_ronda_t obtenerInfoRonda(map<int,string> mapaIDNombre);
		void ejecutar() override;
		void activarJugador(int idMarioConectandose);
		bool empezoElJuego() const;

	private:
        void revisarSiMandarInfoNivel(int *cantidadNivelesRestantes);
        void mandarInfoNivel();
        void enviarSonidos();

		Servidor* servidor;
		Log* log;
		Juego* juego;
        bool terminoElJuego;
		bool comenzoElJuego;
        queue<entrada_usuario_id_t> colaDeEntradasUsuario;
};



#endif /* SRC_SERVER_APLICACIONSERVIDOR_APLICACIONSERVIDOR_HPP_ */