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
    if(linea->io_origen->get())
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    else
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Window::rendererDraw() const
{
    const ListaIMG::Lista* lista{listaIMG.getLista(ListaIMG::FONDO)};
    const ListaLineas::Lista* lista2{controlador->getListaLineas()->getLista()};
    while(lista != nullptr)
    {
        SDL_RenderCopy(renderer, lista->img->getTexture(), nullptr, lista->img->getRect());
        lista = lista->siguiente;
    }
    lista = listaIMG.getLista(ListaIMG::MEDIO);
    while(lista != nullptr)
    {
        SDL_Rect rect{*lista->img->getRect()};
        if(rect.x < -esquinaX + width && rect.x + rect.w > -esquinaX && rect.y < -esquinaY + height && rect.y + rect.h > -esquinaY)
        {
            rect.x += esquinaX;
            rect.y += esquinaY;
            SDL_RenderCopy(renderer, lista->img->getTexture(), nullptr, &rect);
        }
        lista = lista->siguiente;
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while(lista2 != nullptr)
    {
        const std::pair<int, int>* origen = lista2->io_origen->getLineaOrigen();   //TODO: ¡¡ESTOS DOS SON LOS MISMOS!!
        const std::pair<int, int>* destino = lista2->io_destino->getLineaDestino(); //¡¡El destino y el origen son el mismo IO!!

        //Comprueba si alguno de los dos puntos de la línea está dentro de la pantalla
        if((origen->first > -esquinaX && origen->first < -esquinaX + width && origen->second > -esquinaY && origen->second < -esquinaY + height) ||
                (destino->first > -esquinaX && destino->first < -esquinaX + width && destino->second > -esquinaY && destino->second < -esquinaY + height))
            renderLine(renderer, origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX, destino->second + esquinaY, lista2);
        else
        {
            //Si no esta el origen o final de la línea dentro de la pantalla, comprueba si la línea intersecciona la pantalla
            SDL_Rect rect{-esquinaX, -esquinaY, width, height};
            int x1 = origen->first;
            int y1 = origen->second;
            int x2 = destino->first;
            int y2 = destino->second;
            if(SDL_IntersectRectAndLine(&rect, &x1, &y1, &x2, &y2))
                renderLine(renderer, origen->first + esquinaX, origen->second + esquinaY, destino->first + esquinaX, destino->second + esquinaY, lista2);
        }
        lista2 = lista2->siguiente;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    lista = listaIMG.getLista(ListaIMG::FRENTE);
    while(lista != nullptr)
    {
        SDL_RenderCopy(renderer, lista->img->getTexture(), nullptr, lista->img->getRect());
        lista = lista->siguiente;
    }
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