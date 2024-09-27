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

class Entrada
{
    IO salida{this};
    bool mantener;
    IMG img;
    bool mover{false};

    std::pair<int, int> linea{25, 25};

public:

    Entrada(SDL_Renderer* renderer, const int x, const int y, bool mantener_ = false)
        :mantener{mantener_}, img{"./img/entrada/apagado.png", renderer, x, y}
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
    void setMantener(const bool mantener_) { mantener = mantener_; }

    [[nodiscard]] bool get() const { return salida.get(); }
    void set(const bool valor)
    {
        img.cambiarImagen(valor ? "./img/entrada/encendido.png" : "./img/entrada/apagado.png");
        salida.set(valor);
    }

    void click(int x, int y, Raton::Evento evento);
};



#endif //BOTON_H
