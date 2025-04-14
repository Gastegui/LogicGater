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
    Controlador* controlador;

    IO* entrada{};

    IMG img;

    std::pair<int, int> entradaPos{25, 25};

    bool mover{false};
    Raton* raton;

    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + get_offset(ID_TIPOS::Salida);
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

public:

    Salida(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_)
        :Salida(renderer, controlador_, x, y, raton_, idGenerator())
    {}

    Salida(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const unsigned int id_)
        :Simulable{id_}, controlador{controlador_}, img{"./img/salida/apagado.png", renderer, x, y}, raton{raton_}
    {
        img.setClickable(this);
        entradaPos.first += x;
        entradaPos.second += y;
    }

    ~Salida() override = default;

    void setEntrada(IO* entrada_) { entrada = entrada_; }
    [[nodiscard]] IO* getEntrada() const { return entrada; }
    [[nodiscard]] IMG* getImg() override { return &img; }
    [[nodiscard]] bool get() const { return entrada->get(); }
    [[nodiscard]] bool getDesconectado() const override { return entrada == nullptr; }

    void simular() override;
    void actualizar() override {}
    void simularAntiguo();

    void moverRel(int x, int y);
    bool interactuar(int posX, int posY, INTERACCIONES interaccion) override;
    void setIONull(IO* io) override;
    [[nodiscard]] IO* getIOSalida() override { return nullptr; }
    [[nodiscard]] std::pair<int, int>* getLinea(const int conexion) override
    {
        if(entrada == nullptr)
            return nullptr;
        if(conexion == 1)
            return &entradaPos;

        return nullptr;
    }
};

#endif //SALIDA_H