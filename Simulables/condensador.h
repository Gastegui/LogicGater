//
// Created by julen on 11/9/25.
//

#ifndef LOGICGATER_CONDENSADOR_H
#define LOGICGATER_CONDENSADOR_H
#include <vector>
#include "../io.h"
#include "../Wrapper/image.h"
#include "../controlador.h"

#include "simulable.h"

class Condensador final : public Simulable
{
    static auto idGenerator() -> unsigned int
    {
        static unsigned int id = 1 + getOffset(ID_TIPOS::Condensador);
        return id++;
    }

    static auto gastarIds(const unsigned int cantidad) -> void
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

    std::vector<IO*> entradas;
    IO salida;
    IMG imagen;
    Controlador* controlador;
    Raton* raton;
    Puerta::TIPO tipo;

    bool siguiente{false};
    bool seleccionado{false};
    bool mover{false};

    std::pair<int, int> linea{0,25};


    auto cambiar() -> bool
    {
        return imagen.crearImagen<IMG::Capa>(
            IMG::Capa{.img = "./img/condensador/base.png"},
            IMG::Capa{.img = tipo == Puerta::AND ? "./img/condensador/and.png" : "./img/condensador/or.png"},
            IMG::Capa{.img = seleccionado ? "./img/condensador/seleccionado.png" : "./img/condensador/no_seleccionado.png"}
        );
    }

    auto moverRel(int x, int y) -> void;

public:

    Condensador(SDL_Renderer* renderer, Controlador* controlador_, Raton* raton_, Puerta::TIPO tipo_, int x_, int y_)
        :Condensador(renderer, controlador_, raton_, tipo_, x_, y_, idGenerator())
    {}

    Condensador(SDL_Renderer* renderer, Controlador* controlador_, Raton* raton_, Puerta::TIPO tipo_, int x_, int y_, unsigned int id_)
        :Simulable(id_), controlador(controlador_), raton(raton_), tipo(tipo_), salida(this, 100 + x_, 25 + y_), imagen{renderer, x_, y_, 100, 50}
    {
        cambiar();
        imagen.setClickable(this);
        linea.first += x_;
        linea.second += y_;
    }

    auto simular() -> void override;
    auto actualizar() -> void override;
    auto interactuar(int posX, int posY, INTERACCIONES accion) -> bool override;
    [[nodiscard]] auto getImg() -> IMG* override { return &imagen; }
    auto setIONull(IO *io) -> void override;
    auto seleccionar(bool estado) -> void override 
    {
        if(seleccionado != estado)
        {
            seleccionado = estado;
            cambiar();
        }
    }
    [[nodiscard]] auto getIOSalida() -> IO* override { return &salida; }
    [[nodiscard]] auto getLinea(int /*conexion*/) -> std::pair<int, int>* override { return &linea; }
    [[nodiscard]] auto getDesconectado() const -> bool override { return entradas.empty() && salida.getConexiones() == 0; }
    [[nodiscard]] auto getConexion(IO *io) const -> int override { return std::ranges::find(entradas.begin(),entradas.end(), io) == entradas.end() ? 0 : 1; }
    [[nodiscard]] auto getTipoPuerta() const -> Puerta::TIPO { return tipo; }
};


#endif // LOGICGATER_CONDENSADOR_H
