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

    auto interactuar(IMG* actual) -> void;
    auto interactuarConexion(IMG* actual) -> bool;
    auto buscarLista(const std::vector<IMG*>* lista, int posX, int posY, bool absoluto = false) const -> IMG*;

    [[nodiscard]] static auto interpolacionLinear(const double x, const double a, const double b, const double c, const double d) -> int
    {
        return static_cast<int>(c + (x - a) / (b - a) * (d - c));
    }

public:
    auto setControlador(Controlador* controlador_) -> void;

    auto setBorrando(bool borrando_) -> void;
    [[nodiscard]] auto getBorrando() const -> bool { return borrando; }
    auto setImgAnteriorNull() -> void { imgAnterior = nullptr; }

    auto manejarRaton() -> void;
    auto setMoviendoImg(IMG* img) -> void { moviendoImg = img; }
};


#endif //RATON_H
