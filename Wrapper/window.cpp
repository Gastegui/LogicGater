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
    const ListaImg* lista{fondo};
    const ListaLineas* lista2{lineas};
    std::pair<int, int>* a;
    std::pair<int, int>* b;
    while(lista != nullptr)
    {
        SDL_RenderCopy(renderer, lista->img->getTexture(), nullptr, lista->img->getRect());
        lista = lista->siguiente;
    }
    lista = medio;
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

    lista = frente;
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

bool Window::añadir(IMG* img, const Altura altura, const bool final)
{
    ListaImg* lista;
    switch(altura)
    {
    case FONDO:
        lista = fondo;
        break;
    case MEDIO:
        lista = medio;
        break;
    case FRENTE:
        lista = frente;
        break;
    case INVALIDO:
    default:
        return false;
    }

    if(lista == nullptr)
    {
        switch(altura)
        {
        case FONDO:
            fondo = new ListaImg;
            lista = fondo;
            break;
        case MEDIO:
            medio = new ListaImg;
            lista = medio;
            break;
        case FRENTE:
            frente = new ListaImg;
            lista = frente;
        case INVALIDO:
            break;
        }
        lista->img = img;
        lista->siguiente = nullptr;
    }
    else if (final)
    {
        while(lista->siguiente != nullptr)
        {
            lista = lista->siguiente;
        }

        lista->siguiente = new ListaImg;
        lista = lista->siguiente;
        lista->img = img;
        lista->siguiente = nullptr;
    }
    else
    {
        const auto tmp = new ListaImg;
        tmp->img = img;
        tmp->siguiente = lista;
        switch (altura)
        {
        case FONDO:
            fondo = tmp;
            break;
        case MEDIO:
            medio = tmp;
            break;
        case FRENTE:
            frente = tmp;
        case INVALIDO:
            break;
        }
    }
    if(img->getClickable())
        raton.añadir(img, altura, !final);
    return true;
}

//No borra la imagen. Eso es trabajo de la clase IMG
bool Window::borrar(const IMG* img, const Altura altura)
{
    ListaImg* lista;

    switch (altura)
    {
    case FRENTE:
        lista = frente;
        break;
    case MEDIO:
        lista = medio;
        break;
    case FONDO:
        lista = fondo;
        break;
    case INVALIDO:
    default:
        return false;
    }

    if(lista == nullptr) //La lista está vacia
        return false;

    if(lista->siguiente == nullptr) //Solo hay un elemento
    {
        if(lista->img == img) //y coincide
        {
            delete lista;
            switch (altura)
            {
            case FRENTE:
                frente = nullptr;
                break;
            case MEDIO:
                medio = nullptr;
                break;
            case FONDO:
                fondo = nullptr;
            case INVALIDO:
                break;
            }
            raton.borrar(img, altura);
            return true;
        }

        return false; // y no coincide
    }

    if(lista->img == img) //Es el primer elemento de la lista
    {
        const ListaImg* tmp = lista;

        switch (altura)
        {
        case FRENTE:
            frente = frente->siguiente;
            break;
        case MEDIO:
            medio = medio->siguiente;
            break;
        case FONDO:
            fondo = fondo->siguiente;
        case INVALIDO:
            break;
        }

        delete tmp;
        return true;
    }

    ListaImg* atras{lista};
    lista = lista->siguiente;
    while(lista != nullptr && lista->img != img)
    {
        atras = lista;
        lista = lista->siguiente;
    }

    if(lista == nullptr || lista->img != img)
        return false;

    atras->siguiente = lista->siguiente;

    delete lista;

    if(img->getClickable())
        raton.borrar(img, altura);

    return true;
}

//No borra las imagenes. Eso es trabajo de la clase IMG
bool Window::borrarTodo(const Altura altura)
{
    ListaImg* lista;
    // ReSharper disable once CppDFAUnusedValue
    const ListaImg* tmp{nullptr};

    switch (altura)
    {
    case FONDO:
        lista = fondo;
        break;
    case MEDIO:
        lista = medio;
        break;
    case FRENTE:
        lista = frente;
        break;
    case INVALIDO:
    default:
        return false;
    }

    while(lista != nullptr)
    {
        tmp = lista;
        lista = lista->siguiente;
        delete tmp;
    }

    switch (altura)
    {
    case FONDO:
        fondo = nullptr;
        break;
    case MEDIO:
        medio = nullptr;
        break;
    case FRENTE:
        frente = nullptr;
    case INVALIDO:
        break;
    }

    raton.borrarTodo(altura);
    return true;
}

void Window::manejarRaton(const SDL_Event* evento)
{
    raton.manejarRaton(evento);
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

/* BORRAR LINEA
bool Window::borrarLinea(const Puerta* origen, const Puerta* destino, const bool arriba)
{
    if(lineas == nullptr)
        return false;

    ListaLineas* anterior{lineas};

    if(lineas->origenPuerta == origen && lineas->destinoPuerta == destino && lineas->puertaArriba == arriba)
    {
        if(lineas->siguiente != nullptr)
        {
            lineas = lineas->siguiente;
        }
        delete anterior;
        return true;
    }
    ListaLineas* lista{lineas->siguiente};

    while(lista->siguiente != nullptr && lista->origenPuerta != origen && lista->destinoPuerta != destino && lista->puertaArriba != arriba)
    {
        anterior = lista;
        lista = lista->siguiente;
    }

    if(lista->origenPuerta == origen && lista->destinoPuerta == destino && lista->puertaArriba == arriba)
    {
        anterior->siguiente = lista->siguiente;
        delete lista;
        return true;
    }

    return false;
}

bool Window::borrarLinea(const Puerta* origen, const Salida* destino)
{
    if(lineas == nullptr)
        return false;

    ListaLineas* anterior{lineas};

    if(lineas->origenPuerta == origen && lineas->destinoSalida == destino)
    {
        if(lineas->siguiente != nullptr)
        {
            lineas = lineas->siguiente;
        }
        delete anterior;
        return true;
    }
    ListaLineas* lista{lineas->siguiente};

    while(lista->siguiente != nullptr && lista->origenPuerta != origen && lista->destinoSalida != destino)
    {
        anterior = lista;
        lista = lista->siguiente;
    }

    if(lista->origenPuerta == origen && lista->destinoSalida == destino)
    {
        anterior->siguiente = lista->siguiente;
        delete lista;
        return true;
    }

    return false;
}

bool Window::borrarLinea(const Entrada* origen, const Salida* destino)
{
    if(lineas == nullptr)
        return false;

    ListaLineas* anterior{lineas};

    if(lineas->origenEntrada == origen && lineas->destinoSalida == destino)
    {
        if(lineas->siguiente != nullptr)
        {
            lineas = lineas->siguiente;
        }
        delete anterior;
        return true;
    }
    ListaLineas* lista{lineas->siguiente};

    while(lista->siguiente != nullptr && lista->origenEntrada != origen && lista->destinoSalida != destino)
    {
        anterior = lista;
        lista = lista->siguiente;
    }

    if(lista->origenEntrada == origen && lista->destinoSalida == destino)
    {
        anterior->siguiente = lista->siguiente;
        delete lista;
        return true;
    }

    return false;
}

bool Window::borrarLinea(const Entrada* origen, const Puerta* destino, const bool arriba)
{
    if(lineas == nullptr)
        return false;

    ListaLineas* anterior{lineas};

    if(lineas->origenEntrada == origen && lineas->destinoPuerta == destino && lineas->puertaArriba == arriba)
    {
        if(lineas->siguiente != nullptr)
        {
            lineas = lineas->siguiente;
        }
        delete anterior;
        return true;
    }
    ListaLineas* lista{lineas->siguiente};

    while(lista->siguiente != nullptr && lista->origenEntrada != origen && lista->destinoPuerta == destino && lista->puertaArriba != arriba)
    {
        anterior = lista;
        lista = lista->siguiente;
    }

    if(lista->origenEntrada == origen && lista->destinoPuerta == destino && lista->puertaArriba == arriba)
    {
        anterior->siguiente = lista->siguiente;
        delete lista;
        return true;
    }

    return false;
}
*/

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
