#include "image.h"
#include "text.h"

auto IMG::setRotacion(int rotacion_) -> void
{
    rotacion = rotacion_;
    if(rotacion == 90 || rotacion == 270)
    {
        int centroX = rectReal.x + rectReal.w / 2;
        int centroY = rectReal.y + rectReal.h / 2;

        rect.w = rectReal.h;
        rect.h = rectReal.w;

        rect.x = centroX - rect.w / 2;
        rect.y = centroY - rect.h / 2;
    }
    else
        rect = rectReal;
}

auto IMG::procesarCapa(const Capa &capa) const -> void
{
    if(!capa.img.empty())
    {
        SDL_Texture *tmp = IMG_LoadTexture(renderer, capa.img.c_str());

        if(tmp == nullptr)
            std::println(std::cerr, "No se h podido cargar la imagen: {} Error: {}", capa.img, SDL_GetError());

        SDL_SetTextureBlendMode(tmp, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, tmp, nullptr, nullptr);
        SDL_DestroyTexture(tmp); // No olvides liberar la memoria
    }
    else
    {
        TXT::get().write(capa.textoX, capa.textoY, capa.texto.c_str());
    }
}
