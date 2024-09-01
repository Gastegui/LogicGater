//
// Created by julen on 25/08/24.
//

#include "salida.h"

#include "puerta.h"

void Salida::click(const int x, const int y, const Raton::Boton boton, const Raton::Evento evento)
{
    switch (evento)
    {
    case Raton::ABAJO:
        if(boton == Raton::IZQUIERDO)
            mover = true;
        break;
    case Raton::SALIR:
        mover = false;
        break;
    case Raton::ARRIBA:
        if(boton == Raton::IZQUIERDO)
            mover = false;
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
