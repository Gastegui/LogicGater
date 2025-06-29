//
// Created by julen on 3/11/24.
//

#include "ListaIMG.h"
#include "Wrapper/image.h"

auto ListaIMG::getLista(const Altura altura) const -> const std::vector<IMG*>*
{
    switch(altura)
    {
        case FRENTE:
            return &frente;
        case MEDIO:
            return &medio;
        case FONDO:
            return &fondo;
        case INVALIDO:
        default:
            return nullptr;
    }
}

auto ListaIMG::añadir(IMG* img, const Altura altura) -> bool
{
    if(img == nullptr)
        return false;

    std::vector<IMG*>* lista = nullptr;

    switch(altura)
    {
        case FRENTE:
            lista = &frente;
            break;
        case MEDIO:
            lista = &medio;
            break;
        case FONDO:
            lista = &fondo;
            break;
        case INVALIDO:
        default:
            return false;
    }

    lista->push_back(img);

    return true;
}

auto ListaIMG::quitar(const unsigned int id, const Altura altura) -> bool
{
    std::vector<IMG*>* lista{};

    switch(altura)
    {
        case FRENTE:
            lista = &frente;
            break;
        case MEDIO:
            lista = &medio;
            break;
        case FONDO:
            lista = &fondo;
            break;
        case INVALIDO:
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


auto ListaIMG::quitar(const IMG* img, const Altura altura) -> bool
{
    return quitar(img->getId(), altura);
}
