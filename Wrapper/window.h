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

    int esquinaX = 0;
    int esquinaY = 0;
    const int width;
    const int height;

    SDL_Window* window {nullptr};
    SDL_Renderer* renderer {nullptr};
    Raton raton{};
    SDL_Surface* windowIcon{nullptr};
    Controlador* controlador{nullptr};
    ListaIMG listaIMG{};


    void rendererClear() const;
    void rendererDraw(bool simulando) const;
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
private:
    static void renderLine(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, const ListaLineas* linea, bool simulando);

public:
    ListaLineas* lineas{nullptr};

    explicit Window(const char* img)
        :width(1920), height(1080), window{SDL_CreateWindow("LogicGater", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_FULLSCREEN)}, renderer{SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)}
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
    void render(bool simulando) const;

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
    void moverRel(int x, int y);
    [[nodiscard]] int getEsquinaX() const { return esquinaX; }
    [[nodiscard]] int getEsquinaY() const { return esquinaY; }
};

#endif //WINDOW_H
