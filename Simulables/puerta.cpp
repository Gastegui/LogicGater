//
// Created by julen on 22/08/24.
//

#include "puerta.h"
#include "entrada.h"
#include "../controlador.h"


void Puerta::simular()
{
    bool a{false};
    bool b{false};
    bool ret{false};

    if(arriba != nullptr)
    {
        if(arriba->getTipoSimulable() == TIPOS_SIMULABLES::Entrada)
            a = dynamic_cast<Entrada*>(arriba->getSimulable())->get();
        else if(arriba->getTipoSimulable() == TIPOS_SIMULABLES::Puerta)
            a = dynamic_cast<Puerta*>(arriba->getSimulable())->getSalida()->get();
    }

    if(abajo != nullptr)
    {

        if(abajo->getTipoSimulable() == TIPOS_SIMULABLES::Entrada)
            b = dynamic_cast<Entrada*>(abajo->getSimulable())->get();
        else if(abajo->getTipoSimulable() == TIPOS_SIMULABLES::Puerta)
            b = dynamic_cast<Puerta*>(abajo->getSimulable())->getSalida()->get();
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

bool Puerta::simularAntiguo() // NOLINT(*-no-recursion)
{
    bool a{false};
    bool b{false};
    bool ret{false};

    if(simulandoAntiguo)
        return salida.get();

    simulandoAntiguo = true;


    if(arriba != nullptr)
    {
        if(arriba->getTipoSimulable() == TIPOS_SIMULABLES::Entrada)
            a = dynamic_cast<Entrada*>(arriba->getSimulable())->get();
        else if(arriba->getTipoSimulable() == TIPOS_SIMULABLES::Puerta)
            a = dynamic_cast<Puerta*>(arriba->getSimulable())->getSalida()->get();
    }

    if(abajo != nullptr)
    {

        if(abajo->getTipoSimulable() == TIPOS_SIMULABLES::Entrada)
            b = dynamic_cast<Entrada*>(abajo->getSimulable())->get();
        else if(abajo->getTipoSimulable() == TIPOS_SIMULABLES::Puerta)
            b = dynamic_cast<Puerta*>(abajo->getSimulable())->getSalida()->get();
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

    salida.moverLineaOrigenRel(x_, y_);

    if(arriba != nullptr)
        arriba->moverLineaDestinoRel(x_, y_);
    if(abajo != nullptr)
        abajo->moverLineaDestinoRel(x_, y_);

    x += x_;
    y += y_;
}

bool Puerta::interactuar(const int posX, const int posY, const INTERACCIONES interaccion) //TODO: Añadir algo más?
{
    using enum INTERACCIONES;
    switch (interaccion)
    {
        case ConexionAbajo:
            controlador->marcarOrigen(getSalida());
            return true;
        case ConexionArriba:
            if(controlador->getOrigen() == nullptr)
                return false;
            {
                const bool conectarArriba = posY <= getImg()->getRect()->h / 2;
                IO* origen = controlador->getOrigen();
                if(conectarArriba)
                {
                    if(arriba != nullptr)
                        return false;
                    arriba = origen;
                    origen->setLineaDestino(x + 0, y + 11);
                }
                else
                {
                    if(abajo != nullptr)
                        return false;
                    abajo = origen;
                    origen->setLineaDestino(x + 0, y + 37);
                }

                origen->conectado();

                controlador->desmarcarOrigen();
                controlador->añadirConexion(this, conectarArriba ? getArriba() : getAbajo());
            }
            return true;
        case ConexionBorrar:
            if(posX > getImg()->getRect()->w / 2)
                controlador->borrarConexiones(&salida);
            else
                controlador->borrarConexiones(posY <= getImg()->getRect()->h / 2 ? arriba : abajo);
            return true;

        case MoverAbajo: //TODO: QUE SE USEN TODAS LAS SIGUIENTES (antigua funcion click())
            mover = true;
            raton->setMoviendoImg(&imagen);
            return true;
        case MoverArriba:
            mover = false;
            raton->setMoviendoImg(nullptr);
            return true;
        case MovimientoRaton:
            if(mover)
                moverRel(posX, posY);
            return true;

        case InteractuarArriba:
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
            return true;
        case RatonSalir:
            mover = false;
            return true;
        default:
            return false;
    }
}

void Puerta::setIONull(IO* io)
{
    if(arriba != io)
        arriba = nullptr;
    else if(abajo == io)
        abajo = nullptr;
}
