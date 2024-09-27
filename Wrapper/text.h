//
// Created by julen on 15/08/24.
//

#ifndef TEXT_H
#define TEXT_H

#include "iostream"
#include "SDL_ttf.h"

class TXT
{
    TTF_Font* m_font{nullptr};
    int m_size{10};
    SDL_Color m_color{255, 255, 255};
    SDL_Renderer* m_renderer;
    int m_x{0};
    int m_y{0};
    int m_alturaSalto{30};

public:
    TXT(const char* fontName, const int fontSize, const SDL_Color fontColor, SDL_Renderer* renderer)
        : m_font{TTF_OpenFont(fontName, fontSize)}, m_color{fontColor}, m_renderer(renderer)
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

    friend TXT& operator<< (TXT& anterior, const char* str);

    void setPos(const int x, const int y) { m_x = x; m_y = y; }
    void setPos(const int x, const int y, const int salto) { m_x = x; m_y = y; m_alturaSalto = salto; }

    void write(const char* text);

    void write(int x, int y, const char* text);
};

#endif //TEXT_H
