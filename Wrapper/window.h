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


    void rendererClear() const;
    void rendererDraw() const;
    void rendererPresent() const;

public:
    struct ListaImg
    {
        IMG* img;
        ListaImg* siguiente;
    };

    struct ListaLineas
    {
        Puerta* origenPuerta{nullptr};
        Entrada* origenEntrada{nullptr};

        Puerta* destinoPuerta{nullptr};
        bool puertaArriba{nullptr};
        Salida* destinoSalida{nullptr};

        ListaLineas* siguiente{nullptr};
    };

    ListaImg* fondo{nullptr};
    ListaImg* medio{nullptr};
    ListaImg* frente{nullptr};

    ListaLineas* lineas{nullptr};

    enum Altura
    {
        INVALIDO,
        FONDO,
        MEDIO,
        FRENTE
    };

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
        if(fondo != nullptr)
        {
            // ReSharper disable once CppDFAUnusedValue
            const ListaImg* tmp{nullptr};
            while(fondo != nullptr)
            {
                tmp = fondo;
                fondo = fondo->siguiente;
                delete tmp;
            }
        }
        if(medio != nullptr)
        {
            // ReSharper disable once CppDFAUnusedValue
            const ListaImg* tmp{nullptr};
            while(medio != nullptr)
            {
                tmp = medio;
                medio = medio->siguiente;
                delete tmp;
            }
        }
        if(frente != nullptr)
        {
            // ReSharper disable once CppDFAUnusedValue
            const ListaImg* tmp{nullptr};
            while(frente != nullptr)
            {
                tmp = frente;
                frente = frente->siguiente;
                delete tmp;
            }
        }
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

    bool añadir(IMG* img, Altura altura, bool final = true);
    //No borra la imagen. Eso es trabajo de la clase IMG
    bool borrar(const IMG* img, Altura altura);
    //No borra las imagenes. Eso es trabajo de la clase IMG
    bool borrarTodo(Altura altura);

    void añadirLinea(Puerta* origen, Puerta* destino, bool arriba);
    void añadirLinea(Entrada* origen, Puerta* destino, bool arriba);
    void añadirLinea(Puerta* origen, Salida* destino);
    void añadirLinea(Entrada* origen, Salida* destino);
/*
    bool borrarLinea(const Puerta* origen, const Puerta* destino, bool arriba);
    bool borrarLinea(const Entrada* origen, const Puerta* destino, bool arriba);
    bool borrarLinea(const Puerta* origen, const Salida* destino);
    bool borrarLinea(const Entrada* origen, const Salida* destino);
*/
    void borrarLineas(const Puerta* puerta, bool arriba = true, bool abajo = true, bool salida = true);
    void borrarLineas(const Entrada* entrada);
    void borrarLineas(const Salida* salida);

    void manejarRaton(const SDL_Event* evento);
};

#endif //WINDOW_H
