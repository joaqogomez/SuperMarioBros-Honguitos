#ifndef SRC_SERVER_CONEXIONCLIENTE_HPP_
#define SRC_SERVER_CONEXIONCLIENTE_HPP_

class Servidor;
#include "Servidor.hpp"


using namespace std;
#include <thread>
#include <string>
#include <map>
#include "EscuchadoresServer/EscuchadorServer.hpp"

#include "../Utils/Utils.hpp"

#include "../Utils/log/Log.hpp"

class EscuchadorEntradaTeclado;
#include "EscuchadoresServer/EscuchadorEntradaTeclado.hpp"

class ConexionCliente {

	public:
		ConexionCliente(Servidor* servidor, int socket, int cantidadConexiones,string ip);
		~ConexionCliente();

		void escuchar();
		static void* escuchar_helper(void* ptr){
			((ConexionCliente*)ptr)->escuchar();
			return NULL;
		}

		void ejecutar();
		static void* ejecutar_helper(void* ptr){
			((ConexionCliente*) ptr)->ejecutar();
			return NULL;
		}
		void actualizarCantidadConexiones(int cantidadConexiones);
		void recibirCredencial(string nombre,string contrasenia);
		void agregarIDJuego(int IDJugador);
		void enviarInfoPartida(info_partida_t info_partida);
		string obtenerIP(){
			return ip;
		}


	private:
		info_inicio_t crearInformacionInicio();
		void enviarInformacionInicio();
		void enviarVerificacion(bool esUsuarioValido);
		void enviarActualizacionCantidadConexiones();
		string nombre;
		string contrasenia;
		bool puedeJugar = false;
		int idPropio;
		int socket;
		Servidor* servidor;
		int cantidadConexiones;
		EscuchadorEntradaTeclado* escuchadorEntradaTeclado;
		string ip;

		map<char,EscuchadorServer*> escuchadores;
		void esperarCredenciales();
		bool recibioCredenciales = false;

};


#endif /* SRC_SERVER_CONEXIONCLIENTE_HPP_ */