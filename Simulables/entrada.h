//
// Created by julen on 25/08/24.
//

#ifndef BOTON_H
#define BOTON_H

#include "../io.h"
#include "../Wrapper/image.h"
#include "../raton.h"
#include "simulable.h"

class IO;
class IMG;
class Raton;
class SistemaGuardado;

class Entrada final : public Simulable
{
    friend class SistemaGuardado;
    Controlador* controlador;

    IO salida;
    bool mantener;
    IMG img;
    bool mover{false};
    bool interactuarAbajo{false};
    bool seleccionado{false};

    //std::pair<int, int> linea{25, 25};
    Raton* raton;

    static auto idGenerator() -> unsigned int
    {
        static unsigned int id = 1 + getOffset(ID_TIPOS::Entrada);
        return id++;
    }

    static auto gastarIds(const unsigned int cantidad) -> void
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

    auto getImgPath() -> std::string
    {
        if(mantener)
        {
            if(salida.get())
                return "./img/entrada/boton_encendido.png";
            return "./img/entrada/boton_apagado.png";
        }
        //else
        if(salida.get())
            return "./img/entrada/interruptor_encendido.png";
        return "./img/entrada/interruptor_apagado.png";
    }

    auto cambiar() -> void
    {
        img.crearImagen<IMG::Capa>(
            IMG::Capa{.img = getImgPath()},
            IMG::Capa{.img = seleccionado ? "./img/entrada/seleccionado.png" : "./img/entrada/no_seleccionado.png"}
        );
    }

public:
    Entrada(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const bool mantener_ = false)
        : Entrada(renderer, controlador_, x, y, raton_, idGenerator(), mantener_) {}

    Entrada(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const unsigned int id_, const bool mantener_ = false)
        : Simulable{id_}, controlador{controlador_}, salida{this, x + 50, y + 25}, mantener{mantener_}, img{getImgPath().c_str(), renderer, x, y}, raton{raton_}
    {
        img.setClickable(this);
    }

    [[nodiscard]] auto getSalida() -> IO* { return &salida; }
    [[nodiscard]] auto getImg() -> IMG* override { return &img; }
    [[nodiscard]] auto getDesconectado() const -> bool override { return salida.getConexiones() == 0; }
    [[nodiscard]] auto getMantener() const -> bool { return mantener; }
    auto setMantener(const bool mantener_) -> void { mantener = mantener_; }

    [[nodiscard]] auto get() const -> bool { return salida.get(); }

    auto set(const bool valor) -> void
    {
        if(salida.get() != valor)
        {
            salida.set(valor);
            cambiar();
        }
    }

    auto moverRel(int x, int y) -> void;

    auto simular() -> void override {}
    auto actualizar() -> void override {}
    auto interactuar(int posX, int posY, INTERACCIONES interaccion) -> bool override;
    auto setIONull(IO* io) -> void override {}
    [[nodiscard]] auto getIOSalida() -> IO* override { return &salida; }
    [[nodiscard]] auto getLinea([[maybe_unused]] int conexion) -> std::pair<int, int>* override { return nullptr; }
    [[nodiscard]] auto getConexion([[maybe_unused]] IO* io) const -> int override { return 0; }

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


#endif //BOTON_H
