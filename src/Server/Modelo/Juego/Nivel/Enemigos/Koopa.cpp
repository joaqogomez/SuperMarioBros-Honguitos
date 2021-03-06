#include "Koopa.hpp"
#include "src/Server/Modelo/Juego/Sprites/SpriteKoopa.hpp"
#include "src/Utils/Constantes.hpp"
#define DEFAULT 100
#define VELOCIDAD_EXTRA_KOOPA 0.5
#define GRAVEDAD_KOOPA 0.1
Koopa::Koopa(int tipoColor){
    this->tipoColor = tipoColor;
    loMataron = false;
    this->inicializarMapasDeColision();
    posicionActual = PosicionMovil(DEFAULT, DEFAULT);
    movimientoEnemigo = MovimientoEnemigo(VELOCIDAD_EXTRA_KOOPA, GRAVEDAD_KOOPA);
    bool debeEspejarse = this->movimientoEnemigo.debeEspejarse();
    spriteEnemigo = new SpriteKoopa(debeEspejarse, this);
}

Koopa::~Koopa() {
    delete spriteEnemigo;
}

entidad_t Koopa::serializar() {
    return serializarEnemigo(KOOPA);
}

std::string Koopa::obtenerColisionID() {
    return COLISION_ID_KOOPA;
}

bool Koopa::estaEnElAire() {
    return !pisoBloque;
}
