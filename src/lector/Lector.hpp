#ifndef SRC_LECTOR_LECTOR_HPP_
#define SRC_LECTOR_LECTOR_HPP_

using namespace std;
#include <string>
#include "ArchivoLeido.hpp"

class Lector{
	public:
		ArchivoLeido* leerArchivo(string nombreArchivo);
};




#endif /* SRC_LECTOR_LECTOR_HPP_ */
