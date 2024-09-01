//
// Created by julen on 25/08/24.
//

#include "entrada.h"

void Entrada::click(const int x, const int y, const Raton::Boton boton, const Raton::Evento evento)
{
    switch (evento)
    {
    case Raton::ABAJO:
        if(boton == Raton::IZQUIERDO)
            mover = true;
        else if(boton == Raton::DERECHO)
        {
            if(mantener)
                set(true);
        }
        break;
    case Raton::ARRIBA:
        if(boton == Raton::IZQUIERDO)
            mover = false;
        else if(boton == Raton::DERECHO)
        {
            if(mantener)
                set(false);
            else
                set(!get());
        }
        break;
    case Raton::SALIR:
        mover = false;
        if(mantener)
            set(false);
        break;
    case Raton::MOVIMIENTO:
        if(mover)
        {
            img.mover(img.getRect()->x + x, img.getRect()->y + y);
            linea.first += x;
            linea.second += y;
        }
        break;
    default:
        break;
    }

}
