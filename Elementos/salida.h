//
// Created by julen on 25/08/24.
//

#ifndef SALIDA_H
#define SALIDA_H

#include "../io.h"
#include "../Wrapper/image.h"
#include "../raton.h"
#include "simulable.h"

class IO;
class IMG;
class Raton;
class SistemaGuardado;

class Salida final : public Simulable
{
    friend class SistemaGuardado;

    IO* entrada{};

    IMG img;

    std::pair<int, int> linea{25, 25};

    bool mover{false};
    Raton* raton;

    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + 1<<29;
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

public:

    Salida(SDL_Renderer* renderer, const int x, const int y, Raton* raton_)
        :Salida(renderer, x, y, raton_, idGenerator())
    {}

    Salida(SDL_Renderer* renderer, const int x, const int y, Raton* raton_, const unsigned int id_)
        :Simulable{id_}, img{"./img/salida/apagado.png", renderer, x, y}, raton{raton_}
    {
        img.setClickable(this);
        linea.first += x;
        linea.second += y;
    }

    ~Salida() override = default;

    void setEntrada(IO* entrada_) { entrada = entrada_; }
    [[nodiscard]] IO* getEntrada() const { return entrada; }
    [[nodiscard]] IMG* getImg() { return &img; }
    [[nodiscard]] std::pair<int, int>* getLineaPos() { return &linea; }
    [[nodiscard]] bool get() const { return entrada->get(); }
    [[nodiscard]] bool getDesconectado() const { return entrada == nullptr; }

    void simular() override;
    void actualizar() override {}
    void simularAntiguo();

    void click(int x, int y, Raton::Evento evento);
    void moverRel(int x, int y);
};

#endif //SALIDA_H