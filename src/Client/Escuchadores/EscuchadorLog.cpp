#include "EscuchadorLog.hpp"


EscuchadorLog::EscuchadorLog(int socket){
	this->socket = socket;
	strcpy(conjuntoMensajeLog.mensajeParaElLog,"");
	conjuntoMensajeLog.tipo = ' ';
	structPointer = &this->conjuntoMensajeLog;
	bytes = sizeof(mensaje_log_t);
}


void EscuchadorLog::casoError(int resultado){
	Log::getInstance()->huboUnErrorSDL("Ocurrio un error al recibir un mensaje del log", to_string(errno));
	throw runtime_error("ErrorAlEscucharUnMensajeParaElLog");
}

void EscuchadorLog::casoSocketCerrado(){
	Log::getInstance()->mostrarMensajeDeInfo("No se recibio mas informacion del mensaje para el Log. Se cierra el socket.");
	throw runtime_error("ErrorAlEscucharMensajeDelLog");
}

void EscuchadorLog::casoExitoso(){
	string mensajeLog(conjuntoMensajeLog.mensajeParaElLog);
	if(conjuntoMensajeLog.tipo == TIPO_ERROR){
		Log::getInstance()->huboUnError(mensajeLog);
	}
	else if(conjuntoMensajeLog.tipo == INFO){
		Log::getInstance()->mostrarMensajeDeInfo(mensajeLog);
	}
}
