#ifndef SRC_SERVER_MODELO_ENEMIGOS_KOOPA_HPP_
#define SRC_SERVER_MODELO_ENEMIGOS_KOOPA_HPP_

using namespace std;

#include "Enemigo.hpp"
#include "../../sprites/SpriteKoopa.hpp"


class Koopa : public Enemigo{

	public:

		Koopa(int tipoColor){
			spriteEnemigo = new SpriteKoopa(tipoColor);
			this->tipoColor = tipoColor;
            velocidadX = obtenerVelocidad();
		}

		~Koopa(){
			delete spriteEnemigo;
			delete posicionActual;
		}

		enemigo_t serializar() override{
			enemigo_t enemigoSerializado;
			enemigoSerializado.posX = this->obtenerPosicionX();
			enemigoSerializado.numeroRecorteX = spriteEnemigo->obtenerEstadoActual();
			enemigoSerializado.numeroRecorteY = tipoColor;
			enemigoSerializado.tipoEnemigo = TIPO_KOOPA;
			enemigoSerializado.espejar = velocidadX > 0;
			return enemigoSerializado;
		}

		void actualizarPosicion()override{
			posicionActual->moverHorizontal(velocidadX);
			spriteEnemigo->actualizarSprite();
		}
};

#endif /* SRC_SERVER_MODELO_ENEMIGOS_KOOPA_HPP_ */
