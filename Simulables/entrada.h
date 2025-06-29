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
        static unsigned int id = 1 + get_offset(ID_TIPOS::Entrada);
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

    void cambiar()
    {
        img.crearImagen<IMG::Capa>(
            IMG::Capa{.img=salida.get() ? "./img/entrada/encendido.png" : "./img/entrada/apagado.png"},
            IMG::Capa{.img=seleccionado ? "./img/entrada/seleccionado.png" : "./img/entrada/no_seleccionado.png"}
        );
    }

public:

    Entrada(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const bool mantener_ = false)
        :Entrada(renderer, controlador_, x, y, raton_, idGenerator(), mantener_)
    {}

    Entrada(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const unsigned int id_, const bool mantener_ = false)
        :Simulable{id_}, controlador{controlador_}, salida{this, x + 25, y + 25}, mantener{mantener_}, img{"./img/entrada/apagado.png", renderer, x, y}, raton{raton_}
    {
        img.setClickable(this);
    }

    ~Entrada() override = default;

    [[nodiscard]] auto getSalida() -> IO* { return &salida; }
    [[nodiscard]] auto getImg() -> IMG* override { return &img; }
    [[nodiscard]] auto getDesconectado() const -> bool override { return salida.getConexiones() == 0; }
    [[nodiscard]] auto getMantener() const -> bool { return mantener; }
    void setMantener(const bool mantener_) { mantener = mantener_; }

    [[nodiscard]] auto get() const -> bool { return salida.get(); }
    void set(const bool valor)
    {
        if(salida.get() != valor)
        {
            cambiar();
            salida.set(valor);
        }
    }

    void moverRel(int x, int y);

    void simular() override {}
    void actualizar() override {}
    auto interactuar(int posX, int posY, INTERACCIONES interaccion) -> bool override;
    void setIONull(IO* io) override {}
    [[nodiscard]] auto getIOSalida() -> IO* override { return &salida; }
    [[nodiscard]] auto getLinea( [[maybe_unused]] int conexion) -> std::pair<int, int>* override { return nullptr; }
    [[nodiscard]] auto getConexion( [[maybe_unused]] IO* io) const -> int override { return 0; }
    void seleccionar(bool estado) override
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
