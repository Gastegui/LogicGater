//
// Created by julen on 22/08/24.
//

#ifndef GATE_H
#define GATE_H

#include "../Wrapper/image.h"
#include "../raton.h"
#include "../io.h"
#include "simulable.h"

class Window;
class SistemaGuardado;
class Controlador;

class Puerta final : public Simulable
{
    friend class SistemaGuardado;
    Controlador* controlador;
    bool arribaNegado;
    bool abajoNegado;
    bool salidaNegada;

    bool siguiente{false};
    bool simulandoAntiguo{false};

    IO* arriba{nullptr};
    IO* abajo{nullptr};
    IO salida;

    std::pair<int, int> lineaArriba{0, 11};
    std::pair<int, int> lineaAbajo{0, 37};

    bool interactuarAbajo{false};
    bool seleccionado{false};

    static auto idGenerator() -> unsigned int
    {
        static unsigned int id = 1 + getOffset(ID_TIPOS::Puerta);
        return id++;
    }

    static auto gastarIds(const unsigned int cantidad) -> void
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

    bool mover{false};

    Raton* raton;

public:
    enum TIPO : uint8_t
    {
        AND,
        OR,
        XOR
    };

private:
    TIPO tipo{};
    IMG imagen;

public:
    Puerta(SDL_Renderer* renderer, Controlador* controlador_, const TIPO tipo_, const int x_, const int y_, const bool arribaNegado_, const bool abajoNegado_, const bool salidaNegada_, Raton* raton_)
        : Puerta(renderer, controlador_, tipo_, x_, y_, arribaNegado_, abajoNegado_, salidaNegada_, raton_, idGenerator()) {}

    Puerta(SDL_Renderer* renderer, Controlador* controlador_, const TIPO tipo_, const int x_, const int y_, const bool arribaNegado_, const bool abajoNegado_, const bool salidaNegada_, Raton* raton_, const unsigned int id_)
        : Simulable(id_), controlador(controlador_), arribaNegado{arribaNegado_}, abajoNegado{abajoNegado_}, salidaNegada{salidaNegada_}, salida(this, 100 + x_, 25 + y_), raton{raton_}, tipo{tipo_}, imagen{renderer, x_, y_, 100, 50}
    {
        imagen.crearImagen<IMG::Capa>(
            IMG::Capa{.img = arribaNegado_ ? "./img/puertas/entrada_arriba_negada.png" : "./img/puertas/entrada_arriba_normal.png"},
            IMG::Capa{.img = abajoNegado_ ? "./img/puertas/entrada_abajo_negada.png" : "./img/puertas/entrada_abajo_normal.png"},
            IMG::Capa{.img = tipo_ == AND ? "./img/puertas/and.png" : tipo_ == OR ? "./img/puertas/or.png" : "./img/puertas/xor.png"},
            IMG::Capa{.img = salidaNegada_ ? "./img/puertas/salida_negada.png" : "./img/puertas/salida_normal.png"},
            IMG::Capa{.img = seleccionado ? "./img/puertas/seleccionado.png" : "./img/puertas/no_seleccionado.png"}
        );
        imagen.setClickable(this);
        lineaArriba.first += x_;
        lineaArriba.second += y_;
        lineaAbajo.first += x_;
        lineaAbajo.second += y_;
    }

    [[nodiscard]] auto getSalida() -> IO* { return &salida; }
    [[nodiscard]] auto getDesconectado() const -> bool override { return arriba == nullptr && abajo == nullptr && salida.getConexiones() == 0; }
    [[nodiscard]] auto getImg() -> IMG* override { return &imagen; }
    [[nodiscard]] auto getArribaNegado() const -> bool { return arribaNegado; }
    [[nodiscard]] auto getAbajoNegado() const -> bool { return abajoNegado; }
    [[nodiscard]] auto getSalidaNegada() const -> bool { return salidaNegada; }
    [[nodiscard]] auto getTipoPuerta() const -> TIPO { return tipo; }

    auto cambiar() -> bool
    {
        return imagen.crearImagen<IMG::Capa>(
            IMG::Capa{.img = arribaNegado ? "./img/puertas/entrada_arriba_negada.png" : "./img/puertas/entrada_arriba_normal.png"},
            IMG::Capa{.img = abajoNegado ? "./img/puertas/entrada_abajo_negada.png" : "./img/puertas/entrada_abajo_normal.png"},
            IMG::Capa{.img = tipo == AND ? "./img/puertas/and.png" : tipo == OR ? "./img/puertas/or.png" : "./img/puertas/xor.png"},
            IMG::Capa{.img = salidaNegada ? "./img/puertas/salida_negada.png" : "./img/puertas/salida_normal.png"},
            IMG::Capa{.img = seleccionado ? "./img/puertas/seleccionado.png" : "./img/puertas/no_seleccionado.png"}
        );
    }


    auto simular() -> void override;
    auto actualizar() -> void override;
    auto simularAntiguo() -> bool;
    auto simulacionAntiguaTermindada() -> void;
    auto moverRel(int x_, int y_) -> void;
    auto interactuar(int posX, int posY, INTERACCIONES interaccion) -> bool override;
    auto setIONull(IO* io) -> void override;
    [[nodiscard]] auto getIOSalida() -> IO* override { return &salida; }

    [[nodiscard]] auto getLinea(const int conexion) -> std::pair<int, int>* override
    {
        if(conexion == 1)
            return &lineaArriba;
        if(conexion == 2)
            return &lineaAbajo;
        return nullptr;
    }

    [[nodiscard]] auto getConexion(IO* io) const -> int override
    {
        if(arriba != nullptr && arriba == io)
            return 1;
        if(abajo != nullptr && abajo == io)
            return 2;

        return 0;
    }

    auto seleccionar(const bool estado) -> void override
    {
        if(estado != seleccionado)
        {
            seleccionado = estado;
            cambiar();
        }
        else
            seleccionado = estado;
    }
};

#endif //GATE_H
