//
// Created by julen on 15/08/24.
//

#ifndef TEXT_H
#define TEXT_H

#include "iostream"
#include <SDL2/SDL_ttf.h>

class TXT
{
    TTF_Font* font{};
    int size{10};
    SDL_Color color{255, 255, 255};
    int x{0};
    int y{0};
    int alturaSalto{30};

public:
    TXT(const char* fontName, const int fontSize, const SDL_Color fontColor)
        : font{TTF_OpenFont(fontName, fontSize)}, color{fontColor}
    {
        if(font == nullptr)
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

    [[nodiscard]] auto static get() -> TXT&
    {
        static TXT txt = TXT{"ttf/8bitOperatorPlusSC-Regular.ttf", 20, SDL_Color{255, 255, 255, 255}};
        return txt;
    }
};

#endif //TEXT_H
