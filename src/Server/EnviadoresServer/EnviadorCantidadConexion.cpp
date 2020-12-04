
#include "EnviadorCantidadConexion.hpp"

EnviadorCantidadConexion::EnviadorCantidadConexion(int socket){
	this->socket = socket;
}

void EnviadorCantidadConexion::enviar(){
	char caracterMensaje = ACTUALIZACION_JUGADORES;
	send(socket, &caracterMensaje, sizeof(char), 0);
	send(socket, &cant_jugadores_activos, sizeof(actualizacion_cantidad_jugadores_t), 0);
}

void EnviadorCantidadConexion::dejarInformacion(void* cantJugadoresActivos){
	cant_jugadores_activos = *((actualizacion_cantidad_jugadores_t*) cantJugadoresActivos);
}

