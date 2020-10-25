#include "ParserVentana.hpp"

#define VALOR_POR_DEFECTO_ANCHO 800
#define VALOR_POR_DEFECTO_ALTO 600

void ParserVentana::ParsearVentana(pugi::xml_node ventana,ArchivoLeido* archivoLeido){
	string ancho = ventana.child_value("ancho");
	string alto = ventana.child_value("alto");
	try{
		if(stoi(ancho) >= 0){
			archivoLeido->anchoVentana = stoi(ancho);
		}else{
			archivoLeido->leidoCorrectamente = false;
			archivoLeido->mensajeError.push_back("El valor de ancho enviado no tiene valor valido,se carga el valor por defecto");
			archivoLeido->anchoVentana = VALOR_POR_DEFECTO_ANCHO;
		}
	}catch(const std::invalid_argument& error){
		archivoLeido->leidoCorrectamente = false;
		archivoLeido->mensajeError.push_back("El valor de ancho enviado no tiene valor valido,se carga el valor por defecto");
		archivoLeido->anchoVentana = VALOR_POR_DEFECTO_ANCHO;
	}
	try{
		if(stoi(alto) >= 0){
			archivoLeido->altoVentana = stoi(alto);
		}else{
			archivoLeido->leidoCorrectamente = false;
			archivoLeido->mensajeError.push_back("El valor de alto enviado no tiene valor valido,se carga el valor por defecto");
			archivoLeido->altoVentana = VALOR_POR_DEFECTO_ALTO;
		}
	}catch(const std::invalid_argument& error){
		archivoLeido->leidoCorrectamente = false;
		archivoLeido->mensajeError.push_back("El valor de alto enviado no tiene valor valido,se carga el valor por defecto");
		archivoLeido->altoVentana = VALOR_POR_DEFECTO_ALTO;
	}
}

