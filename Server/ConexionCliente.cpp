#include "ConexionCliente.hpp"

ConexionCliente::ConexionCliente(Servidor* servidor, int socket, string nombre){
	this->servidor = servidor;
	this->socket = socket;
	this->nombre = nombre;
}


int Read4Bytes(int socket,  char* buffer){
    int bytesRead = 0;
    int result;
    memset(buffer, 0, 5);
    while (bytesRead < 4){
        result = recv(socket, buffer, 4, MSG_DONTWAIT);
        if (result < 1 ){
            return bytesRead;
        }
        bytesRead += result;
    }
    return bytesRead;
}

void ConexionCliente::recibir(){
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	while(true){
		string mensaje;
		char buffer[5] = "";
		while(Read4Bytes(socket, buffer) > 0){
			if(!(strlen(buffer) == 0)){
				mensaje = mensaje + (string) buffer;
			}
		}
		if(mensaje.length() != 0){
			pthread_mutex_lock(&mutex);
			Log::getInstance()->mostrarMensajeDeInfo("Se escribió el mensaje: \"" + mensaje +"\" del cliente: " + nombre);
			pthread_mutex_unlock(&mutex);
		}
	}
}

void ConexionCliente::enviar(char* msg){}


void ConexionCliente::recibirCredenciales(){
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	bool tengoCredenciales = false;
	string credenciales;
	while(!tengoCredenciales){
		char buffer[5] = "";
		while(Read4Bytes(socket, buffer) > 0){
			if(!(strlen(buffer) == 0)){
				credenciales = credenciales + (string) buffer;
				tengoCredenciales = true;
			}
		}
		if(credenciales.length() != 0){
			pthread_mutex_lock(&mutex);
			Log::getInstance()->mostrarMensajeDeInfo("Se escribió el mensaje: \"" + credenciales +"\" del cliente: " + nombre);
			pthread_mutex_unlock(&mutex);
		}
	}

	string limite = ";";
	size_t posLimite = credenciales.find(limite);
	nombre = credenciales.substr(0, posLimite);
	contrasenia = credenciales.substr(posLimite+1, credenciales.length());


}

void ConexionCliente::ejecutar(){
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	bool usuarioValido = false;
	while(!usuarioValido){
		recibirCredenciales();
		usuarioValido = servidor->esUsuarioValido({nombre,contrasenia});

		send(socket, &usuarioValido, sizeof(usuarioValido), 0);
		if(usuarioValido){
			pthread_mutex_lock(&mutex);
			Log::getInstance()->mostrarMensajeDeInfo("Se acepto el usuario: "+nombre+" con contrasenia: "+contrasenia);
			pthread_mutex_unlock(&mutex);
		}
	}

	//escuchar para teclas

}

ConexionCliente::~ConexionCliente(){
	close(socket);
}