//
// Created by julen on 23/08/24.
//

#include "controlador.h"

void Controlador::desmarcarOrigen()
{
    origen = nullptr;
}

void Controlador::crear(const Puerta::Tipo tipo, int x, int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

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

void Controlador::borrarConexiones(Puerta* puerta, const bool arriba, const bool abajo, const bool salida)
{
    if(origen != nullptr)
        origen = nullptr;

    window->borrarLineas(puerta, arriba, abajo, salida);

    if(arriba && puerta->getArriba() != nullptr)
    {
        puerta->getArriba()->desconectado();
        puerta->setArriba(nullptr);
    }

    if(abajo && puerta->getAbajo() != nullptr)
    {
        puerta->getAbajo()->desconectado();
        puerta->setAbajo(nullptr);
    }

    if(salida && puerta->getSalida()->getConexiones() != 0)
    {
        const ListaPuertas* listaP{listaPuertas};
        while(listaP != nullptr)
        {
            if(listaP->puerta->getArriba() == puerta->getSalida())
            {
                puerta->getSalida()->desconectado();
                listaP->puerta->setArriba(nullptr);
            }
            if(listaP->puerta->getAbajo() == puerta->getSalida())
            {
                puerta->getSalida()->desconectado();
                listaP->puerta->setAbajo(nullptr);
            }
            listaP = listaP->siguiente;
        }

        const ListaSalidas* listaS{listaSalidas};
        while(listaS != nullptr)
        {
            if(listaS->salida->getEntrada() == puerta->getSalida())
            {
                puerta->getSalida()->desconectado();
                listaS->salida->setEntrada(nullptr);
            }
            listaS = listaS->siguiente;
        }
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
    if(origen == nullptr || (arriba && puerta->getArriba() != nullptr) || (!arriba && puerta->getAbajo() != nullptr))
        return;

    if(arriba)
        puerta->setArriba(origen);
    else
        puerta->setAbajo(origen);

    if(origen->getPadreEntrada() != nullptr)
        window->añadirLinea(origen->getPadreEntrada(), puerta, arriba);
    else
        window->añadirLinea(origen->getPadrePuerta(), puerta, arriba);

    origen->conectado();

    origen = nullptr;
}

void Controlador::crear(const bool mantener, int x, int y)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

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
    const ListaPuertas* listaP{listaPuertas};

    window->borrarLineas(entrada);

    while(listaP != nullptr)
    {
        if(listaP->puerta->getArriba() == entrada->getSalida())
        {
            entrada->getSalida()->desconectado();
            listaP->puerta->setArriba(nullptr);
        }
        if(listaP->puerta->getAbajo() == entrada->getSalida())
        {
            entrada->getSalida()->desconectado();
            listaP->puerta->setAbajo(nullptr);
        }

        listaP = listaP->siguiente;
    }

    const ListaSalidas* listaS{listaSalidas}; //Acabo de añadir esto
    while(listaS != nullptr)
    {
        if(listaS->salida->getEntrada() == entrada->getSalida())
        {
            entrada->getSalida()->desconectado();
            listaS->salida->setEntrada(nullptr);
        }
        listaS = listaS->siguiente;
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

void Controlador::crear(int x, int y)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

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
    if(salida->getEntrada() == nullptr)
        return;

    window->borrarLineas(salida);
    salida->getEntrada()->desconectado();
    salida->setEntrada(nullptr);
}

void Controlador::destino(Salida* salida)
{
    if(origen == nullptr)
        return;
    salida->setEntrada(origen);

    if(origen->getPadreEntrada() != nullptr)
        window->añadirLinea(origen->getPadreEntrada(), salida);
    else
        window->añadirLinea(origen->getPadrePuerta(), salida);

    origen->conectado();
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

int Controlador::limpiar()
{
    int limpiados = 0;

    const ListaPuertas* puertas{listaPuertas};
    if(puertas != nullptr)
    {
        while(puertas->siguiente != nullptr)
        {
            if(puertas->siguiente->puerta->getDesconectado())
            {
                borrar(puertas->siguiente->puerta);
                limpiados++;
            }
            else
                puertas = puertas->siguiente;
        }

        if(listaPuertas->puerta->getDesconectado())
        {
            borrar(listaPuertas->puerta);
            limpiados++;
        }
    }

    const ListaEntradas* entradas{listaEntradas};
    if(entradas != nullptr)
    {
        while(entradas->siguiente != nullptr)
        {
            if(entradas->siguiente->entrada->getDesconectado())
            {
                borrar(entradas->siguiente->entrada);
                limpiados++;
            }
            else
                entradas = entradas->siguiente;
        }

        if(listaEntradas->entrada->getDesconectado())
        {
            borrar(listaEntradas->entrada);
            limpiados++;
        }
    }

    const ListaSalidas* salidas{listaSalidas};

    if(salidas != nullptr)
    {

        while(salidas->siguiente != nullptr)
        {
            if(salidas->siguiente->salida->getDesconectado())
            {
                borrar(salidas->siguiente->salida);
                limpiados++;
            }
            else
                salidas = salidas->siguiente;
        }

        if(listaSalidas->salida->getDesconectado())
        {
            borrar(listaSalidas->salida);
            limpiados++;
        }
    }

    return limpiados;
}