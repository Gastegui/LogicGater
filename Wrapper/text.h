//
// Created by julen on 15/08/24.
//

#ifndef TEXT_H
#define TEXT_H

#include "window.h"

class TXT
{
    TTF_Font* m_font{nullptr};
    int m_size{10};
    SDL_Color m_color{255, 255, 255};
    Window* m_window{nullptr};

public:
    TXT(const char* fontName, const int fontSize, const SDL_Color fontColor, Window* img)
        : m_font{TTF_OpenFont(fontName, fontSize)}, m_color{fontColor}, m_window{img}
    {
        // ReSharper disable once CppDFAConstantConditions
        if(m_font == nullptr)
            // ReSharper disable once CppDFAUnreachableCode
            std::cerr << "No se ha podido crear el texto. Error: " << SDL_GetError() << std::endl;
    }

    ~TXT()
    {
        if(m_font != nullptr)
            TTF_CloseFont(m_font);
    }

    constexpr bool operator!() const
    {
        return m_font == nullptr;
    }

    void write(const int x, const int y, const char* text) const
    {

        if(m_font == nullptr)
        {
            std::cerr << "Se ha intentado escribir pero no se ha cargado el texto" << std::endl;
            return;
        }

        SDL_Rect rect;
        SDL_Renderer* renderer{m_window->getRenderer()};
        SDL_Surface* surface = TTF_RenderText_Blended(m_font, text, m_color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

        rect.x = x;
        rect.y = y;
        rect.w = surface->w;
        rect.h = surface->h;

        SDL_RenderCopy(renderer, texture, nullptr, &rect);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
};

#endif //TEXT_H
