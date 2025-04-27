//
// Created by julen on 22/08/24.
//

#include "window.h"
#include "image.h"
#include "../Simulables/entrada.h"
#include "../Simulables/puerta.h"
#include "../controlador.h"

void Window::rendererClear() const
{
    SDL_RenderClear(renderer);
}

void Window::renderLine(SDL_Renderer* renderer, const int x1, const int y1, const int x2, const int y2, const ListaLineas::Lista* linea)
{
    if(linea->io->get())
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Window::rendererDraw() const
{
    const ListaIMG::Lista* imagenes{listaIMG.getLista(ListaIMG::FONDO)};
    const ListaLineas::Lista* lineas{controlador->getListaLineas()->getLista()};
    mapaMinX = -esquinaX;
    mapaMaxX = -esquinaX + width;
    mapaMinY = -esquinaY;
    mapaMaxY = -esquinaY + height;

    while(imagenes != nullptr)
    {
        SDL_RenderCopy(renderer, imagenes->img->getTexture(), nullptr, imagenes->img->getRect());
        imagenes = imagenes->siguiente;
    }
    imagenes = listaIMG.getLista(ListaIMG::MEDIO);
    while(imagenes != nullptr)
    {
        SDL_Rect rect{*imagenes->img->getRect()};

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
            SDL_RenderCopy(renderer, imagenes->img->getTexture(), nullptr, &rect);
        }
        imagenes = imagenes->siguiente;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect{-esquinaX, -esquinaY, width, height};
    while(lineas != nullptr)
    {
        const std::pair<int, int>* origen = lineas->io->getLineaOrigen();
        const std::pair<int, int>* destino = lineas->destino->getLinea(lineas->conexion);


        //Comprueba si alguno de los dos puntos de la línea está dentro de la pantalla
        if((origen->first > -esquinaX && origen->first < -esquinaX + width && origen->second > -esquinaY && origen->second < -esquinaY + height) ||
                (destino->first > -esquinaX && destino->first < -esquinaX + width && destino->second > -esquinaY && destino->second < -esquinaY + height))
            renderLine(renderer, origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX, destino->second + esquinaY, lineas);
        else
        {
            //Si no esta el origen o final de la línea dentro de la pantalla, comprueba si la línea intersecciona la pantalla
            int x1 = origen->first;
            int y1 = origen->second;
            int x2 = destino->first;
            int y2 = destino->second;
            if(SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2))
                renderLine(renderer, origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX, destino->second + esquinaY, lineas);
        }
        lineas = lineas->siguiente;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    imagenes = listaIMG.getLista(ListaIMG::FRENTE);
    while(imagenes != nullptr)
    {
        SDL_RenderCopy(renderer, imagenes->img->getTexture(), nullptr, imagenes->img->getRect());
        imagenes = imagenes->siguiente;
    }

    //Minimapa
    const double factorX = static_cast<double>(minimapaTamañoObjetivoX) / (mapaMaxX - mapaMinX);
    const double factorY = static_cast<double>(minimapaTamañoObjetivoY) / (mapaMaxY - mapaMinY);
    imagenes = listaIMG.getLista(ListaIMG::MEDIO);
    while(imagenes != nullptr)
    {
        rect = *imagenes->img->getRect();
        rect.x = static_cast<int>((rect.x - mapaMinX) * factorX + (width - minimapaTamañoObjetivoX));
        rect.y = static_cast<int>((rect.y - mapaMinY) * factorY + (height - minimapaTamañoObjetivoY));
        rect.w = static_cast<int>(rect.w * factorX);
        rect.h = static_cast<int>(rect.h * factorY);
        SDL_RenderCopy(renderer, imagenes->img->getTexture(), nullptr, &rect);
        imagenes = imagenes->siguiente;
    }

    rect.x = static_cast<int>((-esquinaX - mapaMinX) * factorX + (width - minimapaTamañoObjetivoX));
    rect.y = static_cast<int>((-esquinaY - mapaMinY) * factorY + (height - minimapaTamañoObjetivoY));
    rect.w = static_cast<int>(width * factorX);
    rect.h = static_cast<int>(height * factorY);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Window::rendererPresent() const
{
    SDL_RenderPresent(renderer);
}

void Window::limpiar() const
{
    rendererClear();
}

void Window::render() const
{
    rendererDraw();
    rendererPresent();
}

void Window::moverRel(const int x, const int y)
{
    esquinaX += x;
    esquinaY += y;
}

void Window::mover(const int x, const int y)
{
    esquinaX = x;
    esquinaY = y;
}