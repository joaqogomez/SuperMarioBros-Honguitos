#ifndef SRC_MODELO_JUEGO_HPP_
#define SRC_MODELO_JUEGO_HPP_

using namespace std;
#include "Nivel.hpp"
#include <list>
#include <iostream>
#include <string>

class Mario;
#include "Mario.hpp"

class Juego{

	private:

		void inicializar(int cantJugadores);

		Juego(list<Nivel*> nivelesLector, int cantJugadores){

			inicializar(cantJugadores);
			niveles = nivelesLector;

			for (auto const& nivel : niveles) {
				nivel->inicializarPosicionesOcupadasPorBloques();
				nivel->inicializarPosicionMonedas();
				nivel->inicializarPosicionEnemigo();
			}
		}

		list<Mario*> jugadores;
		list<Nivel*> niveles;

		static Juego* instanciaJuego;
	public:
		Juego(Juego &other) = delete;
		static Juego* getInstance();
		static Juego* getInstance(list<Nivel*> archivoLeido,int cantJugadores);

		void avanzarNivel();
		void actualizarPosicionesEnemigos();
		void sumarPuntosAJugadores(int puntos);
		bool quedaSoloUnNivel();

		string obtenerDireccionFondoNivelActual();
		list<Enemigo*> obtenerEnemigos();
		list<Plataforma*> obtenerPlataformas();
		list<Moneda*> obtenerMonedas();
		list<Mario*> obtenerMarios();
		int obtenerTiempoDelNivel();
		int obtenerPuntuacionJugador();
		int obtenerMundoActual();
		int obtenerPuntoBanderaFinActual();

		~Juego();

};



#endif /* SRC_MODELO_JUEGO_HPP_ */
