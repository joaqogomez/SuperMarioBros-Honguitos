#ifndef SRC_CLIENT_ESCUCHADORES_ESCUCHADORACTUALIZACIONJUGADORES_HPP_
#define SRC_CLIENT_ESCUCHADORES_ESCUCHADORACTUALIZACIONJUGADORES_HPP_

class Cliente;
#include "../Cliente.hpp"

#include "../../Utils/Escuchador.hpp"

class EscuchadorActualizacionJugadores: public Escuchador{

	public:
		EscuchadorActualizacionJugadores(int socketCliente, Cliente* cliente);
		~EscuchadorActualizacionJugadores();
		void escuchar();
		void casoError(int resultado)override;
		void casoSocketCerrado()override;
		void casoExitoso()override;

		bool idsValidas(par_id_nombre_t pares_id_nombre[MAX_JUGADORES], int tope);
		bool recibioMensajeValido();

	private:
		actualizacion_cantidad_jugadores_t actualizacion;
		Cliente* cliente;
		string error;
};

#endif /* SRC_CLIENT_ESCUCHADORES_ESCUCHADORACTUALIZACIONJUGADORES_HPP_ */
