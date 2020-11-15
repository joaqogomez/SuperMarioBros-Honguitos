#include "mainServer.hpp"

#include <stdio.h>
#include <string>
#include <string.h>
using namespace std;

#include <SDL2/SDL.h>

#include "../src/app/App.hpp"
#include "../src/lector/Lector.hpp"

#include "../src/log/Log.hpp"
#include "../src/log/Error.hpp"
#include "../src/log/Debug.hpp"
#include "../src/log/Info.hpp"

#include "Servidor.hpp"

#include <getopt.h>

const int LARGO_ENTRADA = 150,LARGO_IP = 20;
const int ERROR = -1, VACIO=0, TERMINO = -1;
#define CONFIG 'c'
#define IP 'i'
#define LOG 'l'
#define PUERTO 'p'

void manejarEntrada(int cantidadArgumentos, char* argumentos[],char direccionLecturaComando[LARGO_ENTRADA],char nivelLogEntrada[LARGO_ENTRADA],
					char ipEntrada[LARGO_IP],char puertoEntrada[LARGO_IP],bool* esServer){

	  static struct option opcionesLargas[] = {
	     {"config", required_argument, 0, 'c'},
	     {"ip", required_argument, 0, 'i'},
	     {"log", required_argument, 0, 'l'},
	     {"port", required_argument, 0, 'p'},
	     {0, 0, 0, 0}
	  };

	  int argumento;
	  int indiceOpcion = 0;

	  while((argumento = getopt_long(cantidadArgumentos, argumentos, "c:i:p:l:",opcionesLargas, &indiceOpcion))!=TERMINO){
	      switch (argumento) {
	          case CONFIG:
	        	  strcpy(direccionLecturaComando,optarg);
	              break;
	          case IP:
	        	  strcpy(ipEntrada,optarg);
	              break;
	          case LOG:
	              strcpy(nivelLogEntrada,optarg);
	              break;
	          case PUERTO:
	        	  strcpy(puertoEntrada,optarg);
	              break;
	          default:
	              break;
	      }
	  }
}

TipoLog* determinarNivelLog(char nivelLogEntrada[LARGO_ENTRADA]){
	if(strcmp(nivelLogEntrada,"Error")==0 || strcmp(nivelLogEntrada,"ERROR")==0 || strcmp(nivelLogEntrada,"error")==0 ){
		return new Error();
	}
	else if(strcmp(nivelLogEntrada,"Debug")==0 || strcmp(nivelLogEntrada,"DEBUG")==0 || strcmp(nivelLogEntrada,"debug")==0 ){
		return new Debug();
	}
	else if(strcmp(nivelLogEntrada,"Info")==0 || strcmp(nivelLogEntrada,"INFO")==0 || strcmp(nivelLogEntrada,"info")==0 ){
		return new Info();
	}
	else{
		return NULL;
	}
}

ArchivoLeido* realizarConfiguracionesIniciales(char direccionLecturaComando[LARGO_ENTRADA], char nivelLogEntrada[LARGO_ENTRADA], list<string> &mensajesErrorOtroArchivo, TipoLog *nivelLog) {
	Lector* lector = new Lector();
	string direccionLecturaDefault = "resources/ArchivosXML/configuracionDefault.xml";
	ArchivoLeido* archivoLeido;
	if (strcmp(direccionLecturaComando, "") != 0) {
		archivoLeido = lector->leerArchivo(direccionLecturaComando);
		if (!archivoLeido->leidoCorrectamente) {
			mensajesErrorOtroArchivo = archivoLeido->mensajeError;
			delete archivoLeido;
			archivoLeido = lector->leerArchivo(direccionLecturaDefault);
		}
	} else {
		archivoLeido = lector->leerArchivo(direccionLecturaDefault);
	}

	if (strcmp(nivelLogEntrada, "") != 0) {
		nivelLog = determinarNivelLog(nivelLogEntrada);
		if (nivelLog != NULL) {
			archivoLeido->tipoLog = nivelLog;
		}
	}
	delete lector;

	return archivoLeido;
}

void gameLoop(const list<string> &mensajesErrorOtroArchivo, ArchivoLeido *archivoLeido) {
	App *aplicacion = App::getInstance(archivoLeido, mensajesErrorOtroArchivo);
	bool salir = false;
	SDL_Event event;
	while (!salir) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				salir = true;
			}
		}
		SDL_PumpEvents();
		const Uint8 *keyboard_state_array = SDL_GetKeyboardState(NULL);
		if (keyboard_state_array[SDL_SCANCODE_ESCAPE]) {
			salir = true;
		} else {
			aplicacion->actualizar(keyboard_state_array);
		}
		aplicacion->actualizar();
		aplicacion->dibujar();
	}
	delete aplicacion;
}

void abortarMainServidor(ArchivoLeido* archivoLeido, TipoLog* tipoLog, Servidor* servidor){
	delete archivoLeido;
	delete tipoLog;
	delete servidor;
}

int mainServer( int cantidadArgumentos, char* argumentos[] ){

	char direccionLecturaComando[LARGO_ENTRADA] = "";
	char nivelLogEntrada[LARGO_ENTRADA] = "";
	char ipEntrada[LARGO_IP] = "";
	char puertoEntrada[LARGO_IP] = "";
	ArchivoLeido* archivoLeido;
	TipoLog* nivelLog;
	list<string> mensajesErrorOtroArchivo;

	archivoLeido = realizarConfiguracionesIniciales(direccionLecturaComando, nivelLogEntrada, mensajesErrorOtroArchivo, nivelLog);

	int puerto = 5003;			// TODO: CLI.
	char ip[] = "127.0.0.1";	// TODO: CLI.

	Servidor* server = new Servidor(archivoLeido, mensajesErrorOtroArchivo, puerto, ip);

	pthread_t hilo;
	int resultadoCreate = pthread_create(&hilo, NULL, Servidor::escuchar_helper, server);
	if(resultadoCreate!= 0){
		Log::getInstance()->huboUnError("Ocurrió un error al crear el hilo para escuchar, el codigo de error es: " + to_string(resultadoCreate));
		abortarMainServidor(archivoLeido, nivelLog, server);
		return -1;
	}else{
		Log::getInstance()->mostrarMensajeDeInfo("Se creó el hilo para escuchar: (" + to_string(hilo) +").");
	}
	int resultadoJoin = pthread_join(hilo, NULL);
	if(resultadoJoin != 0){
		Log::getInstance()->huboUnError("Ocurrió un error al juntar los hilos main y escuchar, el codigo de error es: " + to_string(resultadoJoin));
		pthread_cancel(hilo);
		abortarMainServidor(archivoLeido, nivelLog, server);
		return -1;
	}else{
		Log::getInstance()->mostrarMensajeDeInfo("Se juntaron los hilos main y escuchar.");
	}


	abortarMainServidor(archivoLeido, nivelLog, server);
	return 0;
}
