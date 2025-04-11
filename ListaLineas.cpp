//
// Created by julen on 9/04/25.
//

#include "ListaLineas.h"

#include "io.h"

void ListaLineas::añadir(Simulable* origen, Simulable* destino, IO* io)
{
    if(origen == nullptr || destino == nullptr || io == nullptr)
        return;

    Lista* tmp{lista};
    if(lista == nullptr)
    {
        lista = new Lista();
        tmp = lista;
    }
    else
    {
        while(tmp->siguiente != nullptr)
            tmp = tmp->siguiente;

        tmp->siguiente = new Lista();
        tmp = tmp->siguiente;
    }
    tmp->origen = origen;
    tmp->destino = destino;
    tmp->io = io;
    tmp->siguiente = nullptr;
}

void ListaLineas::borrar(const Simulable* origen, const Simulable* destino, const IO* io)
{
    if(lista == nullptr || origen == nullptr || destino == nullptr || io == nullptr)
        return;

    const Lista* tmp{lista};

    if(lista->origen == origen && lista->destino == destino && lista->io == io)
    {
        lista = lista->siguiente;
        delete tmp;
        return;
    }

    Lista* anterior{lista};
    tmp = tmp->siguiente;

    while(tmp != nullptr)
    {
        if(tmp->origen == origen && tmp->destino == destino && tmp->io == io)
        {
            anterior->siguiente = tmp->siguiente;
            delete tmp;
            return;
        }
        tmp = tmp->siguiente;
        anterior = anterior->siguiente;
    }
}

void ListaLineas::borrar(const IO* involucrado)
{
    if(lista == nullptr || involucrado == nullptr)
        return;

    const Lista* tmp{nullptr};

    while(lista != nullptr && lista->io == involucrado)
    {
        tmp = lista;
        lista = lista->siguiente;
        tmp->io->desconectado();
        tmp->destino->setIONull(tmp->io);
        delete tmp;
    }

    if(lista == nullptr)
        return;

    Lista* anterior{lista};
    tmp = lista->siguiente;

    while(tmp != nullptr)
    {
        if(tmp->io == involucrado)
        {
            anterior->siguiente = tmp->siguiente;
            delete tmp;
            tmp = anterior->siguiente;
        }
        else
        {
            anterior->siguiente = anterior->siguiente;
            tmp = tmp->siguiente;
        }
    }
}

void ListaLineas::borrar(const Simulable* involucrado)
{
    if(lista == nullptr || involucrado == nullptr)
        return;

    const Lista* tmp{nullptr};

    while(lista != nullptr && (lista->origen == involucrado || lista->destino == involucrado))
    {
        tmp = lista;
        lista = lista->siguiente;
        delete tmp;
    }

    if(lista == nullptr)
        return;

    Lista* anterior{lista};
    tmp = lista->siguiente;

    while(tmp != nullptr)
    {
        if(tmp->origen == involucrado || tmp->destino == involucrado)
        {
            anterior->siguiente = tmp->siguiente;
            delete tmp;
            tmp = anterior->siguiente;
        }
        else
        {
            anterior->siguiente = anterior->siguiente;
            tmp = tmp->siguiente;
        }
    }
}
