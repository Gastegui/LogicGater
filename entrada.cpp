//
// Created by julen on 25/08/24.
//

#include "entrada.h"
#include "controles.h"

void Entrada::click(const int x, const int y, const Raton::Evento evento)
{
    if(evento == Raton::SALIR)
    {
        mover = false;
        if(mantener)
            set(false);

    }
    switch (Controles::getUltimaAccion())
    {
    case Controles::MoverAbajo:
        mover = true;
        break;
    case Controles::InteractuarAbajo:
        if(mantener)
            set(true);
        break;
    case Controles::MoverArriba:
        mover = false;
        break;
    case Controles::Interactuar:
        if(mantener)
            set(false);
        else
            set(!get());
        break;
    case Controles::MovimientoRaton:
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
