//
// Created by julen on 23/08/24.
//

#ifndef RATON_H
#define RATON_H
#include <SDL_rect.h>
#include <vector>

#include "ListaIMG.h"


class Simulable;
class IMG;
class Controlador;
class Window;

class Raton
{
    int posX{0};
    int posY{0};
    IMG* imgAnterior{nullptr};

    Controlador* controlador{nullptr};
    Window* window{nullptr};

    bool borrando{false};
    bool moviendoPantalla{false};
    IMG* moviendoImg{nullptr};

    int cuadriculaX{0};
    int cuadriculaY{0};

    bool seleccionando{false};
    SDL_Rect seleccion{};
    int seleccionXOriginal{};
    int seleccionYOriginal{};
    std::vector<Simulable*> seleccionados;
    bool moviendoSeleccion{false};

    void interactuar(IMG* actual);
    auto interactuarConexion(IMG* actual) -> bool;
    auto buscarLista(const ListaIMG::Lista* lista, int posX, int posY, bool absoluto = false) const -> IMG*;
    [[nodiscard]] static auto interpolacionLinear(const double x, const double a, const double b, const double c, const double d) -> int
    {
        return static_cast<int>(c + (x - a) / (b - a) * (d - c));
    }

public:

    void setControlador(Controlador* controlador_);

    void setBorrando(bool borrando_);
    [[nodiscard]] auto getBorrando() const -> bool { return borrando; }
    void setImgAnteriorNull() { imgAnterior = nullptr; }

    void manejarRaton();
    void setMoviendoImg(IMG* img) { moviendoImg = img; }
};



#endif //RATON_H
