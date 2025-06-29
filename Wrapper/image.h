//
// Created by julen on 18/08/24.
//

#ifndef IMAGE_H
#define IMAGE_H

#include <iostream>

#include "SDL_image.h"
#include "../enums.h"
#include "../Simulables/simulable.h"
#include "text.h"

class Puerta;
class Entrada;
class Salida;
class SistemaGuardado;

class IMG
{
    friend class SistemaGuardado;

    SDL_Renderer* m_renderer{nullptr};
    SDL_Texture* m_texture{nullptr};
    SDL_Rect m_rect{};
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

public:
    struct Capa
    {
        std::string img;
        std::string texto;
        int textoX;
        int textoY;
    };

    IMG(const char* path, SDL_Renderer* renderer)
        : IMG{path, renderer, 0, 0} {}

    IMG(const char* path, SDL_Renderer* renderer, const int x, const int y)
        : m_renderer{renderer}, m_texture{IMG_LoadTexture(m_renderer, path)}, id{idGenerator()}
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

    IMG(SDL_Renderer* renderer, const int x, const int y, const int w, const int h)
        : m_renderer{renderer}, id{idGenerator()}
    {
        m_rect.x = x;
        m_rect.y = y;
        m_rect.w = w;
        m_rect.h = h;
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

    void interactuar(const int x, const int y, const INTERACCIONES interaccion) const
    {
        simulable->interactuar(x, y, interaccion);
    }

    template <typename... Args>
    auto crearImagen(Args&&... capas) -> bool
    {
        if(m_texture != nullptr)
            SDL_DestroyTexture(m_texture);

        m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, m_rect.w, m_rect.h);

        if(m_texture == nullptr)
            return false;

        SDL_SetRenderTarget(m_renderer, m_texture);
        SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0); // Color transparente
        SDL_RenderClear(m_renderer);
        // Procesar cada capa usando fold expression (C++17)
        (procesarCapa(std::forward<Args>(capas)), ...);
        SDL_SetRenderTarget(m_renderer, nullptr);
        return true;
    }

    void setTXT(TXT* txt_) { txt = txt_; }

private:
    void procesarCapa(const Capa& capa)
    {
        if(!capa.img.empty())
        {
            SDL_Texture* tmp = IMG_LoadTexture(m_renderer, capa.img.c_str());

            if(tmp == nullptr)
                std::println(std::cerr, "No se h podido cargar la imagen: {} Error: {}", capa.img, SDL_GetError());

            SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
            SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);
            SDL_DestroyTexture(tmp); // No olvides liberar la memoria
        }
        else
        {
            txt->write(capa.textoX, capa.textoY, capa.texto.c_str());
        }
    }
};

#endif //IMAGE_H
