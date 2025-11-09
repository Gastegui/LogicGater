//
// Created by julen on 15/08/24.
//

#ifndef TEXT_H
#define TEXT_H

#include "iostream"
#include "SDL_ttf.h"

class TXT
{
    TTF_Font* font{};
    int size{10};
    SDL_Color color{255, 255, 255};
    SDL_Renderer* renderer;
    int x{0};
    int y{0};
    int alturaSalto{30};

public:
    TXT(const char* fontName, const int fontSize, const SDL_Color fontColor, SDL_Renderer* renderer)
        : font{TTF_OpenFont(fontName, fontSize)}, color{fontColor}, renderer(renderer)
    {
        // ReSharper disable once CppDFAConstantConditions
        if(font == nullptr)
            // ReSharper disable once CppDFAUnreachableCode
            std::println(std::cerr, "No se ha podido crear el texto. Error: {}", SDL_GetError());
    }

    ~TXT()
    {
        if(font != nullptr)
            TTF_CloseFont(font);
    }

    TXT(const TXT&) = delete;
    auto operator=(const TXT&) -> TXT& = delete;
    TXT(TXT&&) = delete;
    auto operator=(TXT&&) -> TXT& = delete;

    constexpr auto operator!() const -> bool
    {
        return font == nullptr;
    }

    friend auto operator<<(TXT& anterior, const char* str) -> TXT&;

    friend auto operator<<(TXT& anterior, const std::string& str) -> TXT&;

    auto setPos(const int x_, const int y_) -> void
    {
        x = x_;
        y = y_;
    }

    auto setPos(const int x_, const int y_, const int salto) -> void
    {
        x = x_;
        y = y_;
        alturaSalto = salto;
    }

    auto setX(const int x_) -> void
    {
        x = x_;
    }

    auto setY(const int y_) -> void
    {
        y = y_;
    }

    auto write(const char* text) -> void;

    auto write(int x_, int y_, const char* text) -> void;
};

#endif //TEXT_H
