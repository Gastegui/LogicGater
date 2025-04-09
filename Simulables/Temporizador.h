//
// Created by julen on 28/01/25.
//

#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H
#include "simulable.h"
#include "../io.h"
#include "../Wrapper/image.h"


class Temporizador final : public Simulable
{
    friend class SistemaGuardado;

    IO* entrada{};
    IO salida{this};

    std::pair<int, int> lineaSalida{100, 50};
    std::pair<int, int> lineaEntrada{0, 50};

    int x;
    int y;

    bool mover{false};
    Raton* raton;

    int duracion;
    int actual{0};

    IMG imagen;

    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + 1<<31;
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

public:

    Temporizador(SDL_Renderer* renderer, const int x_, const int y_, const int duracion_, Raton* raton)
        : Temporizador(renderer, x_, y_, duracion_, raton, idGenerator())
    {}

    Temporizador(SDL_Renderer* renderer, const int x_, const int y_, const int duracion_, Raton* raton, const unsigned int id)
        : Simulable(id), x(x_), y(y_), raton(raton), duracion{duracion_},
            imagen{"./img/temporizador/0.png", renderer, x_, y_}
    {}

    ~Temporizador() override = default;

    void simular() override;
    void actualizar() override;
    bool simularAntiguo();

    void click(int x, int y, Raton::Evento evento) override;
    void moverRel(int x_, int y_);

};



#endif //TEMPORIZADOR_H
