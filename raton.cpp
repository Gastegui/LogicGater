//
// Created by julen on 23/08/24.
//

#include "raton.h"

#include <algorithm>
#include "Wrapper/image.h"
#include "controles.h"
#include "controlador.h"
#include "Wrapper/window.h"
#include "Simulables/entrada.h"

auto Raton::buscarLista(const std::vector<IMG*>* lista, const int posX, const int posY, const bool absoluto) const -> IMG*
{
    for(IMG* img : *lista)
    {
        if(img->getClickable())
        {
            const SDL_Rect* rect = img->getRect();
            if(absoluto)
            {
                if(rect->x <= posX && rect->x + rect->w >= posX && rect->y <= posY && rect->y + rect->h >= posY)
                    return img;
            }
            else
            {
                if(rect->x <= -Window::get().getEsquinaX() + posX && rect->x + rect->w >= -Window::get().getEsquinaX() + posX && rect->y <= -Window::get().getEsquinaY() + posY && rect->y + rect->h >= -Window::get().getEsquinaY() + posY)
                    return img;
            }
        }
    }

    return nullptr;
}

auto Raton::interactuar(IMG* actual) -> void
{
    const ACCION accion = Controles::getUltimaAccion();
    if(borrando)
    {
        if(accion == ACCION::InteractuarArriba)
        {
            Controlador::get().borrar(actual->getSimulable());
            borrando = false;
            return;
        }
    }

    if(imgAnterior == nullptr) //El raton a entrado a una imagen desde sin estar antes en otra
    {
        imgAnterior = actual;
        actual->interactuar(-Window::get().getEsquinaX() + posX - actual->getRect()->x, -Window::get().getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::RatonEntrar);
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
                actual->interactuar(-Window::get().getEsquinaX() + posX - actual->getRect()->x, -Window::get().getEsquinaY() + posY - actual->getRect()->y, static_cast<INTERACCIONES>(accion));
        }
        else //Se salta de una imagen a otra directamente
        {
            imgAnterior->interactuar(-1, -1, INTERACCIONES::RatonSalir);
            imgAnterior = actual;
            actual->interactuar(-Window::get().getEsquinaX() + posX - actual->getRect()->x, -Window::get().getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::RatonEntrar);
        }
    }
}

auto Raton::interactuarConexion(IMG* actual) -> bool
{
    if(borrando)
    {
        if(Controles::getUltimaAccion() == ACCION::ConexionArriba)
        {
            if(actual->getSimulable() != nullptr)
                actual->getSimulable()->interactuar(-Window::get().getEsquinaX() + posX - actual->getRect()->x, -Window::get().getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::ConexionBorrar);

            borrando = false;
            return true;
        }
    }

    if(Controlador::get().getConectando())
    {
        if(Controles::getUltimaAccion() == ACCION::ConexionArriba && actual->getSimulable() != nullptr)
        {
            if(!actual->getSimulable()->interactuar(-Window::get().getEsquinaX() + posX - actual->getRect()->x, -Window::get().getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::ConexionArriba))
                Controlador::get().desmarcarOrigen(); //Si no se puede conectar, desmarcar origen

            return true;
        }
    }
    else if(Controles::getUltimaAccion() == ACCION::ConexionAbajo && actual->getSimulable() != nullptr)
    {
        return actual->getSimulable()->interactuar(-Window::get().getEsquinaX() + posX - actual->getRect()->x, -Window::get().getEsquinaY() + posY - actual->getRect()->y, INTERACCIONES::ConexionAbajo);
    }

    return false;
}

auto Raton::manejarRaton() -> void
{
    SDL_GetMouseState(&posX, &posY);
    const std::vector<IMG*>* lista{};
    IMG* actual{};

    if(posX < 10)
    {
        Window::get().moverRel(1, 0);
        if(!seleccionados.empty())
        {
            for (Simulable* seleccionado : seleccionados)
                seleccionado->interactuar(-1, 0, INTERACCIONES::MovimientoRaton);
        }
        if(moviendoImg != nullptr)
            moviendoImg->interactuar(-1, 0, INTERACCIONES::MovimientoRaton);
    }
    else if(posX > 1910)
    {
        Window::get().moverRel(-1, 0);
        if(!seleccionados.empty())
        {
            for (Simulable* seleccionado : seleccionados)
                seleccionado->interactuar(1, 0, INTERACCIONES::MovimientoRaton);
        }
        if(moviendoImg != nullptr)
            moviendoImg->interactuar(1, 0, INTERACCIONES::MovimientoRaton);
    }
    if(posY < 10)
    {
        Window::get().moverRel(0, 1);
        if(!seleccionados.empty())
        {
            for (Simulable* seleccionado : seleccionados)
                seleccionado->interactuar(0, -1, INTERACCIONES::MovimientoRaton);
        }
        if(moviendoImg != nullptr)
            moviendoImg->interactuar(0, -1, INTERACCIONES::MovimientoRaton);
    }
    else if(posY > 1070)
    {
        Window::get().moverRel(0, -1);
        if(!seleccionados.empty())
        {
            for (Simulable* seleccionado : seleccionados)
                seleccionado->interactuar(0, 1, INTERACCIONES::MovimientoRaton);
        }
        if(moviendoImg != nullptr)
            moviendoImg->interactuar(0, 1, INTERACCIONES::MovimientoRaton);
    }

    if(seleccionando)
    {
        switch(Controles::getUltimaAccion())
        {
            case ACCION::MovimientoRaton:
                {
                    const int posXmapa = -Window::get().getEsquinaX() + posX;
                    const int posYmapa = -Window::get().getEsquinaY() + posY;

                    if(posXmapa > seleccionXOriginal)
                    {
                        seleccion.x = seleccionXOriginal;
                        seleccion.w = posXmapa - seleccionXOriginal;
                    }
                    else
                    {
                        seleccion.x = posXmapa;
                        seleccion.w = seleccionXOriginal - seleccion.x;
                    }

                    if(posYmapa > seleccionYOriginal)
                    {
                        seleccion.y = seleccionYOriginal;
                        seleccion.h = posYmapa - seleccionYOriginal;
                    }
                    else
                    {
                        seleccion.y = posYmapa;
                        seleccion.h = seleccionYOriginal - seleccion.y;
                    }
                    Window::get().setSeleccion(seleccion);
                    Controlador::get().seleccionar(seleccion);
                }
                break;
            case ACCION::InteractuarArriba:
                seleccionando = false;
                seleccion = {.x = 0, .y = 0, .w = 0, .h = 0};
                Window::get().setSeleccion(seleccion);
                break;
            default:
                break;
        }
        return;
    }


    if(moviendoPantalla)
    {
        const ACCION accion = Controles::getUltimaAccion();
        if(accion == ACCION::MoverArriba)
            moviendoPantalla = false;
        else if(accion == ACCION::MovimientoRaton)
        {
            const SDL_Event* evento = Controles::getEvent();
            Window::get().moverRel(Controles::getShift() ? evento->motion.xrel * 2 : evento->motion.xrel, Controles::getShift() ? evento->motion.yrel * 2 : evento->motion.yrel);
            return;
        }
    }

    if(!seleccionados.empty())
    {
        actual = buscarLista(Window::get().getListaIMG(ListaIMG::Medio), posX, posY);
        const bool imagen = actual != nullptr && std::ranges::find(seleccionados, actual->getSimulable()) != seleccionados.end();
        switch(Controles::getUltimaAccion())
        {
            case ACCION::MoverAbajo:
                if(!imagen)
                {
                    moviendoPantalla = true;
                    break;
                }
                for(Simulable* simulable : seleccionados)
                    simulable->interactuar(0, 0, INTERACCIONES::MoverAbajo);
                moviendoSeleccion = true;
                break;
            case ACCION::MovimientoRaton:
                {
                    if(!moviendoSeleccion)
                        break;
                    const SDL_Event* evento = Controles::getEvent();
                    for(Simulable* simulable : seleccionados)
                        simulable->interactuar(evento->motion.xrel, evento->motion.yrel, INTERACCIONES::MovimientoRaton);
                }
                break;
            case ACCION::MoverArriba:
                if(!moviendoSeleccion)
                    break;
                for(Simulable* simulable : seleccionados)
                    simulable->interactuar(0, 0, INTERACCIONES::MoverArriba);
                moviendoSeleccion = false;
                break;
            case ACCION::InteractuarArriba:
                Controlador::get().desseleccionar();
                moviendoSeleccion = false;
                break;
            case ACCION::InteractuarAbajo:
                Controlador::get().desseleccionar();
                moviendoSeleccion = false;
                seleccionando = true;
                seleccion.x = -Window::get().getEsquinaX() + posX;
                seleccion.y = -Window::get().getEsquinaY() + posY;
                seleccionXOriginal = seleccion.x;
                seleccionYOriginal = seleccion.y;
                break;
            default:
                break;
        }
        return;
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
            if(!Controlador::get().getCuadriculaActiva())
                moviendoImg->interactuar(evento->motion.xrel, evento->motion.yrel, INTERACCIONES::MovimientoRaton);
            else //Pero si está activa hay que calcular cuanto moverlo
            {
                for(int i = 0; i < abs(evento->motion.xrel); i++)
                {
                    if(evento->motion.xrel > 0)
                        cuadriculaX += 1;
                    else
                        cuadriculaX -= 1;

                    if((cuadriculaX + moviendoImg->getRect()->x) % Controlador::get().getCuadriculaTamaño() == 0)
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

                    if((cuadriculaY + moviendoImg->getRect()->y) % Controlador::get().getCuadriculaTamaño() == 0)
                    {
                        moviendoImg->interactuar(0, cuadriculaY, INTERACCIONES::MovimientoRaton);
                        cuadriculaY = 0;
                    }
                }
            }
            return;
        }
    }

    lista = Window::get().getListaIMG(ListaIMG::Frente);
    actual = buscarLista(lista, posX, posY, true);
    if(actual != nullptr)
    {
        if(Window::get().getImgMinimapa() == actual && Controles::getUltimaAccion() == ACCION::InteractuarArriba)
        {
            Window::get().mover(-interpolacionLinear(posX - actual->getRect()->x, 0, Window::get().getMinimapaTamañoX(), Window::get().getMapaMinX(), Window::get().getMapaMaxX()),
                          -interpolacionLinear(posY - actual->getRect()->y, 0, Window::get().getMinimapaTamañoY(), Window::get().getMapaMinY(), Window::get().getMapaMaxY()));
        }

        return;
    }

    lista = Window::get().getListaIMG(ListaIMG::Medio);
    actual = buscarLista(lista, posX, posY);
    if(actual != nullptr)
    {
        if(interactuarConexion(actual))
            return;
        interactuar(actual);
        return;
    }

    //Por si se ha soltado la rueda en algun lugar que no sea clickable y conectable
    if(Controlador::get().getConectando() && Controles::getUltimaAccion() == ACCION::ConexionArriba)
    {
        Controlador::get().desmarcarOrigen();
    }

    lista = Window::get().getListaIMG(ListaIMG::Fondo);
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

    switch(Controles::getUltimaAccion())
    {
        case ACCION::MoverAbajo:
            moviendoPantalla = true;
            break;
        case ACCION::InteractuarAbajo:
            seleccionando = true;
            seleccion.x = -Window::get().getEsquinaX() + posX;
            seleccion.y = -Window::get().getEsquinaY() + posY;
            seleccionXOriginal = seleccion.x;
            seleccionYOriginal = seleccion.y;
            break;
        default:
            break;
    }
}

auto Raton::setBorrando(const bool borrando_) -> void
{
    if(seleccionados.empty() && !seleccionando)
    {
        borrando = borrando_;
        Controlador::get().desmarcarOrigen();
    }
    else if(!moviendoSeleccion && !seleccionando)
    {
        for(Simulable* simulable : seleccionados)
            Controlador::get().borrar(simulable);
        seleccionados.clear();
    }
}
