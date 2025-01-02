//
// Created by julen on 15/08/24.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL_image.h>

#include "../raton.h"

class IMG;
class Raton;
class Controlador;
class Puerta;
class Entrada;
class Salida;

class Window
{
    SDL_Window* window {nullptr};
    SDL_Renderer* renderer {nullptr};
    Raton raton{};
    SDL_Surface* windowIcon{nullptr};
    Controlador* controlador{nullptr};
    ListaIMG listaIMG{};


    void rendererClear() const;
    void rendererDraw() const;
    void rendererPresent() const;

public:
    struct ListaLineas
    {
        Puerta* origenPuerta{nullptr};
        Entrada* origenEntrada{nullptr};

        Puerta* destinoPuerta{nullptr};
        bool puertaArriba{nullptr};
        Salida* destinoSalida{nullptr};

        ListaLineas* siguiente{nullptr};
    };

    ListaLineas* lineas{nullptr};

    explicit Window(const char* img)
        :window{SDL_CreateWindow("LogicGater", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_FULLSCREEN)}, renderer{SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)}
    {
        // ReSharper disable once CppDFAConstantConditions
        if(window != nullptr)
        {
            windowIcon = IMG_Load(img);
            if(windowIcon != nullptr)
                SDL_SetWindowIcon(window, windowIcon);
            SDL_SetRenderTarget(renderer, nullptr);
        }
    }

    ~Window()
    {
        if(renderer != nullptr)
            SDL_DestroyRenderer(renderer);
        if(window != nullptr)
            SDL_DestroyWindow(window);
        if(windowIcon != nullptr)
            SDL_FreeSurface(windowIcon);
        if(lineas != nullptr)
        {
            // ReSharper disable once CppDFAUnusedValue
            const ListaLineas* tmp{nullptr};
            while(lineas != nullptr)
            {
                tmp = lineas;
                lineas = lineas->siguiente;
                delete tmp;
            }
        }
    }

    [[nodiscard]] SDL_Renderer* getRenderer() const { return renderer; }
    [[nodiscard]] SDL_Window* getWindow() const { return window; }
    [[nodiscard]] Raton* getRaton() {return &raton; }
    void setControlador(Controlador* controlador_)
    {
        controlador = controlador_;
        raton.setControlador(controlador_);
    }

    bool operator!() const
    {
        return window == nullptr;
    }

    void limpiar() const;
    void render() const;

    [[nodiscard]] ListaIMG::Lista* getListaIMG(const ListaIMG::Altura altura) const
    {
        return listaIMG.getLista(altura);
    }

    //No borra la imagen. Eso es trabajo de la clase IMG
    bool añadir(IMG* img, const ListaIMG::Altura altura)
    {
        return listaIMG.añadir(img, altura);
    }
    //No borra las imagenes. Eso es trabajo de la clase IMG
    bool borrar(const IMG* img, const ListaIMG::Altura altura)
    {
        return listaIMG.quitar(img, altura);
    }

    void añadirLinea(Puerta* origen, Puerta* destino, bool arriba);
    void añadirLinea(Entrada* origen, Puerta* destino, bool arriba);
    void añadirLinea(Puerta* origen, Salida* destino);
    void añadirLinea(Entrada* origen, Salida* destino);

    void borrarLineas(const Puerta* puerta, bool arriba = true, bool abajo = true, bool salida = true);
    void borrarLineas(const Entrada* entrada);
    void borrarLineas(const Salida* salida);

    void manejarRaton();
};

#endif //WINDOW_H
