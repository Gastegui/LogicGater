//
// Created by julen on 27/09/24.
//

#include "text.h"


TXT& operator<< (TXT& anterior, const char* str)
{
    anterior.write(str);
    return anterior;
}

void TXT::write(const char* text)
{
    if(m_font == nullptr)
    {
        std::cerr << "Se ha intentado escribir pero no se ha cargado el texto" << std::endl;
        return;
    }

    SDL_Rect rect;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(m_font, text, m_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);

    rect.x = m_x;
    rect.y = m_y;
    rect.w = surface->w;
    rect.h = surface->h;

    m_y += m_alturaSalto;

    SDL_RenderCopy(m_renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


void TXT::write(const int x, const int y, const char* text)
{
    m_x = x;
    m_y = y;
    write(text);
}
