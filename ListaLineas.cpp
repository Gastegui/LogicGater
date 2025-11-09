//
// Created by julen on 9/04/25.
//

#include "ListaLineas.h"

#include "io.h"

auto ListaLineas::añadir(Simulable* destino, IO* io, const int conexion, const bool oculta) -> void
{
    if(destino == nullptr || io == nullptr)
        return;

    lista.push_back(Linea{.destino = destino, .io = io, .conexion = conexion, .oculta = oculta});
}

auto ListaLineas::borrar(const Simulable* destino, const IO* io, const int conexion) -> void
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

auto ListaLineas::borrar(const IO* involucrado) -> void
{
    if(lista.empty() || involucrado == nullptr)
        return;

    bool borrado{false};
    do // NOLINT(*-avoid-do-while)
    {
        borrado = false;
        auto it = lista.begin();
        for(; it != lista.end(); ++it)
            if(it->io == involucrado)
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

auto ListaLineas::borrar(const Simulable* involucrado) -> void
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
