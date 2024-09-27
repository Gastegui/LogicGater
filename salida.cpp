//
// Created by julen on 25/08/24.
//

#include "salida.h"
#include "puerta.h"
#include "controles.h"

void Salida::click(const int x, const int y, const Raton::Evento evento)
{
    if(evento == Raton::SALIR)
        mover = false;

    switch (Controles::getUltimaAccion())
    {
    case Controles::MoverAbajo:
        mover = true;
        break;
    case Controles::MoverArriba:
        mover = false;
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

void Salida::simular()
{
    if(entrada == nullptr )
    {
        img.cambiarImagen("./img/salida/apagado.png");
        return;
    }

    if(entrada->getPadrePuerta() != nullptr)
        entrada->getPadrePuerta()->simular();

    //Si la salida está directamente conectada a una entrada, al hacer entrada->get() ya se pilla su valor sin simular

    if(entrada->get())
        img.cambiarImagen("./img/salida/encendido.png");
    else
        img.cambiarImagen("./img/salida/apagado.png");
}
