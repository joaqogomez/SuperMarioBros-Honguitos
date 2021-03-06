#include "src/Utils/log/Log.hpp"
#include "EscuchadorConexionCliente.hpp"
#include "EscuchadorCredenciales.hpp"
#include "src/Utils/Constantes.hpp"
#include "EscuchadorEntradaTeclado.hpp"

EscuchadorConexionCliente::EscuchadorConexionCliente(Socket* socket, ConexionCliente* cliente) {
    escuchadores[CREDENCIAL] = new EscuchadorCredenciales(socket,cliente);
    this->socket = socket;
    this->cliente = cliente;
    idJugador = SIN_JUGAR;
}

EscuchadorConexionCliente::~EscuchadorConexionCliente() {
    for(auto const& parClaveEscuchador:escuchadores){
        delete parClaveEscuchador.second;
    }
    escuchadores.clear();
}

void EscuchadorConexionCliente::ejecutar() {
    char tipoMensaje;
    int resultado;
    bool hayError = false;
    while(!cliente->terminoElJuego() && !hayError){
        resultado = socket->escuchar(&tipoMensaje, sizeof(char));

        if(resultado<0){
            Log::getInstance()->huboUnErrorSDL("Ocurrio un error escuchando el caracter identificatorio del mensaje en el cliente: " + cliente->obtenerIP(), std::to_string(errno));
            hayError = true;
        }else if(resultado == 0){
            Log::getInstance()->mostrarMensajeDeInfo("Se desconecto el socket que escucha al cliente: " + cliente->obtenerIP() + " ---- "+ std::to_string(errno));
            hayError = true;
        }else{
            try{
                escuchadores[tipoMensaje]->escuchar();
            }catch(const std::exception& e){
                hayError = true;
            }
        }
    }
    cliente->desconectarse();
    cliente->terminarElJuego();
}

void EscuchadorConexionCliente::agregarEscuchadorEntrada(int IDjugador,Servidor* servidor) {
    escuchadores[ENTRADA] = new EscuchadorEntradaTeclado(socket,IDjugador,servidor);
    idJugador = IDjugador;
}
