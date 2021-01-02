
#ifndef SRC_MODELO_ENEMIGOS_GOOMBA_HP_
#define SRC_MODELO_ENEMIGOS_GOOMBA_HP_

using namespace std;

#include "Enemigo.hpp"
#include "../../sprites/SpriteGoomba.hpp"

class Goomba : public Enemigo{

	public:

		Goomba(int tipoColor){
			spriteEnemigo = new SpriteGoomba(tipoColor);
			this->tipoColor = tipoColor;
            velocidadX = obtenerVelocidad();
		}


		void actualizarPosicion()override{
			posicionActual->moverHorizontal(velocidadX);
			spriteEnemigo->actualizarSprite();
		}

		enemigo_t serializar() override{
			enemigo_t enemigoSerializado;
			enemigoSerializado.posX = this->obtenerPosicionX();
			enemigoSerializado.numeroRecorteX = spriteEnemigo->obtenerEstadoActual();
			enemigoSerializado.numeroRecorteY = tipoColor;
			enemigoSerializado.tipoEnemigo = TIPO_GOOMBA;
            enemigoSerializado.espejar = false;
			return enemigoSerializado;
		}


		~Goomba(){
			delete posicionActual;
			delete spriteEnemigo;
		}
};

#endif /* SRC_MODELO_ENEMIGOS_GOOMBA_HP_ */
