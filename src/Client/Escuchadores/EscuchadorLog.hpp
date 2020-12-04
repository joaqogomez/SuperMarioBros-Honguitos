#ifndef SRC_CLIENT_ESCUCHADORES_ESCUCHADORLOG_HPP_
#define SRC_CLIENT_ESCUCHADORES_ESCUCHADORLOG_HPP_
#include <thread>

#include <string>
using namespace std;

#include "../../Utils/log/Log.hpp"

#include "../../Utils/Escuchador.hpp"

class EscuchadorLog : public Escuchador{

	public:

		EscuchadorLog(int socket);

		void escuchar();
		void casoError(int resultado)override;
		void casoSocketCerrado()override;
		void casoExitoso()override;

	private:
		mensaje_log_t conjuntoMensajeLog;
};



#endif /* SRC_CLIENT_ESCUCHADORES_ESCUCHADORLOG_HPP_ */
