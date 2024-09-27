//
// Created by julen on 18/08/24.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>

#include "SDL_image.h"
#include "../raton.h"

class Puerta;
class Raton;
class Entrada;
class Salida;
class Window;

class IMG
{
    SDL_Renderer* m_renderer{nullptr};
    SDL_Texture* m_texture{nullptr};
    SDL_Rect     m_rect{};
    unsigned int id;
    bool esPuerta{false};
    bool esClickable{false};
    Puerta* puerta{nullptr};
    Entrada* entrada{nullptr};
    Salida* salida{nullptr};

    static unsigned int idGenerator()
    {
        static unsigned int imgIdGenerator = 0;
        return imgIdGenerator++;
    }

    bool crearPuerta(const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida);
public:
    IMG(const char* path, SDL_Renderer* renderer)
        :IMG{path, renderer, 0, 0}
    {}

    IMG(const char* path, SDL_Renderer* renderer, const int x, const int y)
        :m_renderer{renderer}, m_texture{IMG_LoadTexture(m_renderer, path)}, id{idGenerator()}
    {
        // ReSharper disable once CppDFAConstantConditions
        if(m_texture == nullptr)
        // ReSharper disable once CppDFAUnreachableCode
            std::cerr << "No se ha podido cargar la imagen: " << path << " Error: " << SDL_GetError() << std::endl;
        else
        {
            SDL_QueryTexture(m_texture, nullptr, nullptr, &m_rect.w, &m_rect.h);
            m_rect.x = x;
            m_rect.y = y;
        }
    }

    //Crea la image de una puerta
    IMG(SDL_Renderer* renderer, const int x, const int y, const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida)
        :m_renderer{renderer}, id{idGenerator()}
    {
        esPuerta = true;
        if(crearPuerta(entradaArriba, entradaAbajo, cuerpo, salida))
        {
            SDL_QueryTexture(m_texture, nullptr, nullptr, &m_rect.w, &m_rect.h);
            m_rect.x = x;
            m_rect.y = y;
        }
        else
            std::cerr << "No se ha podido crear la imagen de la puerta lógica. Error " << SDL_GetError() << std::endl;
    }

    ~IMG()
    {
        if(m_texture != nullptr)
            SDL_DestroyTexture(m_texture);
    }

    [[nodiscard]] unsigned long getId() const { return id; }
    [[nodiscard]] SDL_Texture* getTexture() const { return m_texture; }
    [[nodiscard]] SDL_Rect* getRect() { return &m_rect; }
    [[nodiscard]] bool getClickable() const { return esClickable; }

    void setClickable(Puerta* puerta_);
    void setClickable(Entrada* boton_);
    void setClickable(Salida* salida_);
    [[nodiscard]] Puerta* getPadrePuerta() const { return puerta; }
    [[nodiscard]] Entrada* getPadreEntrada() const { return entrada; }
    [[nodiscard]] Salida* getPadreSalida() const { return salida; }


    bool operator==(const IMG& a) const
    {
        return this->getId() == a.getId();
    }

    bool operator!() const
    {
        return m_texture == nullptr;
    }

    void mover(const int x, const int y)
    {
        m_rect.x = x;
        m_rect.y = y;
    }

    void cambiarImagen(const char* path)
    {
        SDL_DestroyTexture(m_texture);

        m_texture = IMG_LoadTexture(m_renderer, path);

        if(m_texture == nullptr)
                std::cerr << "No se ha podido cargar la imagen: " << path << " Error: " << SDL_GetError() << std::endl;
    }

    bool cambiarPuerta(const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida)
    {
        if(!esPuerta)
            return false;

        SDL_DestroyTexture(m_texture);

        return crearPuerta(entradaArriba, entradaAbajo, cuerpo, salida);
    }

    void clickar(int x, int y, Raton::Evento evento) const;
};

#endif //IMAGE_H
