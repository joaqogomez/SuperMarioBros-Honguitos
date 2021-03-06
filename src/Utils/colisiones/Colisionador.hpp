
#ifndef TP_TALLER_DE_PROGRAMACION_FIUBA_COLISIONADOR_HPP
#define TP_TALLER_DE_PROGRAMACION_FIUBA_COLISIONADOR_HPP

#include <list>
#include <string>
#include "src/Utils/Utils.hpp"

class Colisionable;


#define SIN_CHOQUE 0
#define PRIMERO_DERECHA_SEGUNDO_IZQUIERDA 1
#define PRIMERO_IZQUIERDA_SEGUNDO_DERECHA 2
#define PRIMERO_ARRIBA_SEGUNDO_ABAJO 3
#define PRIMERO_ABAJO_SEGUNDO_ARRIBA 4


typedef void(*FuncionLuegoDeColisionar)(void* colisionable1, void* colisionable2, void* contexto);

void chocar(Colisionable* colisionable1, Colisionable* colisionable2);
void chocarContraTodos(Colisionable* colisionable, std::list<Colisionable*> colisionables, FuncionLuegoDeColisionar funcion, void* contexto);
int tipoDeChoque(rectangulo_t rectangulo1, rectangulo_t rectangulo2);
bool esUnBloque(const std::string& idColisionable);
bool colisionan(rectangulo_t rect1, rectangulo_t rect2);
bool intersecarRectangulos(rectangulo_t rect1, rectangulo_t rect2, rectangulo_t* rectInterseccion);

#endif //TP_TALLER_DE_PROGRAMACION_FIUBA_COLISIONADOR_HPP
