#ifndef SERVER_CONEXIONCLIENTE_HPP_
#define SERVER_CONEXIONCLIENTE_HPP_

class Servidor;
#include "Servidor.hpp"

using namespace std;
#include <thread>
#include <string>

#include "../src/log/Log.hpp"

class ConexionCliente {

	public:
		ConexionCliente(Servidor* servidor, int socket, string nombre);
		~ConexionCliente();

		void recibir();
		void ejecutar();
		static void* ejecutar_helper(void* ptr){
			((ConexionCliente*) ptr)->ejecutar();
			return NULL;
		}

		void enviar(char* msg);

	private:
		string nombre;
		string contrasenia;
		int socket;
		Servidor* servidor;
		void recibirCredenciales();
};


#endif /* SERVER_CONEXIONCLIENTE_HPP_ */