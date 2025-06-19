//
// Created by julen on 9/04/25.
//

#include "ListaLineas.h"

#include "io.h"

void ListaLineas::añadir(Simulable* destino, IO* io, const int conexion)
{
    if(destino == nullptr || io == nullptr)
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
    tmp->destino = destino;
    tmp->io = io;
    tmp->conexion = conexion;
    tmp->siguiente = nullptr;
}

void ListaLineas::borrar(const Simulable* destino , const IO* io, const int conexion)
{
    if(lista == nullptr || destino == nullptr || io == nullptr)
        return;

    const Lista* tmp{lista};

    if(lista->destino == destino && lista->io == io && lista->conexion == conexion)
    {
        lista = lista->siguiente;
        tmp->io->getSimulable()->setIONull(tmp->io);
        tmp->destino->setIONull(tmp->io);
        tmp->io->desconectado();
        delete tmp;
        return;
    }

    Lista* anterior{lista};
    tmp = tmp->siguiente;

    while(tmp != nullptr)
    {
        if(tmp->destino == destino && tmp->io == io && tmp->conexion == conexion)
        {
            anterior->siguiente = tmp->siguiente;
            tmp->io->getSimulable()->setIONull(tmp->io);
            tmp->destino->setIONull(tmp->io);
            tmp->io->desconectado();
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
            tmp->io->desconectado();
            tmp->destino->setIONull(tmp->io);
            delete tmp;
            tmp = anterior->siguiente;
        }
        else
        {
            anterior = anterior->siguiente;
            tmp = tmp->siguiente;
        }
    }
}

void ListaLineas::borrar(const Simulable* involucrado)
{
    if(lista == nullptr || involucrado == nullptr)
        return;

    const Lista* tmp{nullptr};

    while(lista != nullptr && (lista->io->getSimulable() == involucrado || lista->destino == involucrado))
    {
        tmp = lista;
        lista = lista->siguiente;
        if(tmp->io->getSimulable() == involucrado)
            tmp->destino->setIONull(tmp->io);
        else
            tmp->io->getSimulable()->setIONull(tmp->io);
        tmp->io->desconectado();
        delete tmp;
    }

    if(lista == nullptr)
        return;

    Lista* anterior{lista};
    tmp = lista->siguiente;

    while(tmp != nullptr)
    {
        if(tmp->io->getSimulable() == involucrado || tmp->destino == involucrado)
        {
            anterior->siguiente = tmp->siguiente;
            if(tmp->io->getSimulable() == involucrado)
                tmp->destino->setIONull(tmp->io);
            else
                tmp->io->getSimulable()->setIONull(tmp->io);
            tmp->io->desconectado();
            delete tmp;
            tmp = anterior->siguiente;
        }
        else
        {
            anterior = anterior->siguiente;
            tmp = tmp->siguiente;
        }
    }
}
