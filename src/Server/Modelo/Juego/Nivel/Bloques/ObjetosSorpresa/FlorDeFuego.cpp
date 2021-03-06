#include "FlorDeFuego.hpp"
#include "src/Server/Botonera/Botonera.hpp"
#include "src/Utils/Constantes.hpp"

const int CANTIDAD_COLORES = 4;

FlorDeFuego::FlorDeFuego(int posX, int posY) {
    posicionFija = PosicionFija(posX,posY);
    _debeDesaparecer = false;
    this->color = rand()%CANTIDAD_COLORES;
    FlorDeFuego::inicializarMapasDeColision();
}

void FlorDeFuego::usarse(Mario *mario){}

entidad_t FlorDeFuego::serializar() {
    entidad_t florSerializada = {0,0,0,0,0,false};
    florSerializada.x = posicionFija.obtenerPosX();
    florSerializada.y = posicionFija.obtenerPosY();
    florSerializada.recorteX = color;
    florSerializada.tipo = FLOR;
    return florSerializada;
}

void FlorDeFuego::actualizar() {}

int FlorDeFuego::obtenerPosicionX() {
    return posicionFija.obtenerPosX();
}

std::string FlorDeFuego::obtenerColisionID() {
    return COLISION_ID_FLOR;
}

rectangulo_t FlorDeFuego::obtenerRectangulo() {
    int x = posicionFija.obtenerPosX();
    int y = posicionFija.obtenerPosY();
    int w = ANCHO_FLOR;
    int h = ALTO_FLOR;
    return {x, x+w, y, y+h, h, w};
}

bool FlorDeFuego::debeColisionar() {
    return !_debeDesaparecer;
}

void FlorDeFuego::inicializarMapasDeColision() {
    auto pAgarrar = (void (Colisionable::*)(void*))&FlorDeFuego::agarrar;
    Colisionable::parFuncionColisionContexto_t parAgarrar = {pAgarrar, nullptr};

    mapaColisionesPorDerecha[COLISION_ID_MARIO] = parAgarrar;
    mapaColisionesPorIzquierda[COLISION_ID_MARIO] = parAgarrar;
    mapaColisionesPorArriba[COLISION_ID_MARIO] = parAgarrar;
    mapaColisionesPorAbajo[COLISION_ID_MARIO] = parAgarrar;
}

void FlorDeFuego::agarrar(__attribute__((unused))void *ptr) {
    _debeDesaparecer = true;
}

void FlorDeFuego::chocarPorDerechaCon(Colisionable *colisionable) {
    if(colisionable->obtenerColisionID() == COLISION_ID_MARIO){
        if(!(((Mario *) colisionable)->puedeAgarrarFlor())){
            return;
        }
    }
    Colisionable::chocarPorDerechaCon(colisionable);
}

void FlorDeFuego::chocarPorIzquierdaCon(Colisionable *colisionable) {
    if(colisionable->obtenerColisionID() == COLISION_ID_MARIO){
        if(!(((Mario *) colisionable)->puedeAgarrarFlor())){
            return;
        }
    }
    Colisionable::chocarPorIzquierdaCon(colisionable);
}

void FlorDeFuego::chocarPorArribaCon(Colisionable *colisionable) {
    if(colisionable->obtenerColisionID() == COLISION_ID_MARIO){
        if(!(((Mario *) colisionable)->puedeAgarrarFlor())){
            return;
        }
    }
    Colisionable::chocarPorArribaCon(colisionable);
}

void FlorDeFuego::chocarPorAbajoCon(Colisionable *colisionable) {
    if(colisionable->obtenerColisionID() == COLISION_ID_MARIO){
        if(!(((Mario *) colisionable)->puedeAgarrarFlor())){
            return;
        }
    }
    Colisionable::chocarPorAbajoCon(colisionable);
}

void FlorDeFuego::sonar() {
    Botonera::getInstance()->agregarSonido(SONIDO_APARECE_FLOR, ID_TODOS_DETONANTES);
}

int FlorDeFuego::tipoDeEfecto() {
    return FLOR;
}
