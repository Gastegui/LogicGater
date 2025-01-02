//
// Created by julen on 23/08/24.
//

#include "raton.h"
#include "Wrapper/image.h"
#include "controlador.h"
#include "controles.h"

IMG* buscarLista(const ListaIMG::Lista* lista, const int posX, const int posY)
{
    // ReSharper disable once CppJoinDeclarationAndAssignment
    SDL_Rect* rect;

    while(lista != nullptr)
    {
        if(lista->img->getClickable())
        {
            rect = lista->img->getRect();
            if(rect->x <= posX && rect->x + rect->w >= posX && rect->y <= posY && rect->y + rect->h >= posY)
                return lista->img;
        }

        lista = lista->siguiente;
    }

    return nullptr;
}

void Raton::interactuar(IMG* actual)
{
    const Controles::Accion accion = Controles::getUltimaAccion();
    if(borrando)
    {
        if(accion == Controles::Interactuar)
        {
            if(actual->getPadrePuerta())
                controlador->borrar(actual->getPadrePuerta());
            else if(actual->getPadreEntrada())
                controlador->borrar(actual->getPadreEntrada());
            else if(actual->getPadreSalida())
                controlador->borrar(actual->getPadreSalida());
            borrando = false;
            return;
        }
    }

    if(imgAnterior == nullptr) //El raton a entrado a una imagen desde sin estar antes en otra
    {
        imgAnterior = actual;
        actual->clickar(posX - actual->getRect()->x, posY - actual->getRect()->y, Evento::ENTRAR);
    }
    else
    {
        if(imgAnterior == actual) //El raton sige encima de la misma imagen
        {
            if(accion == Controles::MovimientoRaton)
            {
                const SDL_Event* evento = Controles::getEvent();
                actual->clickar(evento->motion.xrel, evento->motion.yrel, Raton::MOVIMIENTO);
            }
            else
                actual->clickar(posX - actual->getRect()->x, posY - actual->getRect()->y, Raton::NADA);
        }
        else //Se salta de una imagen a otra directamente
        {
            imgAnterior->clickar(-1, -1,  Evento::SALIR);
            imgAnterior = actual;
            actual->clickar(posX - actual->getRect()->x, posY - actual->getRect()->y, Evento::ENTRAR);
        }
    }
}

bool Raton::interactuarConexion(IMG* actual)
{

    if(borrando)
    {
        if(Controles::getUltimaAccion() == Controles::ConexionArriba)
        {
            if(actual->getPadrePuerta())
            {
                const bool borrarArriba{posY < actual->getRect()->y + actual->getRect()->h / 2 && posX <= actual->getRect()->x + actual->getRect()->w / 2};
                const bool borrarAbajo{posY >= actual->getRect()->y + actual->getRect()->h / 2 && posX <= actual->getRect()->x + actual->getRect()->w / 2};
                const bool borrarSalida{posX > actual->getRect()->x + actual->getRect()->w / 2};
                controlador->borrarConexiones(actual->getPadrePuerta(), borrarArriba, borrarAbajo, borrarSalida);
            }
            else if(actual->getPadreEntrada())
                controlador->borrarConexiones(actual->getPadreEntrada());
            else if(actual->getPadreSalida())
                controlador->borrarConexiones(actual->getPadreSalida());
            borrando = false;
            return true;
        }

    }

    if(controlador->getConectando())
    {
        if(Controles::getUltimaAccion() == Controles::ConexionArriba)
        {
            if(actual->getPadrePuerta() != nullptr)
                controlador->destino(actual->getPadrePuerta(), posY < actual->getRect()->y + actual->getRect()->h / 2);
            else if(actual->getPadreSalida() != nullptr)
                controlador->destino(actual->getPadreSalida());
            else
                controlador->desmarcarOrigen();
            return true;
        }
    }
    else if(Controles::getUltimaAccion() == Controles::ConexionAbajo)
    {
        if(actual->getPadrePuerta() != nullptr)
            controlador->marcarOrigen(actual->getPadrePuerta());
        else if(actual->getPadreEntrada() != nullptr)
            controlador->marcarOrigen(actual->getPadreEntrada());
        else
            return false;

        return true;
    }

    return false;
}


void Raton::manejarRaton()
{
    SDL_GetMouseState(&posX, &posY);

    const ListaIMG::Lista* lista{controlador->getListaIMG(ListaIMG::FRENTE)};
    IMG* actual = buscarLista(lista, posX, posY);
    if(actual != nullptr)
    {
        interactuar(actual);
        return;
    }

    lista = controlador->getListaIMG(ListaIMG::MEDIO);
    actual = buscarLista(lista, posX, posY);
    if(actual != nullptr)
    {
        if(interactuarConexion(actual))
            return;
        interactuar(actual);
        return;
    }

    //Por si se ha soltado la rueda en algun lugar que no sea clickable y conectable
    if(controlador->getConectando() && Controles::getUltimaAccion() == Controles::ConexionArriba)
    {
        controlador->desmarcarOrigen();
    }

    lista = controlador->getListaIMG(ListaIMG::FONDO);
    actual = buscarLista(lista, posX, posY);
    if(actual != nullptr)
    {
        interactuar(actual);
        return;
    }

    if(imgAnterior != nullptr) //El raton no esta en ninguna imagen, y antes si lo estaba
    {
        imgAnterior->clickar(-1, -1, Evento::SALIR);
        imgAnterior = nullptr;
    }
}

void Raton::setBorrando(const bool borrando_)
{
    borrando = borrando_;
    controlador->desmarcarOrigen();
}
