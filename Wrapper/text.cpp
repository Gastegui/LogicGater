//
// Created by julen on 27/09/24.
//

#include "text.h"


auto operator<<(TXT& anterior, const char* str) -> TXT&
{
    anterior.write(str);
    return anterior;
}

auto operator<<(TXT& anterior, const std::string& str) -> TXT&
{
    anterior.write(str.c_str());
    return anterior;
}

auto TXT::write(const char* text) -> void
{
    if(font == nullptr)
    {
        std::println(std::cerr, "Se ha intentado escribir pero no se ha cargado el texto");
        return;
    }
    SDL_Rect rect;
    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text, color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    rect.x = x;
    rect.y = y;
    rect.w = surface->w;
    rect.h = surface->h;

    if(x == -2)
        rect.x = 960 - rect.w/2; //960 = 1920/2

    if(y == -2)
        rect.y = 540 - rect.h/2; //540 = 1080/2

    y += alturaSalto;

    SDL_RenderCopy(renderer, texture, nullptr, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


auto TXT::write(const int x_, const int y_, const char* text) -> void
{
    x = x_;
    y = y_;
    write(text);
}
