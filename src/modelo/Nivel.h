#ifndef SRC_MODELO_NIVEL_H_
#define SRC_MODELO_NIVEL_H_

#include "Bloques/Bloque.h"
#include "Enemigos/Enemigo.h"
#include "Moneda.h"
#include <map>
#include <string>
#include<tuple>
#include <cstdlib>
using namespace std;


class Nivel{


	public:
		Nivel(int mundo,string direccionFondo,int tiempo,int cantidadMonedas,int puntoBanderaFin){
			this->mundo = mundo;
			this->direccionFondo = direccionFondo;
			this->tiempo = tiempo;
			this->cantidadMonedas = cantidadMonedas;
			this->puntoBanderaFin = puntoBanderaFin;

			this->altoNivel = 600;

		}

		void agregarPlataforma(Plataforma* unaPlataforma){
			plataformas.push_back(unaPlataforma);
		}

		void agregarEnemigo(Enemigo* unEnemigo){
			enemigos.push_back(unEnemigo);
		}

		void agregarMoneda(Moneda* unaMoneda){
			monedas.push_back(unaMoneda);
		}

		int obtenerTiempo(){
			return tiempo;
		}

		int obtenerMundo(){
			return mundo;
		}


		void inicializarPosicionEnemigo();
		void inicializarPosicionMonedas();

		void actualizarPosicionesEnemigos();

		void inicializarPosicionesOcupadasPorBloques();

		list<Enemigo*> obtenerEnemigos();
		list<Plataforma*> obtenerPlataformas();
		list<Moneda*> obtenerMonedas();
		string obtenerDireccionFondoActual();

		~Nivel(){
			plataformas.clear();
			enemigos.clear();
			monedas.clear();
		}


	private:

		bool esUnaPosicionXValidaEnemigo(int coordenadaX);

		bool esUnaPosicionValidaMoneda(int numeroPosicionX, int numeroPosicionY);

		map<int, bool> posicionesOcupadasXEnemigos;

		map<tuple<int, int>, bool> posicionesOcupadas;

		list<Plataforma*> plataformas;
		list<Enemigo*> enemigos;
		list<Moneda*> monedas;

		int altoNivel;

		int mundo;
		string direccionFondo;
		int tiempo;
		int cantidadMonedas;
		int puntoBanderaFin;
};



#endif /* SRC_MODELO_NIVEL_H_ */
