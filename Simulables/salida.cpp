//
// Created by julen on 25/08/24.
//

#include "salida.h"

#include "entrada.h"
#include "puerta.h"
#include "../controlador.h"


void Salida::moverRel(const int x, const int y)
{
    img.mover(img.getRect()->x + x, img.getRect()->y + y);
    entradaPos.first += x;
    entradaPos.second += y;
}

void Salida::simular()
{
    if(entrada == nullptr )
    {
        img.cambiarImagen("./img/salida/apagado.png");
        return;
    }

    if(entrada->get())
        img.cambiarImagen("./img/salida/encendido.png");
    else
        img.cambiarImagen("./img/salida/apagado.png");
}

void Salida::simularAntiguo()
{
    if(entrada == nullptr )
    {
        img.cambiarImagen("./img/salida/apagado.png");
        return;
    }

    if(entrada->getTipoSimulable() == TIPOS_SIMULABLES::Puerta)
        dynamic_cast<Puerta*>(entrada->getSimulable())->simularAntiguo();

    //Si la salida está directamente conectada a una entrada, al hacer entrada->get() ya se pilla su valor sin simular

    if(entrada->get())
        img.cambiarImagen("./img/salida/encendido.png");
    else
        img.cambiarImagen("./img/salida/apagado.png");
}

bool Salida::interactuar(const int posX, const int posY, const INTERACCIONES interaccion)
{
    using enum INTERACCIONES;

    switch (interaccion)
    {
        case ConexionArriba:
            {
                IO* origen = controlador->getOrigen();
                if(origen == nullptr || getEntrada() != nullptr)
                    return false;

                setEntrada(origen);
                origen->conectado();
                controlador->añadirConexion(this);
                controlador->desmarcarOrigen();
                return true;
            }
        case ConexionBorrar:
            controlador->borrarConexion(entrada->getSimulable(), this, entrada);
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

void Salida::setIONull(IO* io)
{
    if(entrada == io)
        entrada = nullptr;
}
