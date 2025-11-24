//
// Created by julen on 25/08/24.
//

#include "entrada.h"

#include "../controlador.h"


auto Entrada::moverRel(const int x, const int y) -> void
{
    img.moverRel(x, y);
    salida.moverLineaOrigenRel(x, y);
}

auto Entrada::interactuar(const int posX, const int posY, const INTERACCIONES interaccion) -> bool
{
    using enum INTERACCIONES;
    switch(interaccion)
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
        case Rotar:
            switch(img.getRotacion())
            {
                case 0:
                    img.setRotacion(90);
                    salida.moverLineaOrigenRel(-25, 25);
                    break;
                case 90:
                    img.setRotacion(180);
                    salida.moverLineaOrigenRel(-25, -25);
                    break;
                case 180:
                    img.setRotacion(270);
                    salida.moverLineaOrigenRel(25, -25);
                    break;
                case 270:
                    img.setRotacion(0);
                    salida.moverLineaOrigenRel(25, 25);
                    break;
                default:
                    return false;
            }
            return true;
        default:
            return false;
    }
}
