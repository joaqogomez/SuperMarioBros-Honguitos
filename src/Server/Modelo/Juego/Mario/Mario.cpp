#include "Mario.hpp"
#include "src/Utils/Constantes.hpp"
#include "src/Server/Modelo/Juego/Sprites/SpriteMario.hpp"
#include "src/Server/Botonera/Botonera.hpp"
#include "Modificadores/SinModificador.hpp"
#include "src/Utils/log/Log.hpp"

const int COORDENADA_X_DEFAULT = 20,COORDENADA_Y_DEFAULT = 300;
const int MINIMO_COORDENADA_Y = 0;
const int TERRENO_LIMITE_DERECHO_MAX = 8177,TERRENO_LIMITE_DERECHO_MIN = 0;
const short MARIO_DESCONECTADO = -1;
const int SIN_INMUNIDAD = 180, EMPIEZA_INMUNIDAD = 0, PIERDE_INMUNIDAD = 179;

Mario::Mario(int numeroJugador):posicionDeReaparicion(COORDENADA_X_DEFAULT, COORDENADA_Y_DEFAULT),
                                manejadorSonido(numeroJugador){
	this->posicion = new PosicionMovil(COORDENADA_X_DEFAULT, COORDENADA_Y_DEFAULT, MINIMO_COORDENADA_Y,
			                            TERRENO_LIMITE_DERECHO_MIN, TERRENO_LIMITE_DERECHO_MAX);
	this->puntos=0;
	this->movimiento = new MovimientoMario();
	this->spriteMario = new SpriteMario();
	this->modificador = new SinModificador(this);
	this->vidaMario = new VidaMario();
	this->numeroJugador = numeroJugador;
	this->estaConectadoElJugador = true;
    this->agarreUnaFlorEnEsteInstante = false;
    this->estaEnModoTest = false;
    Mario::inicializarMapasDeColision();
    ticksInmunidad = SIN_INMUNIDAD;
}

void Mario::inicializarMapasDeColision(){
    auto pMatar = (void (Colisionable::*)(void*)) &Mario::matarEnemigo;
    auto pGanarPuntos = (void (Colisionable::*)(void*)) &Mario::agregarPuntos;
    auto pHacerseDeFuego = (void (Colisionable::*)(void*)) &Mario::hacerseDeFuego;

    Colisionable::parFuncionColisionContexto_t parMatarGoomba = {pMatar, (void *) &PUNTOS_GOOMBA};
    Colisionable::parFuncionColisionContexto_t parMatarKoopa = {pMatar, (void *) &PUNTOS_KOOPA};
    Colisionable::parFuncionColisionContexto_t parAgarrarMoneda = {pGanarPuntos, (void* ) &PUNTOS_POR_MONEDA};
    Colisionable::parFuncionColisionContexto_t parHacerseDeFuego = {pHacerseDeFuego, nullptr};


    mapaColisionesPorDerecha[COLISION_ID_MONEDA] = parAgarrarMoneda;
    mapaColisionesPorDerecha[COLISION_ID_FLOR] = parHacerseDeFuego;

    mapaColisionesPorIzquierda[COLISION_ID_MONEDA] = parAgarrarMoneda;
    mapaColisionesPorIzquierda[COLISION_ID_FLOR] = parHacerseDeFuego;

    mapaColisionesPorArriba[COLISION_ID_MONEDA] = parAgarrarMoneda;
    mapaColisionesPorArriba[COLISION_ID_FLOR] = parHacerseDeFuego;

    mapaColisionesPorAbajo[COLISION_ID_KOOPA] = parMatarKoopa;
    mapaColisionesPorAbajo[COLISION_ID_GOOMBA] = parMatarGoomba;
    mapaColisionesPorAbajo[COLISION_ID_MONEDA] = parAgarrarMoneda;
    mapaColisionesPorAbajo[COLISION_ID_FLOR] = parHacerseDeFuego;

    inicializarMapaMorirPorEnemigos();
}

int Mario::obtenerNumeroJugador() const{
    return this->numeroJugador;
}

void Mario::actualizarSaltarMario(){
    if(!movimiento->estaEnElAire()){
        manejadorSonido.reproducirSonidoSalto();
        movimiento->saltar();
        spriteMario->actualizarSpriteMarioSaltar();
    }
}

void Mario::actualizarAgacharseMario(){
	spriteMario->actualizarSpriteMarioAgacharse();
}

void Mario::actualizarIzquierdaMario(){
	spriteMario->actualizarSpriteMarioIzquierda(this);
	movimiento->aceleraraIzquierda();
}

void Mario::actualizarDerechaMario(){
	spriteMario->actualizarSpriteMarioDerecha(this);
	movimiento->aceleraraDerecha();
}

void Mario::actualizarMaximoX(int limite){
    this->posicion->actualizarLimiteTerrenoIzq(limite);
}

void Mario::actualizarMinimoX(int limite){
    this->posicion->actualizarLimiteTerrenoDer(limite);
}

int Mario::obtenerPosicionX(){
	return posicion->obtenerPosX();
}

int Mario::obtenerPosicionY(){
	return posicion->obtenerPosY();
}

int Mario::obtenerPuntos() const{
	return puntos;
}

void Mario::agregarPuntos(int unosPuntos){
	puntos+=unosPuntos;
	this->notificar();
}

void Mario::agregarMoneda(){
    manejadorSonido.reproducirSonidoMoneda();
    puntos+=PUNTOS_POR_MONEDA;
}

mario_t Mario::serializar(unsigned short idImagen){
	mario_t marioSerializado;
    marioSerializado.puntos = puntos;
	marioSerializado.idImagen = idImagen;
	marioSerializado.posX = posicion->obtenerPosX();
	marioSerializado.posY = posicion->obtenerPosY();
	marioSerializado.recorteImagen = spriteMario->obtenerEstadoActual();
    marioSerializado.vidas = vidaMario->obtenerVida();
    marioSerializado.modificador = modificador->serializar();
	if(!estaConectadoElJugador){
		marioSerializado.recorteImagen = MARIO_DESCONECTADO;
	}
	return marioSerializado;
}


void Mario::reiniciarPosicion(){
	delete posicion;
	delete movimiento;
	movimiento = new MovimientoMario();
	posicion = new PosicionMovil(COORDENADA_X_DEFAULT,COORDENADA_Y_DEFAULT, MINIMO_COORDENADA_Y,
			TERRENO_LIMITE_DERECHO_MIN, TERRENO_LIMITE_DERECHO_MAX);
}


void Mario::actualizarPosicion(){
    if(!estaVivo() || !estaConectado()){return;}
    spriteMario->actualizarSprite(this);
	this->movimiento->mover(this->posicion);
	if(this->posicion->obtenerPosY() == MINIMO_COORDENADA_Y){
		this->perderVida();
	}
	modificador->actualizar();
	if(ticksInmunidad<PIERDE_INMUNIDAD && ticksInmunidad != SIN_INMUNIDAD){
        ticksInmunidad++;
	}
	else if (ticksInmunidad == PIERDE_INMUNIDAD && !estaEnModoTest){
        ticksInmunidad = SIN_INMUNIDAD;
        inicializarMapasDeColision();
	}
	Log::getInstance()->mostrarPosicion("Mario", posicion->obtenerPosX(), posicion->obtenerPosY());
}

bool Mario::estaConectado() const{
	return this->estaConectadoElJugador;
}

void Mario::serArrastrado(int corrimiento){
	this->posicion->moverHorizontal(corrimiento);
}

bool Mario::estaQuietoX(){
	return this->movimiento->estaQuieto();
}

bool Mario::estaEnElPiso(){
    return !movimiento->estaEnElAire();
}

bool Mario::puedeAgarrarFlor() {
    bool retorno = agarreUnaFlorEnEsteInstante || modificador->puedeAgarrarFlor();
    agarreUnaFlorEnEsteInstante = false;
    return retorno;
}

int Mario::obtenerVida(){
    return vidaMario->obtenerVida();
}

void Mario::perderVida(void* ptr) {
    posicion->reiniciar(posicionDeReaparicion);
    movimiento->reiniciar();
    if(!this->estaEnModoTest){
        ModificadorMario* nuevoModificador = modificador->perderVida(vidaMario);
        swapDeModificador(nuevoModificador);
        manejadorSonido.activarSonidoFlor();
        manejadorSonido.reproducirSonidoMuerte();
        ticksInmunidad = EMPIEZA_INMUNIDAD;
        desactivarMapaColisionesEnemigos();
    }
}

void Mario::swapDeModificador(ModificadorMario* nuevoModificador){
    delete modificador;
    modificador = nuevoModificador;
}

void Mario::hacerseDeFuego() {
    ModificadorMario* nuevoModificador = modificador->hacerseDeFuego();
    swapDeModificador(nuevoModificador);
}
void Mario::hacerseDeFuego(void *pVoid) {
    if(modificador->puedeAgarrarFlor()) {
        agarreUnaFlorEnEsteInstante = true;
        this->hacerseDeFuego();
        manejadorSonido.desactivarSonidoFlor();
    }
}

ObjetoFugaz* Mario::dispararFuego() {
    Posicion posManos = spriteMario->posicionManos();
    PosicionFija posicionManosMario(obtenerPosicionX() + posManos.obtenerPosX(),obtenerPosicionY() + posManos.obtenerPosY());
    ObjetoFugaz* disparo =  modificador->dispararFuego(posicionManosMario, spriteMario->direccionMirada(), movimiento->obtenerVelocidadXActual());
    manejadorSonido.reproducirSonidoDisparo(disparo->tipoDeEfecto());
    return disparo;
}

std::string Mario::obtenerColisionID() {
    return COLISION_ID_MARIO;
}

rectangulo_t Mario::obtenerRectangulo() {
    int x = this->obtenerPosicionX();
    int y = this->obtenerPosicionY();
    int h = ALTO_MARIO;
    if(spriteMario->estaAgachado()){
        h = ALTO_MARIO_AGACHADO;
    }
    int w = ANCHO_MARIO;
    rectangulo_t rectangulo = {x,x+w,y,y+h, h, w};
    return rectangulo;
}

void Mario::agregarPuntos(void *puntos) {
    if(puntos != nullptr && estaVivo()){
        agregarPuntos(*((int*) puntos));
    }
}

void Mario::matarEnemigo(void* puntos){
    movimiento->impulsarY();
    if(puntos != nullptr) {
        agregarPuntos(*((int *) puntos));
    }
    spriteMario->actualizarSpriteMarioSaltar();
}

void Mario::chocarPorDerechaCon(Colisionable *colisionable) {
    manejadorSonido.reproducirSonidoDerecha(colisionable->obtenerColisionID());
    if(esUnBloque(colisionable->obtenerColisionID())){
        empujarEnX(colisionable->obtenerRectangulo(),IZQUIERDA);
    }
    else{
        Colisionable::chocarPorDerechaCon(colisionable);
    }
}
void Mario::chocarPorIzquierdaCon(Colisionable *colisionable) {
    manejadorSonido.reproducirSonidoIzquierda(colisionable->obtenerColisionID());
    if(esUnBloque(colisionable->obtenerColisionID())){
        empujarEnX(colisionable->obtenerRectangulo(), DERECHA);
    }
    else{
        Colisionable::chocarPorIzquierdaCon(colisionable);
    }
}

void Mario::chocarPorArribaCon(Colisionable *colisionable) {
    manejadorSonido.reproducirSonidoArriba(colisionable->obtenerColisionID());
    if(esUnBloque(colisionable->obtenerColisionID())){
        empujarEnY(colisionable->obtenerRectangulo(),ABAJO);
    }
    else {
        Colisionable::chocarPorArribaCon(colisionable);
    }
}

void Mario::chocarPorAbajoCon(Colisionable *colisionable) {
    manejadorSonido.reproducirSonidoAbajo(colisionable->obtenerColisionID());
    if(esUnBloque(colisionable->obtenerColisionID())){
        empujarEnY(colisionable->obtenerRectangulo(),ARRIBA);
    }
    else{
        Colisionable::chocarPorAbajoCon(colisionable);
    }
}

void Mario::empujarEnX(rectangulo_t rectanguloBloque,int direccion){
    movimiento->setVelocidadX(0);
    rectangulo_t rectanguloMario = obtenerRectangulo();
    if(direccion == DERECHA){
        this->posicion->moverHorizontal(rectanguloBloque.x2-rectanguloMario.x1);
    }
    else{
        this->posicion->moverHorizontal(-(rectanguloMario.x2-rectanguloBloque.x1));
    }
}

void Mario::empujarEnY(rectangulo_t rectanguloBloque, int direccion) {
    movimiento->setVelocidadY(0);
    rectangulo_t rectanguloMario = obtenerRectangulo();
    if(direccion == ABAJO){
        this->posicion->moverVertical(-(rectanguloMario.y2-rectanguloBloque.y1));
    }
    else{
        this->posicion->moverVertical(rectanguloBloque.y2-rectanguloMario.y1);
        this->movimiento->teParasteEnBloque();
    }
}

bool Mario::debeColisionar() {
    return estaConectadoElJugador && vidaMario->obtenerVida() > 0;
}

void Mario::alternarModoTest() {
    auto pPerderVida = (void (Colisionable::*)(void*))&Mario::perderVida;
    Colisionable::parFuncionColisionContexto_t parPerderVida = {pPerderVida, nullptr};
    this->estaEnModoTest = !this->estaEnModoTest;
    if(estaEnModoTest){
        desactivarMapaColisionesEnemigos();
    }else{
        inicializarMapaMorirPorEnemigos();
    }

}

void Mario::desactivarMapaColisionesEnemigos() {
    mapaColisionesPorArriba.erase(COLISION_ID_KOOPA);
    mapaColisionesPorArriba.erase(COLISION_ID_GOOMBA);
    mapaColisionesPorDerecha.erase(COLISION_ID_KOOPA);
    mapaColisionesPorDerecha.erase(COLISION_ID_GOOMBA);
    mapaColisionesPorIzquierda.erase(COLISION_ID_KOOPA);
    mapaColisionesPorIzquierda.erase(COLISION_ID_GOOMBA);
}

void Mario::inicializarMapaMorirPorEnemigos() {
    auto pPerderVida = (void (Colisionable::*)(void*))&Mario::perderVida;
    Colisionable::parFuncionColisionContexto_t parPerderVida = {pPerderVida, nullptr};

    mapaColisionesPorDerecha[COLISION_ID_KOOPA] = parPerderVida;
    mapaColisionesPorDerecha[COLISION_ID_GOOMBA] = parPerderVida;
    mapaColisionesPorIzquierda[COLISION_ID_KOOPA] = parPerderVida;
    mapaColisionesPorIzquierda[COLISION_ID_GOOMBA] = parPerderVida;
    mapaColisionesPorArriba[COLISION_ID_KOOPA] = parPerderVida;
    mapaColisionesPorArriba[COLISION_ID_GOOMBA] = parPerderVida;
}

int Mario::obtenerID() {
    return numeroJugador;
}

void Mario::desconectar() {
    estaConectadoElJugador = false;
}

void Mario::conectar() {
    estaConectadoElJugador = true;
}

bool Mario::estaVivo() {
    return vidaMario->estaVivo();
}

void Mario::nuevoPuntoDeReaparicion(Posicion puntoDeReaparicion) {
    this->posicionDeReaparicion = PosicionFija(puntoDeReaparicion.obtenerPosX(), puntoDeReaparicion.obtenerPosY());
}

int8_t Mario::obtenerEstadoActual() {
    return spriteMario->obtenerEstadoActual();
}

Mario::~Mario() {
    delete this->spriteMario;
    delete this->posicion;
    delete this->movimiento;
    delete this->modificador;
    delete this->vidaMario;
}