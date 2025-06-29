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

    bool ultimoEstado{false};
    bool seleccionado{false};

    static auto idGenerator() -> unsigned int
    {
        static unsigned int id = 1 + get_offset(ID_TIPOS::Salida);
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

    void cambiar(bool estado);

public:
    Salida(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_)
        : Salida(renderer, controlador_, x, y, raton_, idGenerator()) {}

    Salida(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const unsigned int id_)
        : Simulable{id_}, controlador{controlador_}, img{"./img/salida/apagado.png", renderer, x, y}, raton{raton_}
    {
        img.setClickable(this);
        entradaPos.first += x;
        entradaPos.second += y;
    }

    ~Salida() override = default;

    void setEntrada(IO* entrada_) { entrada = entrada_; }
    [[nodiscard]] auto getEntrada() const -> IO* { return entrada; }
    [[nodiscard]] auto getImg() -> IMG* override { return &img; }
    [[nodiscard]] auto get() const -> bool { return entrada->get(); }
    [[nodiscard]] auto getDesconectado() const -> bool override { return entrada == nullptr; }

    void simular() override;
    void actualizar() override {}
    void simularAntiguo();

    void moverRel(int x, int y);
    auto interactuar(int posX, int posY, INTERACCIONES interaccion) -> bool override;
    void setIONull(IO* io) override;
    [[nodiscard]] auto getIOSalida() -> IO* override { return nullptr; }

    [[nodiscard]] auto getLinea(const int conexion) -> std::pair<int, int>* override
    {
        if(conexion == 1)
            return &entradaPos;

        return nullptr;
    }

    [[nodiscard]] auto getConexion(IO* io) const -> int override
    {
        if(entrada != nullptr && entrada == io)
            return 1;
        return 0;
    }

    void seleccionar(bool estado) override
    {
        if(estado != seleccionado)
        {
            seleccionado = estado;
            cambiar(entrada != nullptr ? entrada->get() : false);
        }
        else
            seleccionado = estado;
    }
};

#endif //SALIDA_H
