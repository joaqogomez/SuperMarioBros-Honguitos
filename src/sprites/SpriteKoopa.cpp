
#include "SpriteKoopa.h"

SpriteKoopa::SpriteKoopa(string direccionImagen){
	direccionTextura = direccionImagen;
	estadoActual = 0;
	ciclos=0;
	int corrimientoEnImagen=0;
	for(int i=0;i<6;i++){
		estadosPosibles[i].x = corrimientoEnImagen;
		estadosPosibles[i].y = 0;
		estadosPosibles[i].w = 16;
		estadosPosibles[i].h = 32;
		corrimientoEnImagen+= 16;
	}
}

void SpriteKoopa::actualizarSprite(){
	if(ciclos>=20){
		estadoActual = 1;
		ciclos = 0;
	}
	else if(ciclos>=10){
		estadoActual = 0;
	}
	ciclos++;
}


SDL_Rect SpriteKoopa::obtenerRectanguloActual(){
	return estadosPosibles[estadoActual];
}