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

public:
    struct Capa
    {
        std::string img;
        std::string texto;
        int textoX;
        int textoY;
    };

private:
    friend class SistemaGuardado;

    SDL_Renderer* renderer{nullptr};
    SDL_Texture* texture{nullptr};
    SDL_Rect rect{};
    SDL_Rect rectReal{};
    unsigned int id;
    bool esClickable{false};
    Simulable* simulable{nullptr};
    TIPOS_SIMULABLES tipoSimulable{TIPOS_SIMULABLES::Nada};

    int rotacion{0};

    static auto idGenerator() -> unsigned int
    {
        static unsigned int id = 1 + getOffset(ID_TIPOS::IMG);
        return id++;
    }

    auto procesarCapa(const Capa &capa) const -> void;
    
public:
    IMG(const char* path, SDL_Renderer* renderer)
        : IMG{path, renderer, 0, 0} {}

    IMG(const char* path, SDL_Renderer* renderer, const int x, const int y)
        : renderer{renderer}, texture{IMG_LoadTexture(renderer, path)}, id{idGenerator()}
    {
        // ReSharper disable once CppDFAConstantConditions
        if(texture == nullptr)
        // ReSharper disable once CppDFAUnreachableCode
            std::println(std::cerr, "No se ha podido cargar la imagen: {} Error: {}", path, SDL_GetError());
        else
        {
            SDL_QueryTexture(texture, nullptr, nullptr, &rect.w, &rect.h);
            rect.x = x;
            rect.y = y;
            rectReal = rect;
        }
    }

    IMG(SDL_Renderer* renderer, const int x, const int y, const int w, const int h)
        : renderer{renderer}, id{idGenerator()}
    {
        rect.x = x;
        rect.y = y;
        rect.w = w;
        rect.h = h;
        rectReal = rect; //NOLINT
    }

    [[nodiscard]] auto getId() const -> unsigned long { return id; }
    [[nodiscard]] auto getTexture() const -> SDL_Texture* { return texture; }
    [[nodiscard]] auto getRect() -> SDL_Rect* { return &rect; }
    [[nodiscard]] auto getRectReal() -> SDL_Rect* { return &rectReal; }
    [[nodiscard]] auto getClickable() const -> bool { return esClickable; }

    auto setClickable(Simulable* simulable_) -> void
    {
        simulable = simulable_;
        tipoSimulable = simulable_->getTipo();
        esClickable = true;
    }

    auto setClickable() -> void
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
        return texture == nullptr;
    }

    auto moverRel(const int x, const int y) -> void
    {
        rect.x += x;
        rect.y += y;
        rectReal.x += x;
        rectReal.y += y;
    }

    auto interactuar(const int x, const int y, const INTERACCIONES interaccion) const -> void
    {
        simulable->interactuar(x, y, interaccion);
    }

    template <typename... Args>
    auto crearImagen(Args&&... capas) -> bool
    {
        if(texture != nullptr)
            SDL_DestroyTexture(texture);

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, rectReal.w, rectReal.h);

        if(texture == nullptr)
            return false;

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // Color transparente
        SDL_RenderClear(renderer);
        // Procesar cada capa usando fold expression (C++17)
        (procesarCapa(std::forward<Args>(capas)), ...);
        SDL_SetRenderTarget(renderer, nullptr);
        return true;
    }

    auto setRotacion(int rotacion_) -> void;

    [[nodiscard]] auto getRotacion() const -> int { return rotacion; }

private:
};

#endif //IMAGE_H
