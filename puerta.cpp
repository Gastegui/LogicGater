//
// Created by julen on 22/08/24.
//

#include "puerta.h"
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

void Puerta::simular() // NOLINT(*-no-recursion)
{
    bool a{false};
    bool b{false};
    bool ret{false};

    if(arriba != nullptr)
    {
        if(arriba->getPadreEntrada() != nullptr)
            a = arriba->getPadreEntrada()->get();
        else if(arriba->getPadrePuerta() != nullptr)
            a = arriba->getPadrePuerta()->getSalida()->get();
    }

    if(abajo != nullptr)
    {
        if(abajo->getPadreEntrada() != nullptr)
            b = abajo->getPadreEntrada()->get();
        else if(abajo->getPadrePuerta() != nullptr)
            b = abajo->getPadrePuerta()->getSalida()->get();
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

    siguiente = ret;
}

bool Puerta::simularAntiguo()
{
    bool a{false};
    bool b{false};
    bool ret{false};

    if(simulandoAntiguo)
        return salida.get();

    simulandoAntiguo = true;

    if(arriba != nullptr)
    {
        if(arriba->getPadreEntrada() != nullptr)
            a = arriba->getPadreEntrada()->get();
        else if(arriba->getPadrePuerta() != nullptr)
            a = arriba->getPadrePuerta()->simularAntiguo();
    }

    if(abajo != nullptr)
    {
        if(abajo->getPadreEntrada() != nullptr)
            b = abajo->getPadreEntrada()->get();
        else if(abajo->getPadrePuerta() != nullptr)
            b = abajo->getPadrePuerta()->simularAntiguo();
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

void Puerta::simulacionAntiguaTermindada()
{
    simulandoAntiguo = false;
}

void Puerta::actualizar()
{
    salida.set(siguiente);
}

void Puerta::moverRel(const int x_, const int y_)
{
    imagen.mover(imagen.getRect()->x + x_, imagen.getRect()->y + y_);
    lineaArriba.first += x_;
    lineaArriba.second += y_;
    lineaAbajo.first += x_;
    lineaAbajo.second += y_;
    lineaSalida.first += x_;
    lineaSalida.second += y_;
    x += x_;
    y += y_;
}
