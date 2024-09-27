//
// Created by julen on 22/08/24.
//

#include "puerta.h"
#include "raton.h"
#include "entrada.h"
#include "controles.h"


void Puerta::click(const int x, const int y, const Raton::Evento evento)
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

    switch(Controles::getUltimaAccion())
    {
        case Controles::Interactuar:
            if(x >= 0 && x <= 23 && y >= 0 && y <= 23)
            {
                arribaNegado = !arribaNegado;
                cambiar();
            }
            else if(x >= 0 && x <= 23 && y >= 26 && y <= 49)
            {
                abajoNegado = !abajoNegado;
                cambiar();
            }
            else if(x >= 76 && x <= 99 && y >= 13 && y <= 36)
            {
                salidaNegada = !salidaNegada;
                cambiar();
            }
            else
            {
                tipo = tipo == AND ? OR : tipo == OR ? XOR : AND;
                cambiar();
            }
            break;
        case Controles::MoverAbajo:
            mover = true;
            break;
        case Controles::MoverArriba:
            mover = false;
            break;
        case Controles::MovimientoRaton:
            if(mover)
            {
                imagen.mover(imagen.getRect()->x + x, imagen.getRect()->y + y);
                lineaArriba.first += x;
                lineaArriba.second += y;
                lineaAbajo.first += x;
                lineaAbajo.second += y;
                lineaSalida.first += x;
                lineaSalida.second += y;
            }
            break;
        default:
            break;
    }
}

bool Puerta::simular() // NOLINT(*-no-recursion)
{
    bool a{false};
    bool b{false};
    bool ret{false};

    if(simulando)
        return salida.get();

    simulando = true;

    if(arriba != nullptr)
    {
        if(arriba->getPadreEntrada() != nullptr)
            a = arriba->getPadreEntrada()->get();
        else if(arriba->getPadrePuerta() != nullptr)
            a = arriba->getPadrePuerta()->simular();
    }

    if(abajo != nullptr)
    {
        if(abajo->getPadreEntrada() != nullptr)
            b = abajo->getPadreEntrada()->get();
        else if(abajo->getPadrePuerta() != nullptr)
            b = abajo->getPadrePuerta()->simular();
    }

    if(arribaNegado)
        a = !a;

    if(abajoNegado)
        b = !b;

    if(tipo == AND)
        ret = a && b;
    else if(tipo == OR)
        ret = a || b;
    else if(tipo == XOR)
        ret = a != b;

    if(salidaNegada)
        ret = !ret;

    salida.set(ret);

    return ret;
}

void Puerta::simulacionTermindada()
{
    simulando = false;
}