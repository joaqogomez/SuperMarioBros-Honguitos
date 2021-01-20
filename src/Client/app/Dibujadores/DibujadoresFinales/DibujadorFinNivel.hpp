
#ifndef TP_TALLER_DE_PROGRAMACION_FIUBA_DIBUJADORFINNIVEL_HPP
#define TP_TALLER_DE_PROGRAMACION_FIUBA_DIBUJADORFINNIVEL_HPP
#include "src/Client/app/Dibujadores/Dibujador.hpp"


#include "src/Client/app/Dibujadores/Dibujador.hpp"
#include "src/Client/app/CargadorTexturas.hpp"

#include "src/Client/app/juegoCliente/JuegoCliente.hpp"

class DibujadorFinNivel : public Dibujador{
    public:
        DibujadorFinNivel(CargadorTexturas* cargadorTexturas,SDL_Renderer* renderizador, int ancho_pantalla,int alto_pantalla);
        void dibujar(JuegoCliente* juegoCliente);
        ~DibujadorFinNivel() override{};
    private:
        void dibujarTextoFinNivel(JuegoCliente *juegoCliente);
        map<int, SDL_Color> colores;
        int ciclosDibujado;
};

#endif //TP_TALLER_DE_PROGRAMACION_FIUBA_DIBUJADORFINNIVEL_HPP