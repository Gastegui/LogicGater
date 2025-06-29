//
// Created by julen on 9/04/25.
//

#include "ListaLineas.h"

#include "io.h"

void ListaLineas::añadir(Simulable* destino, IO* io, const int conexion)
{
    if(destino == nullptr || io == nullptr)
        return;

    lista.push_back(Linea{.destino = destino, .io = io, .conexion = conexion});
}

void ListaLineas::borrar(const Simulable* destino, const IO* io, const int conexion)
{
    if(lista.empty() || destino == nullptr || io == nullptr)
        return;

    auto it = lista.begin();
    for(; it != lista.end(); ++it)
        if(it->destino == destino && it->io == io && it->conexion == conexion)
            break;

    if(it != lista.end())
    {
        it->io->desconectado();
        it->destino->setIONull(it->io);
        lista.erase(it);
    }
}

void ListaLineas::borrar(const IO* involucrado)
{
    if(lista.empty() || involucrado == nullptr)
        return;

    bool borrado{false};
    do // NOLINT(*-avoid-do-while)
    {
        borrado = false;
        auto it = lista.begin();
        for(; it != lista.end(); ++it)
            if((*it).io == involucrado)
                break;

        if(it != lista.end())
        {
            it->io->desconectado();
            it->destino->setIONull(it->io);
            lista.erase(it);
            borrado = true;
        }

    }while(borrado);
}

void ListaLineas::borrar(const Simulable* involucrado)
{
    if(lista.empty() || involucrado == nullptr)
        return;

    bool borrado{false};
    do // NOLINT(*-avoid-do-while)
    {
        borrado = false;
        auto it = lista.begin();
        for(; it != lista.end(); ++it)
            if(it->destino == involucrado || it->io->getSimulable() == involucrado)
                break;

        if(it != lista.end())
        {
            it->io->desconectado();
            it->destino->setIONull(it->io);
            lista.erase(it);
            borrado = true;
        }

    }while(borrado);
}
