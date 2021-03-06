#ifndef SRC_SERVER_SERVIDOR_HPP_
#define SRC_SERVER_SERVIDOR_HPP_

#include <cstdio>
#include <cstdlib>
#include <string>
#include <list>
#include <map>
#include "src/Utils/log/Log.hpp"
#include "src/Server/Modelo/AplicacionServidor.hpp"
#include "src/Server/Lector/ArchivoLeido.hpp"
#include "src/Utils/Utils.hpp"
#include "ConexionCliente.hpp"
#include "ReconectadorDeConexiones.hpp"
#include "IntentadorIniciarModelo.hpp"
#include "AceptadorDeConexiones.hpp"
#include "ManejadorIdentificadores.hpp"


class Servidor : public Thread{

	public:
		Servidor(const ArchivoLeido& archivoLeido,const std::list<std::string>& mensajesErrorOtroArchivo, int puerto, char* ip);
		~Servidor() override;

		bool esUsuarioValido(const usuario_t& posibleUsuario,ConexionCliente* conexionClienteConPosibleUsuario);
		void encolarEntradaUsuario(entrada_usuario_id_t entradaUsuario);
		void agregarUsuarioDesconectado(ConexionCliente* conexionPerdida,int idJugador,std::string nombre,const std::string& contrasenia);
		void ejecutar() override;
		void terminarElJuego();
        std::map<int,std::string> obtenerMapaJugadores();

		actualizacion_cantidad_jugadores_t crearActualizacionJugadores();

        void mandarNivelAClientes(nivel_t nivel);
        void mandarPodiosAClientes(ultimos_podios_t ultimos_podios);
        void guardarRondaParaEnvio(info_ronda_t ronda);
        void enviarSonidosA(int id, const std::list<sonido_t>& sonidos);
        bool terminoElJuego() const;
        void guardarConexion(ConexionCliente *conexionCliente);

        bool empezoElJuego();
        void mandarActualizacionAClientes();
        void reconectarJugador(mensaje_log_t mensaje, int idJugador);


    private:
        Socket socketServer;
        ReconectadorDeConexiones reconectador;
        IntentadorIniciarModelo iniciadorModelo;
        AceptadorDeConexiones aceptadorDeConexiones;
		AplicacionServidor aplicacionServidor;
		ManejadorIdentificadores manejadorIDs;

		Log* log;

        bool terminoJuego;
        int cantidadConexiones;
		int cantUsuariosLogueados = 0;

		bool esUsuarioDesconectado(const usuario_t& posibleUsuario,ConexionCliente* conexionClienteConPosibleUsuario);
		bool esUsuarioSinConectarse(const usuario_t& posibleUsuario,ConexionCliente* conexionClienteConPosibleUsuario);
        void notificarClientesDeLaDesconexion(const ConexionCliente *conexionPerdida, std::string &nombre);
        void eliminarConexionesPerdidas();

        bool seEnvioUltimoPodio;

        std::list<ConexionCliente*> clientes;
        std::list<ConexionCliente*> conexionesPerdidas;
        std::map<int,ConexionCliente*> clientesJugando;
        std::list<usuario_t> usuariosValidos;
        std::map<int,std::string> mapaIDNombre;
};

#endif /* SRC_SERVER_SERVIDOR_HPP_ */