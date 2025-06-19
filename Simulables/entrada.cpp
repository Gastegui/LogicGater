//
// Created by julen on 25/08/24.
//

#include "entrada.h"

#include "../controlador.h"


void Entrada::moverRel(const int x, const int y)
{
    img.mover(img.getRect()->x + x, img.getRect()->y + y);
    salida.moverLineaOrigenRel(x, y);
}

auto Entrada::interactuar(const int posX, const int posY, const INTERACCIONES interaccion) -> bool
{
    using enum INTERACCIONES;
    switch (interaccion)
    {
        case ConexionAbajo:
            controlador->marcarOrigen(getSalida());
            return true;
        case ConexionBorrar:
            controlador->borrarConexiones(&salida);
            return true;

        case MoverAbajo:
            mover = true;
            raton->setMoviendoImg(&img);
            return true;
        case MoverArriba:
            mover = false;
            raton->setMoviendoImg(nullptr);
            return true;
        case MovimientoRaton:
            if(mover)
                moverRel(posX, posY);
        return true;

        case InteractuarAbajo:
            if(mantener)
                set(true);
            interactuarAbajo = true;
            return true;
        case InteractuarArriba:
            if(!interactuarAbajo)
                return true;
            if(mantener)
                set(false);
            else
                set(!get());
            return true;
        case RatonSalir:
            mover = false;
            interactuarAbajo = false;
            if(mantener)
                set(false);
            return true;
        default:
            return false;
    }
}
