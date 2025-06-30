//
// Created by julen on 25/08/24.
//

#include "salida.h"

#include "entrada.h"
#include "puerta.h"
#include "../controlador.h"

auto Salida::cambiar(const bool estado) -> void
{
    img.crearImagen<IMG::Capa>(
        IMG::Capa{.img = estado ? "./img/salida/encendido.png" : "./img/salida/apagado.png"},
        IMG::Capa{.img = seleccionado ? "./img/salida/seleccionado.png" : "./img/salida/no_seleccionado.png"}
    );
}


auto Salida::moverRel(const int x, const int y) -> void
{
    img.mover(img.getRect()->x + x, img.getRect()->y + y);
    entradaPos.first += x;
    entradaPos.second += y;
}

auto Salida::simular() -> void
{
    if(entrada == nullptr)
    {
        if(ultimoEstado)
        {
            cambiar(false);
            ultimoEstado = false;
        }
        return;
    }

    if(entrada->get() && !ultimoEstado)
        cambiar(true);
    else if(!entrada->get() && ultimoEstado)
        cambiar(false);

    ultimoEstado = entrada->get();
}

auto Salida::simularAntiguo() -> void
{
    if(entrada == nullptr)
    {
        if(ultimoEstado)
        {
            cambiar(false);
            ultimoEstado = false;
        }
        return;
    }

    if(entrada->getTipoSimulable() == TIPOS_SIMULABLES::Puerta)
        dynamic_cast<Puerta*>(entrada->getSimulable())->simularAntiguo();

    //Si la salida está directamente conectada a una entrada, al hacer entrada->get() ya se pilla su valor sin simular

    if(entrada->get() && !ultimoEstado)
        cambiar(true);
    else if(!entrada->get() && ultimoEstado)
        cambiar(false);

    ultimoEstado = entrada->get();
}

auto Salida::interactuar(const int posX, const int posY, const INTERACCIONES interaccion) -> bool
{
    using enum INTERACCIONES;

    switch(interaccion)
    {
        case ConexionArriba:
            {
                IO* origen = controlador->getOrigen();
                if(origen == nullptr || getEntrada() != nullptr)
                    return false;

                setEntrada(origen);
                origen->conectado();
                controlador->añadirConexion(this, 1);
                controlador->desmarcarOrigen();
                return true;
            }
        case ConexionBorrar:
            controlador->borrarConexion(this, entrada, 1);
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
        case RatonSalir:
            mover = false;
            return true;

        default:
            return false;
    }
}

auto Salida::setIONull(IO* io) -> void
{
    if(entrada == io)
        entrada = nullptr;
}
