//
// Created by julen on 23/08/24.
//

#include "raton.h"
#include "Wrapper/image.h"
#include "controlador.h"
#include "controles.h"
#include "Simulables/entrada.h"
#include "Wrapper/window.h"

IMG* Raton::buscarLista(const ListaIMG::Lista* lista, const int posX, const int posY) const
{

    while(lista != nullptr)
    {
        if(lista->img->getClickable())
        {
            const SDL_Rect* rect = lista->img->getRect();
            if(rect->x <= -window->getEsquinaX() + posX && rect->x + rect->w >= -window->getEsquinaX() + posX && rect->y <= -window->getEsquinaY() + posY && rect->y + rect->h >= -window->getEsquinaY() + posY)
                return lista->img;
        }

        lista = lista->siguiente;
    }

    return nullptr;
}

void Raton::interactuar(IMG* actual)
{
    const ACCION accion = Controles::getUltimaAccion();
    if(borrando)
    {
        if(accion == ACCION::InteractuarArriba)
        {
            controlador->borrar(actual->getSimulable());
            borrando = false;
            return;
        }
    }

    if(imgAnterior == nullptr) //El raton a entrado a una imagen desde sin estar antes en otra
    {
        imgAnterior = actual;
        actual->interactuar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::RatonEntrar);
    }
    else
    {
        if(imgAnterior == actual) //El raton sige encima de la misma imagen
        {
            if(accion == ACCION::MovimientoRaton)
            {
                const SDL_Event* evento = Controles::getEvent();
                actual->interactuar(evento->motion.xrel, evento->motion.yrel, INTERACCIONES::MovimientoRaton);
            }
            else
                actual->interactuar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, static_cast<INTERACCIONES>(accion));
        }
        else //Se salta de una imagen a otra directamente
        {
            imgAnterior->interactuar(-1, -1, INTERACCIONES::RatonSalir);
            imgAnterior = actual;
            actual->interactuar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::RatonEntrar);
        }
    }
}

bool Raton::interactuarConexion(IMG* actual) //TODO: NO SE PUEDEN SACAR DOS SALIDAS?
{
    if(borrando)
    {
        if(Controles::getUltimaAccion() == ACCION::ConexionArriba)
        {
            if(actual->getSimulable() != nullptr)
                actual->getSimulable()->interactuar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::ConexionBorrar);

            borrando = false;
            return true;
        }

    }

    if(controlador->getConectando())
    {
        if(Controles::getUltimaAccion() == ACCION::ConexionArriba)
        {
            if(!actual->getSimulable()->interactuar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::ConexionArriba))
                controlador->desmarcarOrigen(); //Si no se puede conectar, desmarcar origen

            return true;
        }
    }
    else if(Controles::getUltimaAccion() == ACCION::ConexionAbajo)
    {
        return actual->getSimulable()->interactuar(-window->getEsquinaX() + posX - actual->getRect()->x, -window->getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::ConexionAbajo);
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
        const ACCION accion = Controles::getUltimaAccion();
        if(accion == ACCION::MoverArriba)
            moviendoPantalla = false;
        else if(accion == ACCION::MovimientoRaton)
        {
            const SDL_Event* evento = Controles::getEvent();
            window->moverRel(evento->motion.xrel, evento->motion.yrel);
            return;
        }
    }

    if(moviendoImg != nullptr)
    {
        const ACCION accion = Controles::getUltimaAccion();
        if(accion == ACCION::MoverArriba)
        {
            moviendoImg = nullptr;
            cuadriculaX = 0;
            cuadriculaY = 0;
        }
        else if(accion == ACCION::MovimientoRaton)
        {
            const SDL_Event* evento = Controles::getEvent();
            //Si la cuadrícula no está activa, se le puede pasar directo el movimiento al elemento
            if(!controlador->getCuadriculaActiva())
                moviendoImg->interactuar(evento->motion.xrel, evento->motion.yrel, INTERACCIONES::MovimientoRaton);
            else //Pero si está activa hay que calcular cuanto moverlo
            {
                for(int i = 0; i < abs(evento->motion.xrel); i++)
                {
                    if(evento->motion.xrel > 0)
                        cuadriculaX += 1;
                    else
                        cuadriculaX -= 1;

                    if((cuadriculaX + moviendoImg->getRect()->x) % controlador->getCuadriculaTamaño() == 0)
                    {
                        moviendoImg->interactuar(cuadriculaX, 0, INTERACCIONES::MovimientoRaton);
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
                        moviendoImg->interactuar(0, cuadriculaY, INTERACCIONES::MovimientoRaton);
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
    if(controlador->getConectando() && Controles::getUltimaAccion() == ACCION::ConexionArriba)
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
        imgAnterior->interactuar(-1, -1, INTERACCIONES::RatonSalir);
        imgAnterior = nullptr;
    }

    if(Controles::getUltimaAccion() == ACCION::MoverAbajo)
        moviendoPantalla = true;
}

void Raton::setBorrando(const bool borrando_)
{
    borrando = borrando_;
    controlador->desmarcarOrigen();
}
