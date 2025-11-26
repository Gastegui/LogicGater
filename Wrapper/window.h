//
// Created by julen on 15/08/24.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_image.h>
#include <map>

#include "image.h"
#include "../raton.h"
#include "../ListaLineas.h"
#include "../ListaIMG.h"

class IO;
class Raton;
class Controlador;
class Puerta;
class Entrada;
class Salida;
class Temporizador;

class Window
{
    int esquinaX = 0;
    int esquinaY = 0;
    const int width; // NOLINT(*-avoid-const-or-ref-data-members)
    const int height; // NOLINT(*-avoid-const-or-ref-data-members)

    SDL_Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
    Raton raton{};
    SDL_Surface* windowIcon{nullptr};
    Controlador* controlador{nullptr};
    std::map<ESTADOS, ListaIMG> listasIMG;

    int minimapaTamañoObjetivoX;
    int minimapaTamañoObjetivoY;
    IMG minimapaFondo;
    mutable int mapaMinX{0}; //Mutable es para que rendererDraw siga siendo const y que se puedan cambiar sus valores
    mutable int mapaMaxX{0};
    mutable int mapaMinY{0};
    mutable int mapaMaxY{0};

    mutable SDL_Rect seleccion{};

    auto rendererClear() const -> void;
    auto rendererDraw() const -> void;
    auto rendererPresent() const -> void;

    auto renderLine(int x1, int y1, int x2, int y2, const ListaLineas::Linea *linea) const -> void;

public:
    explicit Window(const int witdh_, const int height_, const char* img)
        : width{witdh_}, height{height_}, window{SDL_CreateWindow("LogicGater", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_FULLSCREEN)}, renderer{SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)}, minimapaTamañoObjetivoX{static_cast<int>(width * 0.2)}, minimapaTamañoObjetivoY{static_cast<int>(height * 0.2)}, minimapaFondo{"./img/minimapa.png", renderer, width - minimapaTamañoObjetivoX, height - minimapaTamañoObjetivoY}
    {
        // ReSharper disable once CppDFAConstantConditions
        if(window != nullptr)
        {
            windowIcon = IMG_Load(img);
            if(windowIcon != nullptr)
                SDL_SetWindowIcon(window, windowIcon);
            SDL_SetRenderTarget(renderer, nullptr);
        }
        minimapaFondo.setClickable();
        listasIMG[ESTADOS::Normal].añadir(&minimapaFondo, ListaIMG::Frente);
    }

    ~Window()
    {
        if(renderer != nullptr)
            SDL_DestroyRenderer(renderer);
        if(window != nullptr)
            SDL_DestroyWindow(window);
        if(windowIcon != nullptr)
            SDL_FreeSurface(windowIcon);
    }

    Window(const Window&) = delete;
    auto operator=(const Window&) -> Window& = delete;
    Window(Window&&) = delete;
    auto operator=(Window&&) -> Window& = delete;

    [[nodiscard]] auto getRenderer() const -> SDL_Renderer* { return renderer; }
    [[nodiscard]] auto getWindow() const -> SDL_Window* { return window; }
    [[nodiscard]] auto getRaton() -> Raton* { return &raton; }

    auto setControlador(Controlador* controlador_) -> void
    {
        controlador = controlador_;
        raton.setControlador(controlador_);
    }

    auto operator!() const -> bool
    {
        return window == nullptr;
    }

    auto limpiar() const -> void;
    auto render(bool rendererDraw) const -> void;

    [[nodiscard]] auto getListaIMG(ListaIMG::ALTURA altura) const -> const std::vector<IMG *> *;

    //No borra la imagen. Eso es trabajo de la clase IMG
    auto añadir(IMG *img, ListaIMG::ALTURA altura) -> bool;
    auto añadir(ESTADOS estado, IMG *img, ListaIMG::ALTURA altura) -> bool;

    //No borra las imagenes. Eso es trabajo de la clase IMG
    auto borrar(const IMG *img, ListaIMG::ALTURA altura) -> bool;
    auto borrar(ESTADOS estado, const IMG *img, ListaIMG::ALTURA altura) -> bool;

    auto moverRel(int x, int y) -> void;
    auto mover(int x, int y) -> void;
    auto centrar() -> void;

    [[nodiscard]] auto getEsquinaX() const -> int { return esquinaX; }
    [[nodiscard]] auto getEsquinaY() const -> int { return esquinaY; }

    [[nodiscard]] auto getImgMinimapa() -> IMG* { return &minimapaFondo; }
    [[nodiscard]] auto getMapaMinX() const -> int { return mapaMinX; }
    [[nodiscard]] auto getMapaMaxX() const -> int { return mapaMaxX; }
    [[nodiscard]] auto getMapaMinY() const -> int { return mapaMinY; }
    [[nodiscard]] auto getMapaMaxY() const -> int { return mapaMaxY; }
    [[nodiscard]] auto getMinimapaTamañoX() const -> int { return minimapaTamañoObjetivoX; }
    [[nodiscard]] auto getMinimapaTamañoY() const -> int { return minimapaTamañoObjetivoY; }

    auto setSeleccion(const SDL_Rect rect) const -> void
    {
        seleccion = rect;
    }
};

#endif //WINDOW_H
