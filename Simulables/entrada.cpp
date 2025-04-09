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

bool Entrada::interactuar(const int posX, const int posY, const INTERACCIONES interaccion)
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

        case MoverAbajo: //TODO: QUE SE USEN TODAS LAS SIGUIENTES (antigua funcion click())
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
            return true;
        case InteractuarArriba:
            if(mantener)
                set(false);
            else
                set(!get());
            return true;
        case RatonSalir:
            mover = false;
            if(mantener)
                set(false);
            return true;
        default:
            return false;
    }
}
