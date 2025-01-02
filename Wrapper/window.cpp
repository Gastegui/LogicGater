//
// Created by julen on 22/08/24.
//

#include "window.h"
#include "image.h"
#include "../entrada.h"
#include "../puerta.h"
#include "../salida.h"

void Window::rendererClear() const
{
    SDL_RenderClear(renderer);
}

void Window::rendererDraw() const
{
    const ListaIMG::Lista* lista{listaIMG.getLista(ListaIMG::FONDO)};
    const ListaLineas* lista2{lineas};
    std::pair<int, int>* a;
    std::pair<int, int>* b;
    while(lista != nullptr)
    {
        SDL_RenderCopy(renderer, lista->img->getTexture(), nullptr, lista->img->getRect());
        lista = lista->siguiente;
    }
    lista = listaIMG.getLista(ListaIMG::MEDIO);
    while(lista != nullptr)
    {
        SDL_RenderCopy(renderer, lista->img->getTexture(), nullptr, lista->img->getRect());
        lista = lista->siguiente;
    }


    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    while(lista2 != nullptr)
    {
        if(lista2->origenPuerta)
            a = lista2->origenPuerta->getLineaSalidePos();
        else
            a = lista2->origenEntrada->getLineaPos();

        if(lista2->destinoPuerta)
            b = lista2->puertaArriba ? lista2->destinoPuerta->getLineaArribaPos() : lista2->destinoPuerta->getLineaAbajoPos();
        else
            b = lista2->destinoSalida->getLineaPos();

        SDL_RenderDrawLine(renderer, a->first, a->second, b->first, b->second);
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

void Window::manejarRaton()
{
    raton.manejarRaton();
}


void Window::añadirLinea(Puerta* origen, Puerta* destino, const bool arriba)
{
    ListaLineas* lista{lineas};

    if(lineas == nullptr)
    {
        lineas = new ListaLineas;
        lista = lineas;
    }
    else
    {
        while(lista->siguiente != nullptr)
            lista = lista->siguiente;
        lista->siguiente = new ListaLineas;
        lista = lista->siguiente;
    }

    lista->siguiente = nullptr;
    lista->origenPuerta = origen;
    lista->destinoPuerta = destino;
    lista->puertaArriba = arriba;
}

void Window::añadirLinea(Puerta* origen, Salida* destino)
{
    ListaLineas* lista{lineas};

    if(lineas == nullptr)
    {
        lineas = new ListaLineas;
        lista = lineas;
    }
    else
    {
        while(lista->siguiente != nullptr)
            lista = lista->siguiente;
        lista->siguiente = new ListaLineas;
        lista = lista->siguiente;
    }

    lista->siguiente = nullptr;
    lista->origenPuerta = origen;
    lista->destinoSalida = destino;
}

void Window::añadirLinea(Entrada* origen, Puerta* destino, const bool arriba)
{
    ListaLineas* lista{lineas};

    if(lineas == nullptr)
    {
        lineas = new ListaLineas;
        lista = lineas;
    }
    else
    {
        while(lista->siguiente != nullptr)
            lista = lista->siguiente;
        lista->siguiente = new ListaLineas;
        lista = lista->siguiente;
    }

    lista->siguiente = nullptr;
    lista->origenEntrada = origen;
    lista->destinoPuerta = destino;
    lista->puertaArriba = arriba;
}

void Window::añadirLinea(Entrada* origen, Salida* destino)
{
    ListaLineas* lista{lineas};

    if(lineas == nullptr)
    {
        lineas = new ListaLineas;
        lista = lineas;
    }
    else
    {
        while(lista->siguiente != nullptr)
            lista = lista->siguiente;
        lista->siguiente = new ListaLineas;
        lista = lista->siguiente;
    }

    lista->siguiente = nullptr;
    lista->origenEntrada = origen;
    lista->destinoSalida = destino;
}

void Window::borrarLineas(const Puerta* puerta, const bool arriba, const bool abajo, const bool salida)
{
    if(lineas == nullptr || puerta == nullptr)
        return;

    ListaLineas* lista{lineas};
    ListaLineas* anterior{nullptr};
    int i{arriba + abajo};

    while(lista != nullptr)
    {
        if( (arriba && lista->destinoPuerta == puerta && lista->puertaArriba) || //La conexion de la entrada de arriba
            (abajo && lista->destinoPuerta == puerta && !lista->puertaArriba) || //La conexion de la entrada de abajo
            (salida && lista->origenPuerta == puerta))                           //Todas sus salidas
        {
            if(anterior != nullptr) //No es el primero de la lista
            {
                lista = lista->siguiente; //Si se está borrando el último de la lista se pondrá nullptr aquí
                delete anterior->siguiente;
                anterior->siguiente = lista;
            }
            else //Es el primero de la lista
            {
                if(lineas->siguiente == nullptr) //Es el único de la lista
                {
                    delete lineas;
                    lineas = nullptr;
                    return;
                }
                //No es el único de la lista
                lineas = lineas->siguiente;
                anterior = lista;
                lista = lista->siguiente;
                delete anterior;
                anterior = nullptr;
            }

            if(!salida && i > 0) //Solo puede haber una única conexion en cada entrada, por lo que no tiene sentido seguir buscando
            {
                i--;
                if(i == 0)
                    return;
            }

        }
        else //No ha conincidido con los filtros
        {
            anterior = lista;
            lista = lista->siguiente;
        }
    }
}


void Window::borrarLineas(const Entrada* entrada)
{
    if(lineas == nullptr || entrada == nullptr)
        return;

    ListaLineas* lista{lineas};
    ListaLineas* anterior{nullptr};

    while(lista != nullptr)
    {
        if(lista->origenEntrada == entrada)
        {
            if(anterior != nullptr) //No es el primero de la lista
            {
                lista = lista->siguiente; //Si se está borrando el último de la lista se pondrá nullptr aquí
                delete anterior->siguiente;
                anterior->siguiente = lista;
            }
            else //Es el primero de la lista
            {
                if(lineas->siguiente == nullptr) //Es el único de la lista
                {
                    delete lineas;
                    lineas = nullptr;
                    return;
                }
                //No es el único de la lista
                lineas = lineas->siguiente;
                anterior = lista;
                lista = lista->siguiente;
                delete anterior;
                anterior = nullptr;
            }
        }
        else //No ha conincidido con los filtros
        {
            anterior = lista;
            lista = lista->siguiente;
        }
    }
}

void Window::borrarLineas(const Salida* salida)
{
    if(lineas == nullptr || salida == nullptr)
        return;

    ListaLineas* lista{lineas};
    ListaLineas* anterior{nullptr};

    while(lista != nullptr)
    {
        if(lista->destinoSalida == salida)
        {
            if(anterior != nullptr) //No es el primero de la lista
            {
                lista = lista->siguiente; //Si se está borrando el último de la lista se pondrá nullptr aquí
                delete anterior->siguiente;
                anterior->siguiente = lista;
            }
            else //Es el primero de la lista
            {
                if(lineas->siguiente == nullptr) //Es el único de la lista
                {
                    delete lineas;
                    lineas = nullptr;
                    return;
                }
                //No es el único de la lista
                lineas = lineas->siguiente;
                anterior = lista;
                lista = lista->siguiente;
                delete anterior;
                anterior = nullptr;
            }
        }
        else //No ha conincidido con los filtros
        {
            anterior = lista;
            lista = lista->siguiente;
        }
    }
}
