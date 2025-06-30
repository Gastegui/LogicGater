//
// Created by julen on 3/11/24.
//

#include "ListaIMG.h"
#include "Wrapper/image.h"

auto ListaIMG::getLista(const ALTURA altura) const -> const std::vector<IMG*>*
{
    switch(altura)
    {
        case Frente:
            return &frente;
        case Medio:
            return &medio;
        case Fondo:
            return &fondo;
        case Invalido:
        default:
            return nullptr;
    }
}

auto ListaIMG::añadir(IMG* img, const ALTURA altura) -> bool
{
    if(img == nullptr)
        return false;

    std::vector<IMG*>* lista = nullptr;

    switch(altura)
    {
        case Frente:
            lista = &frente;
            break;
        case Medio:
            lista = &medio;
            break;
        case Fondo:
            lista = &fondo;
            break;
        case Invalido:
        default:
            return false;
    }

    lista->push_back(img);

    return true;
}

auto ListaIMG::quitar(const unsigned int id, const ALTURA altura) -> bool
{
    std::vector<IMG*>* lista{};

    switch(altura)
    {
        case Frente:
            lista = &frente;
            break;
        case Medio:
            lista = &medio;
            break;
        case Fondo:
            lista = &fondo;
            break;
        case Invalido:
        default:
            return false;
    }

    auto it = lista->begin();
    for(; it != lista->end(); ++it)
        if((*it)->getId() == id)
            break;

    if(it != lista->end())
        lista->erase(it);

    return true;
}


auto ListaIMG::quitar(const IMG* img, const ALTURA altura) -> bool
{
    return quitar(img->getId(), altura);
}
