#include "Cliente.hpp"

#include "../Utils/log/Log.hpp"
#include <thread>

#include "app/ManejadorSDL.hpp"

#include "UtilidadesCliente.hpp"

#include "EnviadoresCliente/EnviadorEntrada.hpp"
#include "EnviadoresCliente/EnviadorCredenciales.hpp"

Cliente::Cliente(char ip[LARGO_IP], int puerto){
	socketCliente = conectarAlServidor(ip, puerto);

	terminoJuego = false;
	empiezaElJuego = false;
	pasoVerificacion = false;
	seRecibioVerificacion = false;
	seRecibioInformacionInicio = false;
	cargoLaAplicacion = false;
	terminoEnviar = false;
	terminoEscuchar = false;
	cerroVentana = false;

	cantidadJugadoresActivos = 0;
	escuchadores[VERIFICACION] = new EscuchadorVerificacionCredenciales(socketCliente, this);
	escuchadores[ACTUALIZACION_JUGADORES] = new EscuchadorActualizacionJugadores(socketCliente, this);
	escuchadores[MENSAJE_LOG] = new EscuchadorLog(socketCliente);
	escuchadores[PARTIDA] = new EscuchadorInfoPartidaInicial(socketCliente,this);
	escuchadores[RONDA] = new EscuchadorRonda(socketCliente, this);

	enviadores[CREDENCIAL] = new EnviadorCredenciales(socketCliente);
	enviadores[ENTRADA] = new EnviadorEntrada(socketCliente);

	ventanaInicio = nullptr;
	gameLoop = new GameLoop();
}

void Cliente::escuchar(){
	char tipoMensaje;
	int resultado;
	bool hayError = false;

	while(!hayError && !terminoJuego){
		resultado = recv(socketCliente, &tipoMensaje, sizeof(char), MSG_WAITALL);

		if(resultado<0){
			Log::getInstance()->huboUnErrorSDL("Ocurrio un error escuchando el caracter identificatorio del mensaje",to_string(errno));
			hayError = true;
		}else if(resultado == 0){
			Log::getInstance()->mostrarMensajeDeInfo("Se desconecto el socket que escucha al server. ----- " +to_string(errno));
			hayError = true;
		}
		else{
			try{
				escuchadores[tipoMensaje]->escuchar();
			}catch(const std::exception& e){
				hayError = true;
			}
		}
	}
	terminoEscuchar = true;
	terminarProcesosDelCliente();
}

void Cliente::terminarProcesosDelCliente() {
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);
	terminoJuego = true;
	//cerroVentana = true;
	gameLoop->seMurioElServer();
	if(ventanaInicio!=nullptr){
		ventanaInicio->seMurioElServer();
	}
	pthread_mutex_unlock(&mutex);
}

void Cliente::enviar(){
	char tipoMensaje;
	bool hayError = false;
	while(!terminoJuego && !hayError){
		if(!identificadoresMensajeAEnviar.empty()){
			tipoMensaje = identificadoresMensajeAEnviar.front();
			identificadoresMensajeAEnviar.pop();
			try{
				enviadores[tipoMensaje]->enviar();
			}catch(const std::exception& e){
				hayError = true;
			}
		}
	}
	terminoEnviar = true;
	terminarProcesosDelCliente();
}

void Cliente::empezarJuego(info_partida_t info_partida){
	empiezaElJuego = true;
	infoPartida = info_partida;
}

void Cliente::recibirVerificacionCredenciales(verificacion_t verificacion){
	this->pasoVerificacion = verificacion;
	this->seRecibioVerificacion = true;
}

void Cliente::recibirInformacionActualizacion(actualizacion_cantidad_jugadores_t actualizacion){
	if(!seRecibioInformacionInicio){
		pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
		iniciarSDL();
		ventanaInicio = new VentanaInicio(actualizacion.cantidadJugadoresActivos, actualizacion.cantidadMaximaDeJugadores);
		pthread_mutex_lock(&mutex);
		cantidadJugadoresActivos = actualizacion.cantidadJugadoresActivos;
		ventanaInicio->actualizarJugadores(actualizacion);
		this->seRecibioInformacionInicio = true;
		pthread_mutex_unlock(&mutex);
	}else if(!empiezaElJuego){
		cantidadJugadoresActivos = actualizacion.cantidadJugadoresActivos;
		ventanaInicio->actualizarJugadores(actualizacion);
	}
}
void Cliente::recibirInformacionRonda(info_ronda_t info_ronda){
	if(!cargoLaAplicacion){
		return;
	}
	App* aplicacion = App::getInstance();
	aplicacion->agregarRonda(info_ronda);
}

void Cliente::esperarRecibirInformacionInicio(){
	while(!seRecibioInformacionInicio){
	}
}

void Cliente::esperarRecibirVerificacion(){
	while(!seRecibioVerificacion){
	}
}

void Cliente::esperarAQueEmpieceElJuego() {
	while (!cerroVentana && !empiezaElJuego) {
		try {
			ventanaInicio->imprimirMensajeEspera();
		} catch (const std::exception &e) {
			cerroVentana = true;
		}
	}
}

void Cliente::intentarEntrarAlJuego() {
	while (!pasoVerificacion && !cerroVentana) {
		try {
			ventanaInicio->obtenerEntrada();
			enviarCredenciales(ventanaInicio->obtenerCredenciales());
			esperarRecibirVerificacion();
			if (!pasoVerificacion) {
				ventanaInicio->imprimirMensajeError();
				seRecibioVerificacion = false;
			}
		} catch (const std::exception &e) {
			cerroVentana = true;
		}
	}
}

void Cliente::ejecutar(){
	pthread_t hiloEscuchar;
	int resultadoCreateEscuchar = pthread_create(&hiloEscuchar, NULL, Cliente::escuchar_helper, this);
	if(resultadoCreateEscuchar != 0){
		Log::getInstance()->huboUnError("Ocurrió un error al crear el hilo para escuchar la informacion del server.");
		return;
	}

	pthread_t hiloEnviar;
	int resultadoCreateEnviar = pthread_create(&hiloEnviar, NULL, Cliente::enviar_helper, this);
	if(resultadoCreateEnviar != 0){
		Log::getInstance()->huboUnError("Ocurrió un error al crear el hilo para enviar la informacion del cliente al server.");
		terminoJuego = true;
		return;
	}

	esperarRecibirInformacionInicio();
	intentarEntrarAlJuego();
	if(cerroVentana){
		delete ventanaInicio;
		ventanaInicio = nullptr;
		cerradoVentanaInicio();
		while(!terminoEnviar || !terminoEscuchar){}
		delete Log::getInstance();
		exit(0);
	}

	esperarAQueEmpieceElJuego();
	delete ventanaInicio;
	ventanaInicio = nullptr;
	if(cerroVentana){
		cerradoVentanaInicio();
		while(!terminoEnviar || !terminoEscuchar){}
		delete Log::getInstance();
		exit(0);
	}

	cargoLaAplicacion = gameLoop->inicializarAplicacion(infoPartida, this);
	if(!cargoLaAplicacion){
		Log::getInstance()->huboUnError("No se inicializo la aplicacion");
		cerrarSocketCliente(socketCliente);
		while(!terminoEnviar || !terminoEscuchar){}
		delete Log::getInstance();
		exit(-1);
	}

	gameLoop->gameLoop();

	terminoJuego = true;
}

/////------------------ENVIADORES------------------/////

void Cliente::agregarEntrada(entrada_usuario_t entradaUsuario){
	enviadores[ENTRADA]->dejarInformacion(&entradaUsuario);
	identificadoresMensajeAEnviar.push(ENTRADA);
}

void Cliente::enviarCredenciales(credencial_t credenciales){
	enviadores[CREDENCIAL]->dejarInformacion(&credenciales);
	identificadoresMensajeAEnviar.push(CREDENCIAL);
}

/////------------------DESTRUCTOR------------------/////

void Cliente::cerradoVentanaInicio() {
	Log::getInstance()->mostrarMensajeDeInfo("Se cerro la ventana de inicio");
	cerrarSocketCliente(socketCliente);
}

Cliente::~Cliente(){
	terminarSDL();
	cerrarSocketCliente(socketCliente);
	while(!terminoEnviar || !terminoEscuchar){}

	for(auto const& parClaveEscuchador:escuchadores){
		delete parClaveEscuchador.second;
	}
	for(auto const& parClaveEnviador:enviadores){
		delete parClaveEnviador.second;
	}
	escuchadores.clear();
	enviadores.clear();

	delete gameLoop;

}
