//
// Created by julen on 23/08/24.
//

#include "controlador.h"

void Controlador::desmarcarOrigen()
{
    origen = nullptr;
}

void Controlador::crear(const Puerta::Tipo tipo, const int x, const int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada)
{
    Puerta* puerta = new Puerta{renderer, tipo, x, y, arribaNegado, abajoNegado, salidaNegada};
    window->añadir(puerta->getIMG(), Window::MEDIO);
    if(listaPuertas == nullptr)
    {
        listaPuertas = new ListaPuertas;
        listaPuertas->puerta = puerta;
        listaPuertas->siguiente = nullptr;
    }
    else
    {
        ListaPuertas* tmp{listaPuertas};
        while(tmp->siguiente != nullptr)
        {
            tmp = tmp->siguiente;
        }

        tmp->siguiente = new ListaPuertas;
        tmp = tmp->siguiente;
        tmp->puerta = puerta;
        tmp->siguiente = nullptr;
    }
}

void Controlador::borrarConexiones(Puerta* puerta, const bool arriba, const bool abajo, const bool salida) const
{
    const ListaPuertas* tmp{listaPuertas};

    window->borrarLineas(puerta, arriba, abajo, salida);

    if(arriba)
        puerta->setAbajo(nullptr);

    if(abajo)
        puerta->setAbajo(nullptr);

    while(tmp != nullptr)
    {
        if(salida)
        {
            if(tmp->puerta->getArriba() == puerta->getSalida())
                tmp->puerta->setArriba(nullptr);
            if(tmp->puerta->getAbajo() == puerta->getSalida())
                tmp->puerta->setAbajo(nullptr);
        }
        tmp = tmp->siguiente;
    }
}


bool Controlador::borrar(Puerta* puerta)
{
    if(listaPuertas == nullptr)
        return false;

    if(listaPuertas->puerta == puerta) //Es la primra puerta de la listaPuertas
    {
        borrarConexiones(puerta);

        if(listaPuertas->siguiente == nullptr) //Y es la unica puerta de la listaPuertas
        {
            window->borrar(listaPuertas->puerta->getIMG(), Window::MEDIO);
            delete listaPuertas->puerta;
            delete listaPuertas;
            listaPuertas = nullptr;
        }
        else //Y hay más elementos
        {
            const ListaPuertas* tmp{listaPuertas};
            listaPuertas = listaPuertas->siguiente;
            window->borrar(tmp->puerta->getIMG(), Window::MEDIO);
            delete tmp->puerta;
            delete tmp;
        }
    }
    else //No es la primera puerta de la listaPuertas
    {
        ListaPuertas* anterior{listaPuertas};
        const ListaPuertas* tmp{anterior->siguiente};

        while(tmp->siguiente != nullptr && tmp->puerta != puerta)
        {
            anterior = anterior->siguiente;
            tmp = tmp->siguiente;
        }

        if(tmp->puerta == puerta)
        {
            borrarConexiones(puerta);
            anterior->siguiente = tmp->siguiente;
            window->borrar(tmp->puerta->getIMG(), Window::MEDIO);
            delete tmp->puerta;
            delete tmp;
        }
        else
            return false;
    }

    return true;
}

void Controlador::marcarOrigen(Puerta* puerta)
{
    origen = puerta->getSalida();
}

void Controlador::destino(Puerta* puerta, const bool arriba)
{
    if(origen == nullptr)
        return;

    if(arriba)
        puerta->setArriba(origen);
    else
        puerta->setAbajo(origen);

    if(origen->getPadreEntrada() != nullptr)
        window->añadirLinea(origen->getPadreEntrada(), puerta, arriba);
    else
        window->añadirLinea(origen->getPadrePuerta(), puerta, arriba);

    origen = nullptr;
}

void Controlador::crear(const bool mantener, const int x, const int y)
{
    Entrada* entrada = new Entrada{renderer, x, y, mantener};
    window->añadir(entrada->getImg(), Window::MEDIO);

    if(listaEntradas == nullptr)
    {
        listaEntradas = new ListaEntradas;
        listaEntradas->entrada = entrada;
        listaEntradas->siguiente = nullptr;
    }
    else
    {
        ListaEntradas* tmp{listaEntradas};
        while(tmp->siguiente != nullptr)
        {
            tmp = tmp->siguiente;
        }

        tmp->siguiente = new ListaEntradas;
        tmp = tmp->siguiente;
        tmp->entrada = entrada;
        tmp->siguiente = nullptr;
    }
}

void Controlador::borrarConexiones(Entrada* entrada) const
{
    const ListaPuertas* lista{listaPuertas};

    window->borrarLineas(entrada);

    while(lista != nullptr)
    {
        if(lista->puerta->getArriba() == entrada->getSalida())
            lista->puerta->setArriba(nullptr);
        if(lista->puerta->getAbajo() == entrada->getSalida())
            lista->puerta->setAbajo(nullptr);

        lista = lista->siguiente;
    }
}


bool Controlador::borrar(Entrada* entrada)
{
    if(listaEntradas == nullptr)
        return false;

    if(listaEntradas->entrada == entrada) //Es el primer entrada de listaEntradas
    {
        borrarConexiones(entrada);

        if(listaEntradas->siguiente == nullptr) //Y es el unico entrada de listaEntradas
        {
            window->borrar(listaEntradas->entrada->getImg(), Window::MEDIO);
            delete listaEntradas->entrada;
            delete listaEntradas;
            listaEntradas = nullptr;
        }
        else //Y hay más elementos
        {
            const ListaEntradas* tmp{listaEntradas};
            listaEntradas = listaEntradas->siguiente;
            window->borrar(listaEntradas->entrada->getImg(), Window::MEDIO);
            delete tmp->entrada;
            delete tmp;
        }
    }
    else //No es el primer entrada de listaEntradas
    {
        ListaEntradas* anterior{listaEntradas};
        const ListaEntradas* tmp{anterior->siguiente};

        while(tmp->siguiente != nullptr && tmp->entrada != entrada)
        {
            anterior = anterior->siguiente;
            tmp = tmp->siguiente;
        }

        if(tmp->entrada == entrada)
        {
            borrarConexiones(entrada);
            anterior->siguiente = tmp->siguiente;
            window->borrar(tmp->entrada->getImg(), Window::MEDIO);
            delete tmp->entrada;
            delete tmp;
        }
        else
            return false;
    }

    return true;
}

void Controlador::marcarOrigen(Entrada* boton)
{
    origen = boton->getSalida();
}

void Controlador::crear(const int x, const int y)
{
    Salida* salida = new Salida{renderer, x, y};
    window->añadir(salida->getImg(), Window::MEDIO);

    if(listaSalidas == nullptr)
    {
        listaSalidas = new ListaSalidas;
        listaSalidas->salida = salida;
        listaSalidas->siguiente = nullptr;
    }
    else
    {
        ListaSalidas* tmp{listaSalidas};
        while(tmp->siguiente != nullptr)
        {
            tmp = tmp->siguiente;
        }

        tmp->siguiente = new ListaSalidas;
        tmp = tmp->siguiente;
        tmp->salida = salida;
        tmp->siguiente = nullptr;
    }
}

bool Controlador::borrar(Salida* salida)
{
    if(listaSalidas == nullptr)
        return false;

    if(listaSalidas->salida == salida) //Es la primera salida de listaSalidas
    {
        borrarConexiones(salida);

        if(listaSalidas->siguiente == nullptr) //Y es la unica salida de listaSalidas
        {
            window->borrar(listaSalidas->salida->getImg(), Window::MEDIO);
            delete listaSalidas->salida;
            delete listaSalidas;
            listaSalidas = nullptr;
        }
        else //Y hay más elementos
        {
            const ListaSalidas* tmp{listaSalidas};
            listaSalidas = listaSalidas->siguiente;
            window->borrar(listaSalidas->salida->getImg(), Window::MEDIO);
            delete tmp->salida;
            delete tmp;
        }
    }
    else //No es la primera salida de listaSalidas
    {
        ListaSalidas* anterior{listaSalidas};
        const ListaSalidas* tmp{anterior->siguiente};

        while(tmp->siguiente != nullptr && tmp->salida != salida)
        {
            anterior = anterior->siguiente;
            tmp = tmp->siguiente;
        }

        if(tmp->salida == salida)
        {
            borrarConexiones(salida);
            anterior->siguiente = tmp->siguiente;
            window->borrar(tmp->salida->getImg(), Window::MEDIO);
            delete tmp->salida;
            delete tmp;
        }
        else
            return false;
    }

    return true;
}

void Controlador::borrarConexiones(Salida* salida) const
{
    window->borrarLineas(salida);
    salida->setEntrada(nullptr);
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Controlador::destino(Salida* salida)
{
    if(origen == nullptr)
        return;
    salida->setEntrada(origen);

    if(origen->getPadreEntrada() != nullptr)
        window->añadirLinea(origen->getPadreEntrada(), salida);
    else
        window->añadirLinea(origen->getPadrePuerta(), salida);

    origen = nullptr;
}


void Controlador::simular() const
{
    const ListaSalidas* lista{listaSalidas};

    while(lista != nullptr)
    {
        lista->salida->simular();
        lista = lista->siguiente;
    }

    const ListaPuertas* listaP{listaPuertas};

    while(listaP != nullptr)
    {
        listaP->puerta->simulacionTermindada();
        listaP = listaP->siguiente;
    }
}
