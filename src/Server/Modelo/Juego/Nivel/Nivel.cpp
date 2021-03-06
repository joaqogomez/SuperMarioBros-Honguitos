#include "src/Utils/log/Log.hpp"
#include "Nivel.hpp"
#include "src/Utils/colisiones/Colisionador.hpp"
#include "Filtro.hpp"


const int ALTO_NIVEL = 600;
const int TAMANIO_MONEDA = 40;
const int TAMANIO_BLOQUE = 40;
const int TAMANIO_ENEMIGO = 40;

Nivel::Nivel(int mundo, std::string direccionFondo, int tiempo, int cantidadMonedas, int puntoBanderaFin, int altoPiso)
      : piso(altoPiso), meta(ANCHO_FONDO* (float) puntoBanderaFin /100),contador(tiempo + TIEMPO_ESPERA_GAME_LOOP, SEGUNDOS){
    this->mundo = mundo;
    this->direccionFondo = std::move(direccionFondo);
    this->cantidadMonedas = cantidadMonedas;
    this->puntoBanderaFin = ANCHO_FONDO* (float) puntoBanderaFin /100;
}

void Nivel::actualizarPosicionesEnemigos(rectangulo_t rectangulo) {
	Log* log = Log::getInstance();
	int i = 1;
	for (auto& enemigo : enemigos) {
        enemigo->actualizarPosicion(rectangulo);
	    log->mostrarPosicion("Enemigo " + std::to_string(i), enemigo->obtenerPosicionX(), enemigo->obtenerPosicionY());
	    i++;
	}
}


void Nivel:: actualizarModelo(const std::map<int, Mario*>& jugadores, rectangulo_t rectanguloEscena){
    actualizarPosicionesEnemigos(rectanguloEscena);

    imponerPosicionDeReaparicion(jugadores, rectanguloEscena);
    resolverColisiones(jugadores, rectanguloEscena);

    actualizador.actualizar(&objetosFugaces);
    actualizador.actualizar(&monedas);
    actualizador.actualizar(&plataformas);

    sacarEnemigosMuertos();

    resolverGanadores(jugadores);
}

void Nivel::resolverColisiones(const std::map<int, Mario *>& jugadores, rectangulo_t rectanguloEscena) {
    std::list<Colisionable*> plataformasPiso = piso.obtenerPiso();
    std::list<Colisionable*> plataformasPisoEnEscena = Filtro::filtrarPlataformasEnEscena(plataformasPiso, rectanguloEscena);
    std::list<Colisionable*> enemigosEnEscena = Filtro::filtrarEnemigosEnEscena(enemigos, rectanguloEscena);
    std::list<Colisionable*> monedasEnEscena = Filtro::filtrarMonedasEnEscena(monedas, rectanguloEscena);
    std::list<Colisionable*> bloquesEnEscena = Filtro::filtrarBloquesEnEscena(plataformas, rectanguloEscena);
    std::list<Colisionable*> objetosEnEscena = Filtro::filtrarObjetosEnEscena(objetosFugaces, rectanguloEscena);
    std::list<Colisionable*> piezasDeTuberiaEnEscena = Filtro::filtrarPiezasDeTuberiaEnEscena(tuberias, rectanguloEscena);

    for(auto& parClaveJugador: jugadores){
        Mario* jugador = parClaveJugador.second;
        chocarContraTodos(jugador, enemigosEnEscena, nullptr, nullptr);
        chocarContraTodos(jugador, monedasEnEscena, nullptr, nullptr);
        chocarContraTodos(jugador, bloquesEnEscena, &Nivel::agregarObjeto_helper, this);
        chocarContraTodos(jugador, objetosEnEscena, nullptr, nullptr);
        chocarContraTodos(jugador, piezasDeTuberiaEnEscena, nullptr, nullptr);
        chocarContraTodos(jugador, plataformasPisoEnEscena,nullptr, nullptr);
    }
    for(auto& enemigo:enemigosEnEscena){
        chocarContraTodos(enemigo, objetosEnEscena, nullptr, nullptr);
        chocarContraTodos(enemigo, bloquesEnEscena, nullptr, nullptr);
        chocarContraTodos(enemigo, piezasDeTuberiaEnEscena,nullptr, nullptr);
        chocarContraTodos(enemigo, plataformasPisoEnEscena,nullptr, nullptr);
    }
    for(auto& objeto: objetosEnEscena){
        chocarContraTodos(objeto, bloquesEnEscena, nullptr, nullptr);
        chocarContraTodos(objeto, piezasDeTuberiaEnEscena,nullptr, nullptr);
        chocarContraTodos(objeto, plataformasPisoEnEscena, nullptr, nullptr);
    }
}

void Nivel::resolverGanadores(const std::map<int, Mario *>& mapaJugadores) {
    for(auto const& parClaveJugador:mapaJugadores)
        meta.agregarSiPasoLaMeta(parClaveJugador.second);
}

void Nivel::sacarEnemigosMuertos(){
    std::list<Enemigo*> enemigosABorrar;
    for(auto enemigo : enemigos){
        if(enemigo->sePuedeEliminar()){
            enemigosABorrar.push_back(enemigo);
        }
    }
    for(auto enemigo : enemigosABorrar ){
        enemigos.remove(enemigo);
        delete enemigo;
    }
}

std::string Nivel::obtenerDireccionFondoActual(){
	return direccionFondo;
}

int Nivel::obtenerMundo() const{
    return mundo;
}

void Nivel::agregarPlataforma(std::list<Bloque *> unaPlataforma){
    plataformas.splice(plataformas.end(),unaPlataforma);
}
void Nivel::agregarEnemigo(Enemigo* unEnemigo){
    enemigos.push_back(unEnemigo);
}
void Nivel::agregarMoneda(Moneda* unaMoneda){
    monedas.push_back(unaMoneda);
}
bool Nivel::esUnaPosicionXValidaEnemigo(int numeroPosicion){
	return !posicionesOcupadasXEnemigos[numeroPosicion];
}

bool Nivel::esUnaPosicionValidaMoneda(int numeroPosicionX, int numeroPosicionY){
	return !posicionesOcupadas[std::make_tuple(numeroPosicionX, numeroPosicionY)];
}

void Nivel::inicializar() {
    inicializarPosicionesOcupadasPorBloques();
    inicializarPosicionOcupadasPorTuberias();
    inicializarPosicionMonedas();
    inicializarPosicionEnemigo();
    elevarObstaculos();
    piso.inicializar();
}

void Nivel::elevarObstaculos() {
    for(auto& bloque: plataformas){
        bloque->elevar(piso.obtenerAltura());
    }
    for(auto& tuberia: tuberias){
        tuberia->elevar(piso.obtenerAltura());
    }
    for(auto& moneda: monedas){
        moneda->elevar(piso.obtenerAltura());
    }
}

void Nivel::inicializarPosicionesOcupadasPorBloques(){

    std::list<Bloque *> plataformasARemover;
    for(auto const& bloque : plataformas){
        if((bloque->obtenerPosicionX() >= (int) puntoBanderaFin) || (bloque->obtenerPosicionY() >= ALTO_NIVEL)){
            Log::getInstance()->huboUnError("No se pudo poner un bloque en la posicion X: " + std::to_string(bloque->obtenerPosicionX()) +
                    + " Y: "+std::to_string(bloque->obtenerPosicionX()) +	" se elimina");
            plataformasARemover.push_back(bloque);
        }
        posicionesOcupadas[std::make_tuple(bloque->obtenerPosicionX()/TAMANIO_BLOQUE, bloque->obtenerPosicionY()/TAMANIO_BLOQUE)] = true;
    }

    for(auto const& bloque : plataformasARemover){
        this->plataformas.remove(bloque);
    }
}

void Nivel::inicializarPosicionOcupadasPorTuberias(){
    int posicionesQueOcupaUnaTuberia = 5;
    int posicionXOcupada;
    int posicionYOcupada;
    for(auto const& tuberia : tuberias){
        posicionXOcupada = tuberia->obtenerPosicionX()/TAMANIO_BLOQUE;
        posicionYOcupada = 0;
        for(int i = 0; i < posicionesQueOcupaUnaTuberia; i++) {
            for(int j = 0; j < posicionesQueOcupaUnaTuberia; j++){
                posicionesOcupadas[std::make_tuple(posicionXOcupada+i, posicionYOcupada+j)] = true;
            }
            posicionesOcupadasXEnemigos[posicionXOcupada + i] = true;
        }
    }
}



void Nivel::inicializarPosicionMonedas(){

	int cantidadMaximaMonedas = (int)(puntoBanderaFin/2)/(TAMANIO_MONEDA);

	if(cantidadMonedas > cantidadMaximaMonedas){
		Log::getInstance()->huboUnError("No se pueden poner " + std::to_string(cantidadMonedas) +" monedas, se ponen entonces: " + std::to_string(cantidadMaximaMonedas));
	}

	int numeroPosicionX = 0, numeroPosicionY = 0, coordenadaX = 0, coordenadaY = 0;

	int limiteXSuperior = (int)puntoBanderaFin;
	int limiteXInferior = (int)puntoBanderaFin/10;
	int limiteYInferior = ALTO_NIVEL/6;
	int limiteYSuperior = (ALTO_NIVEL*2)/5;

	for(int i=0; i<cantidadMonedas && i<cantidadMaximaMonedas; i++){

		do{
			numeroPosicionX = rand() % (limiteXSuperior/TAMANIO_MONEDA + 1 - limiteXInferior/TAMANIO_MONEDA) + limiteXInferior/TAMANIO_MONEDA;
			numeroPosicionY = rand() % (limiteYSuperior/TAMANIO_MONEDA + 1 - limiteYInferior/TAMANIO_MONEDA) + limiteYInferior/TAMANIO_MONEDA;
		}while(!this->esUnaPosicionValidaMoneda(numeroPosicionX, numeroPosicionY));

		this->posicionesOcupadas[std::make_tuple(numeroPosicionX, numeroPosicionY)] = true;

		coordenadaX = numeroPosicionX*TAMANIO_MONEDA;
		coordenadaY = numeroPosicionY*TAMANIO_MONEDA;

		this->agregarMoneda(new Moneda(coordenadaX, coordenadaY));

	}

}

void Nivel::inicializarPosicionEnemigo(){

	int numeroPosicionX = 0;

	int limiteXSuperior = (int)puntoBanderaFin;
	int limiteXInferior = (int)puntoBanderaFin/10;

	int coordenadaX = 0;
	int coordenadaY = 100;

	unsigned int cantidadMaximaEnemigos =  (unsigned int)(puntoBanderaFin/3)/TAMANIO_ENEMIGO;

	if(enemigos.size()>=cantidadMaximaEnemigos){
			Log::getInstance()->huboUnError("No se pudieron cargar "+ std::to_string((int)enemigos.size()) +
				" enemigos, se carga la cantidad maxima permitida para este nivel: " + std::to_string((int)cantidadMaximaEnemigos));
			auto iterador1 = enemigos.begin();
			auto iterador2 = enemigos.end();
			advance(iterador1, cantidadMaximaEnemigos-1);
			enemigos.erase(iterador1,iterador2);
	}

	for (auto const& enemigo : enemigos) {
		do{
			numeroPosicionX = rand() % (limiteXSuperior/TAMANIO_ENEMIGO + 1 - limiteXInferior/TAMANIO_ENEMIGO) + limiteXInferior/TAMANIO_ENEMIGO;
		}while(!esUnaPosicionXValidaEnemigo(numeroPosicionX));

		posicionesOcupadasXEnemigos[numeroPosicionX] = true;

		coordenadaX = numeroPosicionX*TAMANIO_ENEMIGO;
		enemigo->agregarPosicion(coordenadaX,coordenadaY);
	}

}

void Nivel::agregarTuberia(int posicionXNuevaTuberia, int tipoTuberia, int colorTuberia) {
    auto* posibleTuberia = new Tuberia(posicionXNuevaTuberia,tipoTuberia,colorTuberia);

    bool superponeAObjeto = false;
    for (auto tuberia:tuberias){
        if(tuberia->colisionaCon(posibleTuberia)){
            superponeAObjeto = true;
        }
    }

    if(!superponeAObjeto){
        tuberias.push_back(posibleTuberia);
    }
    else{
        delete posibleTuberia;
    }
}

void Nivel::completarInformacionRonda(info_ronda_t *ptrInfoRonda, bool (* deboAgregarlo)(void*, rectangulo_t), void* contexto) {
    if(!ptrInfoRonda) return;

    ptrInfoRonda->tiempoFaltante = contador.tiempoRestante();

    serializador.serializar(plataformas,ptrInfoRonda,deboAgregarlo,contexto);
    serializador.serializar(enemigos,ptrInfoRonda,deboAgregarlo,contexto);
    serializador.serializar(monedas,ptrInfoRonda,deboAgregarlo,contexto);
    serializador.serializar(objetosFugaces,ptrInfoRonda,deboAgregarlo,contexto);
}

void Nivel::agregarPozo(int posicionX, int tipoPozo, int fondo) {
    piso.agregarPozo(posicionX, tipoPozo, fondo);
    for(int i = 0; i<5; i++){
        posicionesOcupadasXEnemigos[posicionX/TAMANIO_ENEMIGO + i] = true;
    }
}

void Nivel::terminar(const std::map<int, Mario *>& jugadores) {
    meta.sumarPuntos(contador.tiempoRestante());

    for(auto const& parJugador:jugadores){
        parJugador.second->eliminar(&podio);
    }
}

bool Nivel::todosEnLaMeta(const std::map<int, Mario *>& jugadores) {
    return meta.todosEnLaMeta(jugadores);
}

void Nivel::completarInformacionNivel(nivel_t *nivel) {
    nivel->mundo = mundo;
    serializador.serializar(tuberias,nivel);
    std::list<entidad_t> pozos = piso.serializar();
    serializador.serializar(pozos,nivel);
    serializador.serializar(plataformas,nivel);
}

Nivel::~Nivel (){
    for(const auto& plataforma:plataformas){
        delete plataforma;
    }
    for(const auto& moneda:monedas){
        delete moneda;
    }
    for(const auto& enemigo:enemigos){
        delete enemigo;
    }
    for(const auto& disparo:objetosFugaces){
        delete disparo;
    }
    objetosFugaces.clear();
    plataformas.clear();
    enemigos.clear();
    monedas.clear();

}

void Nivel::iniciar(const std::map<int, Mario*>& jugadores) {
    contador.iniciar();
    podio.recibirJugadores(jugadores);
    for(auto const& parJugador:jugadores){
        parJugador.second->agregar(&podio);
    }
}

Podio* Nivel::obtenerPodio(){
    return &podio;
}

int Nivel::tiempoRestante() {
    return contador.tiempoRestante();
}

void Nivel::aparecerDisparo(ObjetoFugaz* disparo) {
    objetosFugaces.push_back(disparo);
}

void Nivel::agregarObjeto_helper(void* ptr_jugador, void *ptr_bloque, void *ptr_nivel) {
    ((Nivel *) ptr_nivel)->utilizarSorpresa((Mario *) ptr_jugador, (Bloque *) ptr_bloque);
}

void Nivel::utilizarSorpresa(Mario* jugador, Bloque *bloque) {
    ObjetoSorpresa* objeto = bloque->colisionaronAbajo();
    objeto->usarse(jugador);
    if(objeto->debeDesaparecer()){
        delete objeto;
    }
    else{
        objetosFugaces.push_front(objeto);
    }
}


void Nivel::buscarBloqueParaCaer(rectangulo_t rectanguloEscena, PosicionFija* pos) {
    int xActual = rectanguloEscena.x1;
    int yActual = rectanguloEscena.y2;
    bool hayCandidato = false;

    for(auto const& bloque : plataformas){
        rectangulo_t rectBloque = bloque->obtenerRectangulo();
        rectangulo_t interseccion{};
        bool hayInterseccion = intersecarRectangulos(rectanguloEscena, rectBloque, &interseccion);
        if(hayInterseccion && interseccion.w >= ANCHO_MARIO){
            if(!hayCandidato){
                xActual = rectBloque.x1;
                yActual = rectBloque.y2;
                hayCandidato = true;
            }else if(rectBloque.x1 < xActual ||
                    (rectBloque.x1 == xActual && rectBloque.y2 > yActual)){
                xActual = rectBloque.x1;
                yActual = rectBloque.y2;
            }
        }
    }

    *pos = PosicionFija(xActual, yActual);
}


void Nivel::imponerPosicionDeReaparicion(const std::map<int, Mario*>& jugadores, rectangulo_t rectanguloEscena) {
    PosicionFija posicionDeReaparicion(rectanguloEscena.x1, piso.obtenerAltura());

    bool hayPiso = piso.obtenerRespawn(rectanguloEscena, &posicionDeReaparicion);
    if(!hayPiso) {
        buscarBloqueParaCaer(rectanguloEscena, &posicionDeReaparicion);
    }else{
        buscarBloqueMasAlto(&posicionDeReaparicion);
    }

    for(auto& parClaveJugador : jugadores){
        Mario* jugador = parClaveJugador.second;
        jugador->nuevoPuntoDeReaparicion(posicionDeReaparicion);
    }
}

void Nivel::buscarBloqueMasAlto(PosicionFija* posicion) {
    int xACaer = posicion->obtenerPosX();
    int yACaer = posicion->obtenerPosY();
    for(auto const& bloque : plataformas){
        int xBloque = bloque->obtenerPosicionX();
        int yTechoBloque = bloque->obtenerPosicionY() + LARGO_BLOQUE;
        if(xACaer <= xBloque + LARGO_BLOQUE &&
           xACaer + ANCHO_MARIO >= xBloque &&
           yTechoBloque > yACaer){
            yACaer = yTechoBloque;
        }
    }

    for(auto const& tuberia: tuberias){
        std::list<PiezaDeTuberia*> piezas = tuberia->obtenerPiezas();
        for(auto const& pieza: piezas){
            rectangulo_t rectanguloPieza = pieza->obtenerRectangulo();
            if(xACaer <= rectanguloPieza.x2 &&
               xACaer + ANCHO_MARIO >= rectanguloPieza.x1 &&
               rectanguloPieza.y2 > yACaer){
                yACaer = rectanguloPieza.y2;
            }
        }
    }

    *posicion = PosicionFija(xACaer, yACaer);
}



