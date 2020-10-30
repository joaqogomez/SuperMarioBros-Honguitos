#ifndef SRC_APP_DIBUJADORES_H_
#define SRC_APP_DIBUJADORES_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <sstream>

using namespace std;

#include "../../modelo/Juego.hpp"
#include "../CargadorTexturas.hpp"
#include "../../sprites/SpriteMario.hpp"

#include "DibujadorGameOver.hpp"
#include "DibujadorInicio.hpp"
#include "DibujadorGanadores.hpp"
#include "DibujadorJuego.hpp"

class Dibujadores{

	public:
		Dibujadores(CargadorTexturas* cargadorTexturas, SDL_Renderer* renderizador, int ancho_pantalla, int alto_pantalla);
		void dibujarJuego(SDL_Rect* rectanguloCamara);
		void dibujarGameOver();
		void dibujarInicio();
		void dibujarPantallaGanadores();
		~Dibujadores();

	private:
		DibujadorGameOver* dibujadorGameOver;
		DibujadorInicio* dibujadorInicio;
		DibujadorGanadores* dibujadorGanadores;
		DibujadorJuego* dibujadorJuego;
};



#endif /* SRC_APP_DIBUJADORES_H_ */