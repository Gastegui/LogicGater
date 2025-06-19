//
// Created by julen on 18/08/24.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>

#include "SDL_image.h"
#include "../enums.h"
#include "../Simulables/simulable.h"

class TXT;
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
    bool esClickable{false};
    Simulable* simulable{nullptr};
    TIPOS_SIMULABLES tipoSimulable{TIPOS_SIMULABLES::Nada};
    TXT* txt{nullptr};

    static auto idGenerator() -> unsigned int
    {
        static unsigned int id = 1 + get_offset(ID_TIPOS::IMG);
        return id++;
    }

    auto crearPuerta(const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida) -> bool;
    auto crearTemporizador(const char* entrada, const char* cuerpo, const char* salida, const char* relleno, int ciclosActuales, int ciclosTotales) -> bool;
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
            std::println(std::cerr, "No se ha podido cargar la imagen: {} Error: {}", path, SDL_GetError());
        else
        {
            SDL_QueryTexture(m_texture, nullptr, nullptr, &m_rect.w, &m_rect.h);
            m_rect.x = x;
            m_rect.y = y;
        }
    }

    //Crea la imagen de una puerta
    IMG(SDL_Renderer* renderer, const int x, const int y, const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida)
        :m_renderer{renderer}, id{idGenerator()}
    {
        if(crearPuerta(entradaArriba, entradaAbajo, cuerpo, salida))
        {
            SDL_QueryTexture(m_texture, nullptr, nullptr, &m_rect.w, &m_rect.h);
            m_rect.x = x;
            m_rect.y = y;
        }
        else
            std::println(std::cerr, "No se ha podido crear la imagen de la puerta lógica. Error: {}", SDL_GetError());
    }

    //Crea la imagen de un temporizador
    IMG(SDL_Renderer* renderer, const int x, const int y, const char* entrada, const char* cuerpo, const char* salida, const char* relleno, const int ciclos, TXT* txt_)
        :m_renderer{renderer}, id{idGenerator()}, txt{txt_}
    {
        if(crearTemporizador(entrada, cuerpo, salida, relleno, 0, ciclos))
        {
            SDL_QueryTexture(m_texture, nullptr, nullptr, &m_rect.w, &m_rect.h);
            m_rect.x = x;
            m_rect.y = y;
        }
        else
            std::println(std::cerr, "No se ha podido crear la imagen del temporizador. Error: {}", SDL_GetError());
    }

    ~IMG()
    {
        if(m_texture != nullptr)
            SDL_DestroyTexture(m_texture);
    }

    [[nodiscard]] auto getId() const -> unsigned long { return id; }
    [[nodiscard]] auto getTexture() const -> SDL_Texture* { return m_texture; }
    [[nodiscard]] auto getRect() -> SDL_Rect* { return &m_rect; }
    [[nodiscard]] auto getClickable() const -> bool { return esClickable; }

    void setClickable(Simulable* simulable_)
    {
        simulable = simulable_;
        tipoSimulable = simulable_->getTipo();
        esClickable = true;
    }
    void setClickable()
    {
        esClickable = true;
    }
    [[nodiscard]] auto getSimulable() const -> Simulable* { return simulable; }

    auto operator==(const IMG& a) const -> bool
    {
        return this->getId() == a.getId();
    }

    auto operator!() const -> bool
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
            std::println(std::cerr, "No se h podido cargar la imagen: {} Error: {}", path, SDL_GetError());
    }

    auto cambiarPuerta(const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida) -> bool
    {
        if(tipoSimulable != TIPOS_SIMULABLES::Puerta)
            return false;

        SDL_DestroyTexture(m_texture);

        return crearPuerta(entradaArriba, entradaAbajo, cuerpo, salida);
    }

    auto cambiarTemporizador(const char* entrada, const char* cuerpo, const char* salida, const char* relleno, const int ciclosActuales, const int ciclosTotales) -> bool
    {
        if(tipoSimulable != TIPOS_SIMULABLES::Temporizador)
            return false;

        SDL_DestroyTexture(m_texture);

        return crearTemporizador(entrada, cuerpo, salida, relleno, ciclosActuales, ciclosTotales);
    }

    void interactuar(const int x, const int y, const INTERACCIONES interaccion) const { simulable->interactuar(x, y, interaccion); }
};

#endif //IMAGE_H
