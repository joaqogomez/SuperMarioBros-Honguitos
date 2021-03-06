#include "RecorteFlor.hpp"

#define CANTIDAD_FLORES 4
#define ANCHO_FLOR_SPRITE 16
#define ALTO_FLOR_SPRITE 16
#define DESPLAZAMIENTO_FLOR 16

RecorteFlor::RecorteFlor() {
    inicializarEstados(CANTIDAD_FLORES, DESPLAZAMIENTO_FLOR, ALTO_FLOR_SPRITE, ANCHO_FLOR_SPRITE);
}

int RecorteFlor::obtenerAnchuraParaDibujarImagen(int recorteX, int recorteY) {
    return ANCHO_FLOR;
}

int RecorteFlor::obtenerAlturaParaDibujarImagen(int recorteX, int recorteY) {
    return ALTO_FLOR;
}
