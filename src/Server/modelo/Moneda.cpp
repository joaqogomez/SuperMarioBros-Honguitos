#include "Moneda.hpp"


Moneda::Moneda(int coordenadaX, int coordenadaY) {
    this->posicion = new PosicionFija(coordenadaX, coordenadaY);
    this->sprite = new SpriteMoneda();
    agarrada = false;
    inicializarMapasDeColision();
}

Moneda::~Moneda() {
    delete this->posicion;
    delete this->sprite;
}

void Moneda::actualizar() {
    this->sprite->actualizarSprite();
}

moneda_t Moneda::serializar() {
    moneda_t monedaSerializada;

    monedaSerializada.posX = posicion->obtenerPosX();
    monedaSerializada.posY = posicion->obtenerPosY();
    monedaSerializada.numeroRecorte = sprite->obtenerEstadoActual();
    return monedaSerializada;
}

int Moneda::obtenerPosicionX() {
    return this->posicion->obtenerPosX();
}

int Moneda::obtenerPosicionY() {
    return this->posicion->obtenerPosY();
}

string Moneda::obtenerColisionID() {
    return COLISION_ID_MONEDA;
}

rectangulo_t Moneda::obtenerRectangulo() {
    int x = posicion->obtenerPosX();
    int y = posicion->obtenerPosY();
    int w = LARGO_MONEDA;
    int h = LARGO_MONEDA;
    return {x, x+w, y, y+h, h, w};
}

void Moneda::inicializarMapasDeColision() {
    auto pAgarrar = (void (Colisionable::*)(void*))&Moneda::agarrar;
    Colisionable::parFuncionColisionContexto_t parAgarrarMoneda = {pAgarrar, nullptr};

    mapaColisionesPorDerecha[COLISION_ID_MARIO] = parAgarrarMoneda;
    mapaColisionesPorIzquierda[COLISION_ID_MARIO] = parAgarrarMoneda;
    mapaColisionesPorArriba[COLISION_ID_MARIO] = parAgarrarMoneda;
    mapaColisionesPorAbajo[COLISION_ID_MARIO] = parAgarrarMoneda;
}

bool Moneda::fueAgarrada() {
    return agarrada;
}

void Moneda::agarrar(void *ptr) {
    agarrada = true;
}
