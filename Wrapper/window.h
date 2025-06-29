//
// Created by julen on 15/08/24.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_image.h>

#include "image.h"
#include "../raton.h"
#include "../ListaLineas.h"

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
    ListaIMG listaIMG{};

    int minimapaTamañoObjetivoX;
    int minimapaTamañoObjetivoY;
    IMG minimapaFondo;
    mutable int mapaMinX{0}; //Mutable es para que rendererDraw siga siendo const y que se puedan cambiar sus valores
    mutable int mapaMaxX{0};
    mutable int mapaMinY{0};
    mutable int mapaMaxY{0};

    mutable SDL_Rect seleccion{};

    void rendererClear() const;
    void rendererDraw() const;
    void rendererPresent() const;

    static void renderLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const ListaLineas::Lista* linea);

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
        listaIMG.añadir(&minimapaFondo, ListaIMG::FRENTE);
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

    [[nodiscard]] auto getRenderer() const -> SDL_Renderer* { return renderer; }
    [[nodiscard]] auto getWindow() const -> SDL_Window* { return window; }
    [[nodiscard]] auto getRaton() -> Raton* { return &raton; }

    void setControlador(Controlador* controlador_)
    {
        controlador = controlador_;
        raton.setControlador(controlador_);
    }

    auto operator!() const -> bool
    {
        return window == nullptr;
    }

    void limpiar() const;
    void render() const;

    [[nodiscard]] auto getListaIMG(const ListaIMG::Altura altura) const -> ListaIMG::Lista*
    {
        return listaIMG.getLista(altura);
    }

    //No borra la imagen. Eso es trabajo de la clase IMG
    auto añadir(IMG* img, const ListaIMG::Altura altura) -> bool
    {
        return listaIMG.añadir(img, altura);
    }

    //No borra las imagenes. Eso es trabajo de la clase IMG
    auto borrar(const IMG* img, const ListaIMG::Altura altura) -> bool
    {
        if(listaIMG.quitar(img, altura))
        {
            raton.setImgAnteriorNull();
            return true;
        }
        return false;
    }

    void moverRel(int x, int y);
    void mover(int x, int y);
    void centrar();

    [[nodiscard]] auto getEsquinaX() const -> int { return esquinaX; }
    [[nodiscard]] auto getEsquinaY() const -> int { return esquinaY; }

    [[nodiscard]] auto getIMGMinimapa() -> IMG* { return &minimapaFondo; }
    [[nodiscard]] auto getMapaMinX() const -> int { return mapaMinX; }
    [[nodiscard]] auto getMapaMaxX() const -> int { return mapaMaxX; }
    [[nodiscard]] auto getMapaMinY() const -> int { return mapaMinY; }
    [[nodiscard]] auto getMapaMaxY() const -> int { return mapaMaxY; }
    [[nodiscard]] auto getMinimapaTamañoX() const -> int { return minimapaTamañoObjetivoX; }
    [[nodiscard]] auto getMinimapaTamañoY() const -> int { return minimapaTamañoObjetivoY; }

    void setSeleccion(SDL_Rect rect)
    {
        seleccion = rect;
    }
};

#endif //WINDOW_H
