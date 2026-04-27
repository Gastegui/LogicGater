//
// Created by julen on 22/08/24.
//

#include <SDL_rect.h>
#include <SDL_render.h>
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

auto Window::renderLine(const int x1, const int y1, const int x2, const int y2, const ListaLineas::Linea *linea) const -> void
{
    if(linea->io->get())
    {
        if(linea->oculta)
            SDL_SetRenderDrawColor(renderer, 0, 150, 0, 255);
        else
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    }
    else
    {
        if(linea->oculta)
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        else
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    }
    if(!linea->oculta || (linea->oculta && Controlador::get().getConexionesOcultas()))
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

auto Window::rendererDraw() const -> void
{
    // ReSharper disable CppDFANullDereference
    const std::vector<IMG*>* imagenes{listasIMG.at(Controlador::get().getEstado()).getLista(ListaIMG::Fondo)};
    const std::vector<ListaLineas::Linea>* lineas{Controlador::get().getListaLineas()->getLista()};
    mapaMinX = -esquinaX;
    mapaMaxX = -esquinaX + width;
    mapaMinY = -esquinaY;
    mapaMaxY = -esquinaY + height;

    if(Controlador::get().getCuadriculaActiva())
    {
        SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
        for(int i = -esquinaX; i < -esquinaX + width; i++)
            if(i % Controlador::get().getCuadriculaTamaño() == 0)
                SDL_RenderDrawLine(renderer, esquinaX + i, 0, esquinaX + i, esquinaX + i + height);

        for(int i = -esquinaY; i < -esquinaY + height; i++)
            if(i % Controlador::get().getCuadriculaTamaño() == 0)
                SDL_RenderDrawLine(renderer, 0, esquinaY + i, esquinaY + i + width, esquinaY + i);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
    // ReSharper restore CppDFANullDereference
    for(IMG* img : *imagenes)
        SDL_RenderCopy(renderer, img->getTexture(), nullptr, img->getRect());

    imagenes = listasIMG.at(Controlador::get().getEstado()).getLista(ListaIMG::Medio);
    for(IMG* img : *imagenes | std::views::reverse)
    {
        SDL_Rect rectColision{*img->getRect()};

        if(rectColision.x < mapaMinX)
            mapaMinX = rectColision.x;
        else if(rectColision.x + rectColision.w > mapaMaxX)
            mapaMaxX = rectColision.x + rectColision.w;
        if(rectColision.y < mapaMinY)
            mapaMinY = rectColision.y;
        else if(rectColision.y + rectColision.h > mapaMaxY)
            mapaMaxY = rectColision.y + rectColision.h;

        if(rectColision.x < -esquinaX + width && rectColision.x + rectColision.w > -esquinaX && rectColision.y < -esquinaY + height && rectColision.y + rectColision.h > -esquinaY)
        {
            SDL_Rect rectReal = *img->getRectReal();
            rectColision.x += esquinaX;
            rectColision.y += esquinaY;
            rectReal.x += esquinaX;
            rectReal.y += esquinaY;

            SDL_RenderCopyEx(renderer, img->getTexture(), nullptr, &rectReal, img->getRotacion(), nullptr, SDL_FLIP_NONE);
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
            renderLine(origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX,
                       destino->second + esquinaY, &linea);
        else
        {
            //Si no esta el origen o final de la línea dentro de la pantalla, comprueba si la línea intersecciona la pantalla
            int x1 = origen->first;
            int y1 = origen->second;
            int x2 = destino->first;
            int y2 = destino->second;
            if(SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2))
                renderLine(origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX,
                           destino->second + esquinaY, &linea);
        }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    imagenes = listasIMG.at(Controlador::get().getEstado()).getLista(ListaIMG::Frente);
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

    if(Controlador::get().getEstado() == ESTADOS::Normal)
    {
        //Minimapa
        const double factorX = static_cast<double>(minimapaTamañoObjetivoX) / (mapaMaxX - mapaMinX);
        const double factorY = static_cast<double>(minimapaTamañoObjetivoY) / (mapaMaxY - mapaMinY);
        imagenes = listasIMG.at(Controlador::get().getEstado()).getLista(ListaIMG::Medio);
        for(IMG* img : *imagenes)
        {
            rect = *img->getRectReal();
            rect.x = static_cast<int>((rect.x - mapaMinX) * factorX + (width - minimapaTamañoObjetivoX));
            rect.y = static_cast<int>((rect.y - mapaMinY) * factorY + (height - minimapaTamañoObjetivoY));
            rect.w = static_cast<int>(rect.w * factorX);
            rect.h = static_cast<int>(rect.h * factorY);
            SDL_RenderCopyEx(renderer, img->getTexture(), nullptr, &rect, img->getRotacion(), nullptr, SDL_FLIP_NONE);
        }
    
        rect.x = static_cast<int>((-esquinaX - mapaMinX) * factorX + (width - minimapaTamañoObjetivoX));
        rect.y = static_cast<int>((-esquinaY - mapaMinY) * factorY + (height - minimapaTamañoObjetivoY));
        rect.w = static_cast<int>(width * factorX);
        rect.h = static_cast<int>(height * factorY);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    }
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

    const std::vector<IMG*>* imagenes = listasIMG[Controlador::get().getEstado()].getLista(ListaIMG::Medio);

    if(imagenes == nullptr || imagenes->size() == 0)
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

auto Window::getListaIMG(const ListaIMG::ALTURA altura) const -> const std::vector<IMG *>*
{
    return listasIMG.at(Controlador::get().getEstado()).getLista(altura);
}

auto Window::añadir(ESTADOS estado, IMG *img, ListaIMG::ALTURA altura) -> bool
{
    return listasIMG[estado].añadir(img, altura);
}
auto Window::añadir(IMG* img, const ListaIMG::ALTURA altura) -> bool
{
    return añadir(Controlador::get().getEstado(), img, altura);
}

auto Window::borrar(ESTADOS estado, const IMG* img, const ListaIMG::ALTURA altura) -> bool
{
    if(listasIMG[estado].quitar(img, altura))
    {
        Raton::get().setImgAnteriorNull();
        return true;
    }
    return false;

}
auto Window::borrar(const IMG* img, const ListaIMG::ALTURA altura) -> bool
{
    return borrar(Controlador::get().getEstado(), img, altura);
}
