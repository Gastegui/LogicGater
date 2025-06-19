//
// Created by julen on 3/11/24.
//

#include "ListaIMG.h"
#include "Wrapper/image.h"

auto ListaIMG::getLista(const Altura altura) const -> Lista*
{
    switch (altura)
    {
        case FRENTE:
            return frente;
        case MEDIO:
            return medio;
        case FONDO:
            return fondo;
        case INVALIDO:
        default:
            return nullptr;
    }
}

auto ListaIMG::añadir(IMG* img, const Altura altura) -> bool
{
    if(img == nullptr)
        return false;

    Lista** lista = nullptr;

    switch (altura)
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

    if(*lista == nullptr) //La lista está vacía
    {
        *lista = new Lista();
        (*lista)->img = img;
        (*lista)->siguiente = nullptr;
    }
    else //La lista no está vacía
    {
        Lista* tmp = *lista;
        while(tmp->siguiente != nullptr)
            tmp = tmp->siguiente;
        tmp->siguiente = new Lista();
        tmp->siguiente->img = img;
        tmp->siguiente->siguiente = nullptr;
    }

    return true;
}

auto ListaIMG::quitar(const unsigned int id, const Altura altura) -> bool
{
    Lista** lista{};

    switch (altura)
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

    if(!*lista)
        return false;

    if((*lista)->img->getId() == id) //Es el primer elemento de la lista
    {
        const Lista* tmp = *lista;
        *lista = (*lista)->siguiente;
        delete tmp;
    }
    else
    {
        Lista* tmp = *lista;

        while(tmp->siguiente != nullptr && tmp->siguiente->img->getId() != id)
            tmp = tmp->siguiente;

        if(tmp->siguiente == nullptr)
            return false;

        const Lista* borrar = tmp->siguiente;
        tmp->siguiente = borrar->siguiente;
        delete borrar;
    }

    return true;
}


auto ListaIMG::quitar(const IMG* img, const Altura altura) -> bool
{
    return quitar(img->getId(), altura);
}