//
// Created by julen on 28/01/25.
//

#include "Temporizador.h"

#include "../controles.h"

void Temporizador::simular()
{
    if(entrada->get() && duracion != actual)
        actual++;
    else if(!entrada->get() && actual != 0)
        actual = 0;
}

void Temporizador::actualizar()
{
    salida.set(actual == duracion);
}

bool Temporizador::simularAntiguo()
{
    salida.set(entrada->get());

    return salida.get();
}



void Temporizador::click(const int x, const int y, const Raton::Evento evento)
{
    switch (evento)
    {
        case Raton::ENTRAR:
            break;
        case Raton::SALIR:
            if(mover == true)
                mover = false;
            break;
        default:
            break;
    }

    switch (Controles::getUltimaAccion())
    {
        case Controles::TemporizadorBajar:
            duracion--;
            break;
        case Controles::TemporizadorSubir:
            duracion++;
            break;
        case Controles::MoverAbajo:
            mover = true;
            raton->setMoviendoImg(&imagen);
            break;
        case Controles::MoverArriba:
            mover = false;
            raton->setMoviendoImg(nullptr);
            break;
        case Controles::MovimientoRaton:
            if(mover)
                moverRel(x, y);
            break;
        default:
            break;
    }
}

void Temporizador::moverRel(const int x_, const int y_)
{
    imagen.mover(imagen.getRect()->x + x_, imagen.getRect()->y + y_);
    lineaEntrada.first += x_;
    lineaEntrada.second += y_;
    lineaSalida.first += x_;
    lineaSalida.second += y_;
    x += x_;
    y += y_;
}