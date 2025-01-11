//
// Created by julen on 23/08/24.
//

#include "raton.h"
#include "Wrapper/image.h"
#include "controlador.h"
#include "controles.h"
#include "Wrapper/window.h"

IMG* Raton::buscarLista(const ListaIMG::Lista* lista, const int posX, const int posY) const
{
    // ReSharper disable once CppJoinDeclarationAndAssignment
    SDL_Rect* rect;

    while(lista != nullptr)
    {
        if(lista->img->getClickable())
        {
            rect = lista->img->getRect();
            if(rect->x <= -window->getEsquinaX() + posX && rect->x + rect->w >= -window->getEsquinaX() + posX && rect->y <= -window->getEsquinaY() + posY && rect->y + rect->h >= -window->getEsquinaY() + posY)
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
        actual->clickar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, ENTRAR);
    }
    else
    {
        if(imgAnterior == actual) //El raton sige encima de la misma imagen
        {
            if(accion == Controles::MovimientoRaton)
            {
                const SDL_Event* evento = Controles::getEvent();
                actual->clickar(evento->motion.xrel, evento->motion.yrel, MOVIMIENTO);
            }
            else
                actual->clickar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, NADA);
        }
        else //Se salta de una imagen a otra directamente
        {
            imgAnterior->clickar(-1, -1,  SALIR);
            imgAnterior = actual;
            actual->clickar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, ENTRAR);
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
                const bool borrarArriba{-window->getEsquinaY() + posY < actual->getRect()->y + actual->getRect()->h / 2 && -window->getEsquinaX() + posX <= actual->getRect()->x + actual->getRect()->w / 2};
                const bool borrarAbajo{-window->getEsquinaY() + posY >= actual->getRect()->y + actual->getRect()->h / 2 && -window->getEsquinaX() + posX <= actual->getRect()->x + actual->getRect()->w / 2};
                const bool borrarSalida{-window->getEsquinaX() + posX > actual->getRect()->x + actual->getRect()->w / 2};
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
                controlador->destino(actual->getPadrePuerta(), -window->getEsquinaY() + posY < actual->getRect()->y + actual->getRect()->h / 2);
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

void Raton::setControlador(Controlador* controlador_)
{
    controlador = controlador_;
    window = controlador->getWindow();
}


void Raton::manejarRaton()
{
    SDL_GetMouseState(&posX, &posY);

    if(moviendoPantalla)
    {
        const Controles::Accion accion = Controles::getUltimaAccion();
        if(accion == Controles::MoverArriba)
            moviendoPantalla = false;
        else if(accion == Controles::MovimientoRaton)
        {
            const SDL_Event* evento = Controles::getEvent();
            window->moverRel(evento->motion.xrel, evento->motion.yrel);
            return;
        }
    }

    if(moviendoImg != nullptr)
    {
        const Controles::Accion accion = Controles::getUltimaAccion();
        if(accion == Controles::MoverArriba)
        {
            moviendoImg = nullptr;
            cuadriculaX = 0;
            cuadriculaY = 0;
        }
        else if(accion == Controles::MovimientoRaton)
        {
            const SDL_Event* evento = Controles::getEvent();
            if(!controlador->getCuadriculaActiva())
                moviendoImg->clickar(evento->motion.xrel, evento->motion.yrel, MOVIMIENTO);
            else
            {
                for(int i = 0; i < abs(evento->motion.xrel); i++)
                {
                    if(evento->motion.xrel > 0)
                        cuadriculaX += 1;
                    else
                        cuadriculaX -= 1;

                    if((cuadriculaX + moviendoImg->getRect()->x) % controlador->getCuadriculaTamaño() == 0)
                    {
                        moviendoImg->clickar(cuadriculaX, 0, MOVIMIENTO);
                        cuadriculaX = 0;
                    }
                }
                for(int i = 0; i < abs(evento->motion.yrel); i++)
                {
                    if(evento->motion.yrel > 0)
                        cuadriculaY += 1;
                    else
                        cuadriculaY -= 1;

                    if((cuadriculaY + moviendoImg->getRect()->y) % controlador->getCuadriculaTamaño() == 0)
                    {
                        moviendoImg->clickar(0, cuadriculaY, MOVIMIENTO);
                        cuadriculaY = 0;
                    }
                }
            }
            return;
        }
    }

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

    //El ratón no está en ninguna imágen
    if(imgAnterior != nullptr) //... y antes si lo estaba
    {
        imgAnterior->clickar(-1, -1, SALIR);
        imgAnterior = nullptr;
    }

    if(Controles::getUltimaAccion() == Controles::MoverAbajo)
        moviendoPantalla = true;
}

void Raton::setBorrando(const bool borrando_)
{
    borrando = borrando_;
    controlador->desmarcarOrigen();
}
