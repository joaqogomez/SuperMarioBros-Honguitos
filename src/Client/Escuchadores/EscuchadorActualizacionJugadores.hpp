#ifndef SRC_CLIENT_ESCUCHADORES_ESCUCHADORACTUALIZACIONJUGADORES_HPP_
#define SRC_CLIENT_ESCUCHADORES_ESCUCHADORACTUALIZACIONJUGADORES_HPP_



class Cliente;
#include "../Cliente.hpp"

#include "Escuchador.hpp"

class EscuchadorActualizacionJugadores: public Escuchador{

	public:
		EscuchadorActualizacionJugadores(int socketCliente, Cliente* cliente);
		void escuchar() override;
		~EscuchadorActualizacionJugadores();
	private:
		int socket;
		Cliente* cliente;
};



#endif /* SRC_CLIENT_ESCUCHADORES_ESCUCHADORACTUALIZACIONJUGADORES_HPP_ */