#ifndef SRC_SERVER_SERVIDOR_HPP_
#define SRC_SERVER_SERVIDOR_HPP_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>

#include <thread>
#include <map>

#include "../Utils/log/Log.hpp"
#include "modelo/Juego.hpp"

#include "AplicacionServidor.hpp"
#include "../Utils/Utils.hpp"
#include "UtilidadesServer.hpp"
#include "lector/ArchivoLeido.hpp"

#include "ConexionCliente.hpp"
#include "ManejadorIdentificadores.hpp"
#include "src/Utils/Constantes.hpp"
#include "AceptadorDeConexiones.hpp"
#include "ReconectadorDeConexiones.hpp"

class Servidor : public Thread{

	public:
		Servidor(const ArchivoLeido& archivoLeido,const list<string>& mensajesErrorOtroArchivo, int puerto, char* ip);
		~Servidor() override;

		bool esUsuarioValido(const usuario_t& posibleUsuario,ConexionCliente* conexionClienteConPosibleUsuario);
		void intentarIniciarModelo();
		void encolarEntradaUsuario(entrada_usuario_id_t entradaUsuario);
		void agregarUsuarioDesconectado(ConexionCliente* conexionPerdida,int idJugador,string nombre,const string& contrasenia);
		void ejecutar() override;
		void terminarElJuego();
		map<int,string> obtenerMapaJugadores();

		actualizacion_cantidad_jugadores_t crearActualizacionJugadores();

        void mandarNivelAClientes(nivel_t nivel);
        void guardarRondaParaEnvio(info_ronda_t ronda);
        void enviarSonidosA(int id, const list<sonido_t>& sonidos);
        bool terminoElJuego() const;
        void guardarConexion(ConexionCliente *conexionCliente);

        bool empezoElJuego();
        void mandarActualizacionAClientes();
        void reconectarJugador(mensaje_log_t mensaje, int idJugador);


    private:
        ReconectadorDeConexiones reconectador;
        AceptadorDeConexiones aceptadorDeConexiones = AceptadorDeConexiones(nullptr, 0);
		map<int,string> mapaIDNombre;
		Log* log;
		AplicacionServidor aplicacionServidor;
		ManejadorIdentificadores manejadorIDs;

		int socketServer;
		int cantidadConexiones;
		int cantUsuariosLogueados = 0;

		list<usuario_t> usuariosValidos;

		bool esUsuarioDesconectado(const usuario_t& posibleUsuario,ConexionCliente* conexionClienteConPosibleUsuario);
		bool esUsuarioSinConectarse(const usuario_t& posibleUsuario,ConexionCliente* conexionClienteConPosibleUsuario);
        void notificarClientesDeLaDesconexion(const ConexionCliente *conexionPerdida, string &nombre);

        bool terminoJuego;
		list<ConexionCliente*> clientes;
		list<ConexionCliente*> conexionesPerdidas;
		map<int,ConexionCliente*> clientesJugando;
};

#endif /* SRC_SERVER_SERVIDOR_HPP_ */