//
// Created by julen on 25/08/24.
//

#ifndef SALIDA_H
#define SALIDA_H

#include "io.h"
#include "Wrapper/image.h"
#include "raton.h"

class IO;
class IMG;
class Raton;


class Salida
{
    IO* entrada{};

    IMG img;

    std::pair<int, int> linea{25, 25};

    bool mover{false};
public:

    Salida(SDL_Renderer* renderer, const int x, const int y)
        :img{"./img/salida/apagado.png", renderer, x, y}
    {
        img.setClickable(this);
        linea.first += x;
        linea.second += y;
    }

    void setEntrada(IO* entrada_) { entrada = entrada_; }
    [[nodiscard]] IMG* getImg() { return &img; }
    [[nodiscard]] std::pair<int, int>* getLineaPos() { return &linea; }
    [[nodiscard]] bool get() const { return entrada->get(); }

    void simular();

    void click(int x, int y, Raton::Boton boton, Raton::Evento evento);
};



#endif //SALIDA_H
