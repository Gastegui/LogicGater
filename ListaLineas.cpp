//
// Created by julen on 9/04/25.
//

#include "ListaLineas.h"

#include "io.h"

void ListaLineas::añadir(Simulable* origen, Simulable* destino, IO* io_origen, IO* io_destino)
{
    if(origen == nullptr || destino == nullptr || io_origen == nullptr || io_destino == nullptr)
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
    tmp->simulable_origen = origen;
    tmp->simulable_destino = destino;
    tmp->io_origen = io_origen;
    tmp->io_destino = io_destino;
    tmp->siguiente = nullptr;
}

void ListaLineas::borrar(const Simulable* origen, const Simulable* destino, const IO* io_origen, const IO* io_destino)
{
    if(lista == nullptr || origen == nullptr || destino == nullptr || io_origen == nullptr || io_destino == nullptr)
        return;

    const Lista* tmp{lista};

    if(lista->simulable_origen == origen && lista->simulable_destino == destino && lista->io_origen == io_origen && lista->io_destino == io_destino)
    {
        lista = lista->siguiente;
        delete tmp;
        return;
    }

    Lista* anterior{lista};
    tmp = tmp->siguiente;

    while(tmp != nullptr)
    {
        if(tmp->simulable_origen == origen && tmp->simulable_destino == destino && tmp->io_origen == io_origen && tmp->io_destino == io_destino)
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

    while(lista != nullptr && (lista->io_origen == involucrado || lista->io_destino == involucrado))
    {
        tmp = lista;
        lista = lista->siguiente;
        tmp->io_origen->desconectado();
        tmp->simulable_destino->setIONull(tmp->io_destino);
        delete tmp;
    }

    if(lista == nullptr)
        return;

    Lista* anterior{lista};
    tmp = lista->siguiente;

    while(tmp != nullptr)
    {
        if(tmp->io_origen == involucrado || tmp->io_destino == involucrado)
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

    while(lista != nullptr && (lista->simulable_origen == involucrado || lista->simulable_destino == involucrado))
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
        if(tmp->simulable_origen == involucrado || tmp->simulable_destino == involucrado)
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
