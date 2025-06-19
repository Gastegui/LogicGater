//
// Created by julen on 22/08/24.
//

#include "image.h"
#include "text.h"
#include "window.h"

auto IMG::crearPuerta(const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida) -> bool
{
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100, 50);

    if(m_texture == nullptr)
        return false;

    SDL_SetRenderTarget(m_renderer, m_texture);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0); // Color transparente
    SDL_RenderClear(m_renderer);

    SDL_Texture* tmp = IMG_LoadTexture(m_renderer, entradaArriba);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    tmp = IMG_LoadTexture(m_renderer, entradaAbajo);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    tmp = IMG_LoadTexture(m_renderer, cuerpo);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    tmp = IMG_LoadTexture(m_renderer, salida);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    SDL_SetRenderTarget(m_renderer, nullptr);

    return true;
}

auto IMG::crearTemporizador(const char* entrada, const char* cuerpo, const char* salida, const char* relleno, const int ciclosActuales, const int ciclosTotales) -> bool
{
    m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 100, 50);

    if(m_texture == nullptr)
        return false;

    SDL_SetRenderTarget(m_renderer, m_texture);
    SDL_SetTextureBlendMode(m_texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 0); // Color transparente
    SDL_RenderClear(m_renderer);

    SDL_Texture* tmp = IMG_LoadTexture(m_renderer, entrada);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    tmp = IMG_LoadTexture(m_renderer, cuerpo);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    tmp = IMG_LoadTexture(m_renderer, salida);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    tmp = IMG_LoadTexture(m_renderer, relleno);
    SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(m_renderer, tmp, nullptr, nullptr);

    txt->write(40, -8, std::to_string(ciclosTotales).c_str());
    txt->write(40, 27, std::to_string(ciclosActuales).c_str());

    SDL_SetRenderTarget(m_renderer, nullptr);

    return true;
}
