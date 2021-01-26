#ifndef SRC_SERVER_MODELO_BLOQUES_BLOQUE_HPP_
#define SRC_SERVER_MODELO_BLOQUES_BLOQUE_HPP_

#include <list>
#include <string>
using namespace std;

#include "ObjetosSorpresa/ObjetoSorpresa.hpp"
#include "ObjetosSorpresa/SinSorpresa.hpp"

#include "src/Server/Modelo/Juego/Colisionable.hpp"
#include "src/Utils/Utils.hpp"
#include "src/Server/Modelo/Juego/Posiciones/PosicionFija.hpp"
#include "src/Server/Modelo/Juego/Sprites/SpriteBloque.hpp"

#define SORPRESA 0

class Bloque : public Colisionable{
    public:
		~Bloque() override = default;
		void actualizar();
		int obtenerPosicionX();
		int obtenerPosicionY();

        virtual ObjetoSorpresa* colisionaronAbajo();
        virtual entidad_t serializar();
        string obtenerColisionID() override = 0;
        rectangulo_t obtenerRectangulo() override;
        bool debeColisionar() override;
        bool cambioElSprite();
        virtual void elevar(int y);

    protected:
        int tipoBloque{};
    	Posicion posicion;
    	SpriteBloque* spriteBloque{};
		static int normalizarCoordenadaIngresada(int coordenada);
        void inicializarMapasDeColision() override = 0;
};

#endif /* SRC_SERVER_MODELO_BLOQUES_BLOQUE_HPP_ */