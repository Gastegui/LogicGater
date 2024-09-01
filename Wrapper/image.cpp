//
// Created by julen on 22/08/24.
//

#include "image.h"
#include "window.h"
#include "../puerta.h"
#include "../entrada.h"
#include "../salida.h"

bool IMG::crearPuerta(const char* entradaArriba, const char* entradaAbajo, const char* cuerpo, const char* salida)
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

void IMG::setClickable(Puerta* puerta_)
{
    esClickable = true;
    puerta = puerta_;
}

void IMG::setClickable(Entrada* boton_)
{
    esClickable = true;
    entrada = boton_;
}

void IMG::setClickable(Salida* salida_)
{
    esClickable = true;
    salida = salida_;
}

void IMG::clickar(const int x, const int y, const Raton::Boton ratonBoton, const Raton::Evento evento) const
{
    if(!esClickable)
        return;

    if(puerta != nullptr)
        puerta->click(x, y, ratonBoton, evento);
    else if(entrada != nullptr)
        entrada->click(x, y, ratonBoton, evento);
    else if(salida != nullptr)
        salida->click(x, y, ratonBoton, evento);
}
