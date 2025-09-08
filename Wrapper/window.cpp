//
// Created by julen on 22/08/24.
//

#include <ranges>
#include "window.h"
#include "image.h"
#include "../Simulables/entrada.h"
#include "../Simulables/puerta.h"
#include "../controlador.h"

auto Window::rendererClear() const -> void
{
    SDL_RenderClear(renderer);
}

auto Window::renderLine(SDL_Renderer* renderer, const int x1, const int y1, const int x2, const int y2, const ListaLineas::Linea* linea) -> void
{
    if(linea->io->get())
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

auto Window::rendererDraw() const -> void
{
    // ReSharper disable CppDFANullDereference
    const std::vector<IMG*>* imagenes{listaIMG.getLista(ListaIMG::Fondo)};
    const std::vector<ListaLineas::Linea>* lineas{controlador->getListaLineas()->getLista()};
    mapaMinX = -esquinaX;
    mapaMaxX = -esquinaX + width;
    mapaMinY = -esquinaY;
    mapaMaxY = -esquinaY + height;

    if(controlador->getCuadriculaActiva())
    {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        for(int i = -esquinaX; i < -esquinaX + width; i++)
            if(i % controlador->getCuadriculaTamaño() == 0)
                SDL_RenderDrawLine(renderer, esquinaX + i, 0, esquinaX + i, esquinaX + i + height);

        for(int i = -esquinaY; i < -esquinaY + height; i++)
            if(i % controlador->getCuadriculaTamaño() == 0)
                SDL_RenderDrawLine(renderer, 0, esquinaY + i, esquinaY + i + width, esquinaY + i);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    // ReSharper restore CppDFANullDereference
    for(IMG* img : *imagenes)
        SDL_RenderCopy(renderer, img->getTexture(), nullptr, img->getRect());

    imagenes = listaIMG.getLista(ListaIMG::Medio);
    for(IMG* img : *imagenes | std::views::reverse)
    {
        SDL_Rect rect{*img->getRect()};

        if(rect.x < mapaMinX)
            mapaMinX = rect.x;
        else if(rect.x + rect.w > mapaMaxX)
            mapaMaxX = rect.x + rect.w;
        if(rect.y < mapaMinY)
            mapaMinY = rect.y;
        else if(rect.y + rect.h > mapaMaxY)
            mapaMaxY = rect.y + rect.h;

        if(rect.x < -esquinaX + width && rect.x + rect.w > -esquinaX && rect.y < -esquinaY + height && rect.y + rect.h > -esquinaY)
        {
            rect.x += esquinaX;
            rect.y += esquinaY;
            SDL_RenderCopy(renderer, img->getTexture(), nullptr, &rect);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect{-esquinaX, -esquinaY, width, height};
    for(ListaLineas::Linea linea : *lineas)
    {
        const std::pair<int, int>* origen = linea.io->getLineaOrigen();
        const std::pair<int, int>* destino = linea.destino->getLinea(linea.conexion);


        //Comprueba si alguno de los dos puntos de la línea está dentro de la pantalla
        if((origen->first > -esquinaX && origen->first < -esquinaX + width && origen->second > -esquinaY && origen->second < -esquinaY + height) ||
                (destino->first > -esquinaX && destino->first < -esquinaX + width && destino->second > -esquinaY && destino->second < -esquinaY + height))
            renderLine(renderer, origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX, destino->second + esquinaY, &linea);
        else
        {
            //Si no esta el origen o final de la línea dentro de la pantalla, comprueba si la línea intersecciona la pantalla
            int x1 = origen->first;
            int y1 = origen->second;
            int x2 = destino->first;
            int y2 = destino->second;
            if(SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2))
                renderLine(renderer, origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX, destino->second + esquinaY, &linea);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    imagenes = listaIMG.getLista(ListaIMG::Frente);
    for(IMG* img : *imagenes)
        SDL_RenderCopy(renderer, img->getTexture(), nullptr, img->getRect());

    if(seleccion.h != 0 && seleccion.w != 0)
    {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect tmp = seleccion;
        tmp.x += esquinaX;
        tmp.y += esquinaY;
        SDL_RenderDrawRect(renderer, &tmp);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }

    //Minimapa
    const double factorX = static_cast<double>(minimapaTamañoObjetivoX) / (mapaMaxX - mapaMinX);
    const double factorY = static_cast<double>(minimapaTamañoObjetivoY) / (mapaMaxY - mapaMinY);
    imagenes = listaIMG.getLista(ListaIMG::Medio);
    for(IMG* img : *imagenes)
    {
        rect = *img->getRect();
        rect.x = static_cast<int>((rect.x - mapaMinX) * factorX + (width - minimapaTamañoObjetivoX));
        rect.y = static_cast<int>((rect.y - mapaMinY) * factorY + (height - minimapaTamañoObjetivoY));
        rect.w = static_cast<int>(rect.w * factorX);
        rect.h = static_cast<int>(rect.h * factorY);
        SDL_RenderCopy(renderer, img->getTexture(), nullptr, &rect);
    }

    rect.x = static_cast<int>((-esquinaX - mapaMinX) * factorX + (width - minimapaTamañoObjetivoX));
    rect.y = static_cast<int>((-esquinaY - mapaMinY) * factorY + (height - minimapaTamañoObjetivoY));
    rect.w = static_cast<int>(width * factorX);
    rect.h = static_cast<int>(height * factorY);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

auto Window::rendererPresent() const -> void
{
    SDL_RenderPresent(renderer);
}

auto Window::limpiar() const -> void
{
    rendererClear();
}

auto Window::render(bool rendererDraw_) const -> void
{
    if(rendererDraw_)
        rendererDraw();
    rendererPresent();
}

auto Window::moverRel(const int x, const int y) -> void
{
    esquinaX += x;
    esquinaY += y;
}

auto Window::mover(const int x, const int y) -> void
{
    esquinaX = x;
    esquinaY = y;
}

auto Window::centrar() -> void
{
    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();

    const std::vector<IMG*>* imagenes = listaIMG.getLista(ListaIMG::Medio);

    if(imagenes == nullptr)
    {
        mover(0, 0);
        return;
    }

    for(IMG* img : *imagenes)
    {
        const SDL_Rect* rect{img->getRect()};

        minX = std::min(rect->x, minX);
        maxX = std::max(rect->x + rect->w, maxX);
        minY = std::min(rect->y, minY);
        maxY = std::max(rect->y + rect->h, maxY);
    }


    int x = (maxX + minX) / 2;
    int y = (maxY + minY) / 2;

    x -= width / 2;
    y -= height / 2;

    mover(-x, -y);
}
