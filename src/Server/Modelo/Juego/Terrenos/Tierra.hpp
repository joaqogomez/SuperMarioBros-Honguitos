#ifndef SRC_MODELO_MOVIMIENTO_TERRENO_TIERRA_H_
#define SRC_MODELO_MOVIMIENTO_TERRENO_TIERRA_H_

#include "Terreno.hpp"

class Tierra : public Terreno{

	public:
		Tierra();
		float aplicarCoeficienteDeRozamiento(float velocidadX) override;
		float obtenerImpulsoHorizontal(float aceleracion) override;
		float obtenerImpulsoVertical(float fuerza) override;
        float amortiguarVelocidad(float velocidadY) override;

};

#endif /* SRC_MODELO_MOVIMIENTO_TERRENO_TIERRA_H_ */
