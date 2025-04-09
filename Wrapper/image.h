//
// Created by julen on 18/08/24.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>

#include "SDL_image.h"
#include "../enums.h"
#include "../Simulables/simulable.h"

class Puerta;
class Entrada;
class Salida;
class SistemaGuardado;

class IMG
{
    friend class SistemaGuardado;

    SDL_Renderer* m_renderer{nullptr};
    SDL_Texture* m_texture{nullptr};
    SDL_Rect     m_rect{};
    unsigned int id;
    bool esPuerta{false};
    bool esClickable{false};
    Simulable* simulable{nullptr};
    TIPOS_SIMULABLES tipoSimulable{TIPOS_SIMULABLES::Nada};

    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + 1<<27;
        return id++;
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

    void setClickable(Simulable* simulable_)
    {
        simulable = simulable_;
        tipoSimulable = simulable_->getTipo();
    }
    [[nodiscard]] Simulable* getSimulable() const { return simulable; }

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

    void interactuar(const int x, const int y, const INTERACCIONES interaccion) const { simulable->interactuar(x, y, interaccion); }
};

#endif //IMAGE_H
