//
// Created by julen on 25/08/24.
//

#ifndef BOTON_H
#define BOTON_H

#include "io.h"
#include "Wrapper/image.h"
#include "raton.h"

class IO;
class IMG;
class Raton;
class SistemaGuardado;

class Entrada
{
    friend class SistemaGuardado;

    IO salida{this};
    bool mantener;
    IMG img;
    bool mover{false};

    std::pair<int, int> linea{25, 25};
    Raton* raton;

    unsigned int id;

    static unsigned int idGenerator()
    {
        static unsigned int id = 1;
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

public:

    Entrada(SDL_Renderer* renderer, const int x, const int y, Raton* raton_, const bool mantener_ = false)
        :Entrada(renderer, x, y, raton_, idGenerator(), mantener_)
    {}

    Entrada(SDL_Renderer* renderer, const int x, const int y, Raton* raton_, const unsigned int id_, const bool mantener_ = false)
        :mantener{mantener_}, img{"./img/entrada/apagado.png", renderer, x, y}, raton{raton_}, id{id_}
    {
        img.setClickable(this);
        linea.first += x;
        linea.second += y;
    }

    [[nodiscard]] IO* getSalida() { return &salida; }
    [[nodiscard]] IMG* getImg() { return &img; }
    [[nodiscard]] std::pair<int, int>* getLineaPos() { return &linea; }
    [[nodiscard]] bool getDesconectado() const { return salida.getConexiones() == 0; }
    [[nodiscard]] bool getMantener() const { return mantener; }
    [[nodiscard]] unsigned int getId() const { return id; }
    void setMantener(const bool mantener_) { mantener = mantener_; }

    [[nodiscard]] bool get() const { return salida.get(); }
    void set(const bool valor)
    {
        img.cambiarImagen(valor ? "./img/entrada/encendido.png" : "./img/entrada/apagado.png");
        salida.set(valor);
    }

    void click(int x, int y, Raton::Evento evento);
    void moverRel(int x, int y);
};



#endif //BOTON_H
