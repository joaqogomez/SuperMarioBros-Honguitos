#include "SpriteMario.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <sstream>



const int ANCHO_IMAGEN_PIXEL = 405;
const int ALTO_IMAGEN_PIXEL = 32;
const int ANCHO_SPRITE_PIXEL = 16;
const int ALTO_SPRITE_PIXEL = 32;
const int PASO_PROXIMO_SPRITE = 30;
const int CANT_DE_EVENTOS_MAX = 10;

const int AGACHADO_IZQ = 0, AGACHADO_DER = 13;
const int QUIETO_IZQ = 6, QUIETO_DER = 7;
const int SALTANDO_IZQ = 1, SALTANDO_DER = 12;
const int CAMBIO_BRUSCO_IZQ = 2, CAMBIO_BRUSCO_DER = 11;
const int MOV_DER_1 = 8, MOV_DER_2 = 9, MOV_DER_3 = 10;
const int MOV_IZQ_1 = 5, MOV_IZQ_2 = 4, MOV_IZQ_3 = 3;

SpriteMario::SpriteMario(string direccionImagen){
	direccionTextura = direccionImagen;
	estadoActual = QUIETO_DER;
	proximoEstado = QUIETO_DER;
	int corrimientoEnImagen = 0;

	// Variables que controlan el cambio de Sprite
	contadorEvento = 0;
	temporizadorMarioAgachado = 0;

	for(int i=0;i<14;i++){
		estadosPosibles[i].x = corrimientoEnImagen;
		estadosPosibles[i].y = 0;
		estadosPosibles[i].w = ANCHO_SPRITE_PIXEL;
		estadosPosibles[i].h = ALTO_SPRITE_PIXEL;
		corrimientoEnImagen+= PASO_PROXIMO_SPRITE;
	}
}

SDL_Rect SpriteMario::obtenerRectanguloActual(){
	return estadosPosibles[estadoActual];
}

bool SpriteMario::controlDeFlujoSprite() {

    bool estabaQuieto = estadoActual == QUIETO_IZQ || estadoActual == QUIETO_DER;

    if (estabaQuieto) { return estabaQuieto; }

    if (++contadorEvento < CANT_DE_EVENTOS_MAX) { return false; }

    contadorEvento = 0;

    return true;
}
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++ ACTUALIZAR ESTADO DE MARIO SEGUN EL EVENTO ++++++++++++++*/
/* Se actualiza estado segun corresponda luego de presionar LEFT o RIGTH */
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

bool SpriteMario::estaCorriendo(Mario* mario, bool btnDerecho) {

    bool corriendo = false;

    /* Corriendo hacia la izquierda, Estados permitidos: 3, 4 y 5 */
    if (estadoActual >= CAMBIO_BRUSCO_IZQ && estadoActual < QUIETO_DER) {
        corriendo = true;

        // Cuando la funcion controlDeFlujoSprite devuelve "true", se permite el cambio de sprite.
        if (!controlDeFlujoSprite()) { return corriendo; }

        estadoActual--;
        proximoEstado = estadoActual - 1;

        /* Fuera de rango */
        if (estadoActual == CAMBIO_BRUSCO_IZQ) {
            estadoActual = MOV_IZQ_1;
			proximoEstado = MOV_IZQ_2;
        }
        if (proximoEstado == CAMBIO_BRUSCO_IZQ) { proximoEstado = MOV_IZQ_1; }
    }


    /* Corriendo hacia la derecha, Estados permitidos: 8, 9 y 10 */
    else if (estadoActual > QUIETO_IZQ && estadoActual <= CAMBIO_BRUSCO_DER) {
        corriendo = true;

        // Cuando la funcion controlDeFlujoSprite devuelve "true", se permite el cambio de sprite.
        if (!controlDeFlujoSprite()) { return corriendo; }

        estadoActual++;
        proximoEstado = estadoActual + 1;

        /* Fuera de rango */
        if (estadoActual == CAMBIO_BRUSCO_DER) {
            estadoActual = MOV_DER_1;
			proximoEstado = MOV_DER_2;
        }
        if (proximoEstado == CAMBIO_BRUSCO_DER) { proximoEstado = MOV_DER_1; }

    }

    return corriendo;
}


bool SpriteMario::estaCambiandoDeDireccion(Mario* mario, bool btnDerecho) {
	bool cambiandoDirecc = false;

	bool caminandoAIzq = estadoActual > CAMBIO_BRUSCO_IZQ && estadoActual < QUIETO_IZQ;
	bool caminandoADer = estadoActual > QUIETO_DER && estadoActual < CAMBIO_BRUSCO_DER;


	/* Mario caminando */
	if (caminandoAIzq && btnDerecho) {
		cambiandoDirecc = true;
		estadoActual = CAMBIO_BRUSCO_IZQ;
		proximoEstado = QUIETO_DER;
	}
	else if (caminandoADer && !btnDerecho) {
	    cambiandoDirecc = true;
        estadoActual = CAMBIO_BRUSCO_DER;
        proximoEstado = QUIETO_IZQ;
	}

	/* Mario quieto */
	else if (estadoActual == QUIETO_IZQ && btnDerecho) {
		cambiandoDirecc = true;
		if (!controlDeFlujoSprite()) { return cambiandoDirecc; }
		estadoActual = QUIETO_DER;
		proximoEstado = QUIETO_DER;
	}
	else if (estadoActual == QUIETO_DER && !btnDerecho) {
		cambiandoDirecc = true;
		if (!controlDeFlujoSprite()) { return cambiandoDirecc; }
		estadoActual = QUIETO_IZQ;
		proximoEstado = QUIETO_IZQ;
	}

    return cambiandoDirecc;
}

bool SpriteMario::estaSaltando(Mario* mario, bool btnDerecho) {

    bool saltando = false;

    if (estadoActual != SALTANDO_DER && estadoActual != SALTANDO_IZQ ) {
    	return saltando;
    }

	saltando = true;

    if (btnDerecho && !mario->estaEnElPiso()) {
    	estadoActual = SALTANDO_DER;
    	proximoEstado = SALTANDO_DER;
    }

    else if (btnDerecho && mario->estaEnElPiso()) {
    	estadoActual = QUIETO_DER;
    	proximoEstado = MOV_DER_1;

    }

    else if (!btnDerecho && !mario->estaEnElPiso()) {
    	estadoActual = SALTANDO_IZQ;
    	proximoEstado = SALTANDO_IZQ;
    }

    else if (!btnDerecho && mario->estaEnElPiso()) {
    	estadoActual = QUIETO_IZQ;
    	proximoEstado = MOV_IZQ_1;
    }

    return saltando;
}


bool SpriteMario::estaAgachado(Mario* mario, bool btnDerecho) {
	bool agachado = estadoActual == AGACHADO_IZQ || estadoActual == AGACHADO_DER;

	if (btnDerecho && agachado && mario->estaEnElPiso()) {

		estadoActual = MOV_DER_1;
		proximoEstado = MOV_DER_2;
	}

	else if (!btnDerecho && agachado && mario->estaEnElPiso()) {

		estadoActual = MOV_IZQ_1;
		proximoEstado = MOV_IZQ_2;
	}

	return agachado;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/*+++++++++++++++++++++ ACTUALIZAR ESTADO DE MARIO +++++++++++++++++++++*/
/*       Se actualiza estado cuando no se produce ningun evento         */
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
bool SpriteMario::estaSaltando(Mario* mario) {
    bool saltando = estadoActual == SALTANDO_IZQ || estadoActual == SALTANDO_DER;

    if(saltando && mario->estaQuietoX() && mario->estaEnElPiso()) {
        if (estadoActual == SALTANDO_IZQ) {
            estadoActual = QUIETO_IZQ;
            proximoEstado = QUIETO_IZQ;
        }
        else if (estadoActual == SALTANDO_DER) {
            estadoActual = QUIETO_DER;
            proximoEstado = QUIETO_DER;
        }
    }

    else if(saltando && !mario->estaQuietoX()) {
        estadoActual = proximoEstado;
        if(estadoActual == SALTANDO_IZQ && mario->estaEnElPiso()) {
            estadoActual = MOV_IZQ_1;
            proximoEstado = MOV_IZQ_2;
        }
        else if(estadoActual == SALTANDO_DER && mario->estaEnElPiso()) {
            estadoActual = MOV_DER_1;
            proximoEstado = MOV_DER_2;
        }

    }
    return saltando;
}

bool SpriteMario::estaCambiandoDeDireccion(Mario* mario) {
    bool cambiandoDirecc = estadoActual == CAMBIO_BRUSCO_IZQ || estadoActual == CAMBIO_BRUSCO_DER;

    if (cambiandoDirecc &&  mario->estaQuietoX()) {
        if (estadoActual == CAMBIO_BRUSCO_IZQ) {
            estadoActual = QUIETO_DER;
            proximoEstado = QUIETO_DER;
        } else {
            estadoActual = QUIETO_IZQ;
            proximoEstado = QUIETO_IZQ;
        }
    }
    if (cambiandoDirecc &&  !mario->estaQuietoX()) {
        if (estadoActual == CAMBIO_BRUSCO_IZQ) {
            if(!controlDeFlujoSprite()) {return cambiandoDirecc;}
            estadoActual = MOV_DER_1;
            proximoEstado = MOV_DER_2;
        } else {
            if(!controlDeFlujoSprite()) {return cambiandoDirecc;}
            estadoActual = MOV_IZQ_1;
            proximoEstado = MOV_IZQ_2;
        }
    }

    return cambiandoDirecc;
}

bool SpriteMario::estaCorriendo(Mario* mario) {

    bool corriendo = false;

    // CORRIENDO HACIA LA IZQUIERDA
    if (estadoActual > CAMBIO_BRUSCO_IZQ && estadoActual < QUIETO_DER) {
        corriendo = true;
        if (mario->estaQuietoX()) {
            estadoActual = QUIETO_IZQ;
            proximoEstado = QUIETO_IZQ;
        }
        else {
            estadoActual = proximoEstado;
            proximoEstado--;
            if (proximoEstado == CAMBIO_BRUSCO_IZQ) { proximoEstado = MOV_IZQ_1; }
        }
    }

    // CORIIENDO HACIA LA DERECHA
    if (estadoActual > QUIETO_IZQ && estadoActual < CAMBIO_BRUSCO_DER) {
        corriendo = true;
        if (mario->estaQuietoX()) {
            estadoActual = QUIETO_DER;
            proximoEstado = QUIETO_DER;
        }
        else {
            estadoActual = proximoEstado;
            proximoEstado++;
            if (proximoEstado == CAMBIO_BRUSCO_DER) { proximoEstado = MOV_DER_1; }
        }
    }

    return corriendo;

}

bool SpriteMario::estaAgachado(Mario* mario) {
    bool estaAgachado = estadoActual == AGACHADO_DER || estadoActual == AGACHADO_IZQ;

    if (!estaAgachado) {
        temporizadorMarioAgachado = 0;
        return estaAgachado;
    }

    if(++temporizadorMarioAgachado < 3 ) { return estaAgachado; }

    temporizadorMarioAgachado = 0;

    if (estadoActual == AGACHADO_DER && mario->estaQuietoX() && mario->estaEnElPiso()) {
        estadoActual = QUIETO_DER; proximoEstado = QUIETO_DER;
    }

    else if (estadoActual == AGACHADO_IZQ && mario->estaQuietoX() && mario->estaEnElPiso()) {
        estadoActual = QUIETO_IZQ; proximoEstado = QUIETO_IZQ;
    }

    return estaAgachado;
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void SpriteMario::actualizarSprite(Mario* mario){

	if(estaSaltando(mario)) { return; }

    if (!controlDeFlujoSprite()) { return; }

    if (estaAgachado(mario)) {return; }

    if(estaCambiandoDeDireccion(mario)) { return; }

	estaCorriendo(mario);
}
/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void SpriteMario::actualizarSpriteMarioDerecha(Mario* mario) {

    if (estaSaltando(mario,true)) { return; }

    if (estaCambiandoDeDireccion(mario,true)) { return; }

    if (estaCorriendo(mario, true)) { return; }

    if (estaAgachado(mario,true)) { return; }
}

void SpriteMario::actualizarSpriteMarioIzquierda(Mario* mario) {

    if (estaSaltando(mario,false)) { return; }

    if (estaCambiandoDeDireccion(mario,false)) { return; }

    if (estaCorriendo(mario,false)) { return; }

    if (estaAgachado(mario,false)) { return; }
}

void SpriteMario::actualizarSpriteMarioSaltar() {

    if (estadoActual >= QUIETO_DER && estadoActual <= MOV_DER_3) {
        estadoActual = SALTANDO_DER;
        proximoEstado = SALTANDO_DER;
    }

    else if (estadoActual == AGACHADO_DER) {
        estadoActual = AGACHADO_DER;
        proximoEstado = AGACHADO_DER;
    }

    else if (estadoActual >= MOV_IZQ_3 && estadoActual <= QUIETO_IZQ) {
        estadoActual = SALTANDO_IZQ;
        proximoEstado = SALTANDO_IZQ;
    }

    else if (estadoActual == SALTANDO_IZQ) {
        estadoActual = SALTANDO_IZQ;
        proximoEstado = SALTANDO_IZQ;
    }
}

void SpriteMario::actualizarSpriteMarioAgacharse() {

    // Reinicia el temporizador
    temporizadorMarioAgachado = 0;

    if (estadoActual >= QUIETO_DER && estadoActual <= MOV_DER_3) {
        estadoActual = AGACHADO_DER;
        proximoEstado = AGACHADO_DER;
    }
    else if (estadoActual >= MOV_IZQ_3 && estadoActual <= QUIETO_IZQ) {
        estadoActual = AGACHADO_IZQ;
        proximoEstado = AGACHADO_IZQ;
    }
}