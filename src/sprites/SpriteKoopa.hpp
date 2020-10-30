#ifndef SRC_SPRITES_SPRITEKOOPA_HPP_
#define SRC_SPRITES_SPRITEKOOPA_HPP_

#include <SDL2/SDL.h>
#include "Sprite.hpp"

class SpriteKoopa : public Sprite{
	public:

		SpriteKoopa(string direccionImagen);
		~SpriteKoopa(){};

		SDL_Rect obtenerRectanguloActual()override;

		void actualizarSprite()override;

	private:
		int ciclos;
		int estadoActual;
		SDL_Rect estadosPosibles[6];
};

#endif /* SRC_SPRITES_SPRITEKOOPA_HPP_ */