#ifndef SRC_MODELO_ENEMIGOS_GOOMBA_HP_
#define SRC_MODELO_ENEMIGOS_GOOMBA_HP_

#include "Enemigo.hpp"
#include "src/Utils/Utils.hpp"

class Goomba : public Enemigo{
	public:
		explicit Goomba(int tipoColor);
        entidad_t serializar() override;
		~Goomba() override;
        std::string obtenerColisionID() override;
};

#endif /* SRC_MODELO_ENEMIGOS_GOOMBA_HP_ */
