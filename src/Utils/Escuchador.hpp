#ifndef SRC_UTILS_ESCUCHADOR_HPP_
#define SRC_UTILS_ESCUCHADOR_HPP_

#include "Utils.hpp"

class Escuchador{

	public:
		virtual ~Escuchador()= default;
		void escuchar();

		virtual void casoError(int resultado) = 0;
		virtual void casoSocketCerrado() = 0;
		virtual void casoExitoso() = 0;

	protected:
		int socket{};
		void* structPointer{};
		int bytes{};
};

#endif /* SRC_UTILS_ESCUCHADOR_HPP_ */
