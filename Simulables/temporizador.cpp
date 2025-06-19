//
// Created by julen on 25/04/25.
//

#include "temporizador.h"
#include "../controlador.h"

auto Temporizador::cambiar() -> bool
{
    std::string relleno = "./img/temporizador/" + std::to_string(ciclosActuales == 0 ? 0 : static_cast<int>(static_cast<float>(ciclosActuales) / ciclosTotales * 10)); // NOLINT(*-narrowing-conversions)
    relleno.append(".png");
    return imagen.cambiarTemporizador(
    entradaNegada ? "./img/temporizador/entrada_negada.png" : "./img/temporizador/entrada_normal.png",
    "./img/temporizador/cuerpo.png",
    salidaNegada ? "./img/temporizador/salida_negada.png" : "./img/temporizador/salida_normal.png",
    relleno.c_str(), ciclosActuales, ciclosTotales);
}


void Temporizador::simular()
{
    if(entrada == nullptr)
        return;

    if(entradaNegada)
    {
        if(entrada->get())
            ciclosActuales = 0;
        else if(ciclosActuales != ciclosTotales)
            ciclosActuales++;
    }
    else
    {
        if(entrada->get())
        {
            if(ciclosActuales != ciclosTotales)
                ciclosActuales++;
        }
        else
            ciclosActuales = 0;
    }
}

void Temporizador::actualizar()
{
    if(salidaNegada)
    {
        if(entrada == nullptr)
            salida.set(true);
        else
            salida.set(ciclosActuales != ciclosTotales);
    }
    else
        salida.set(ciclosActuales == ciclosTotales && entrada != nullptr && (entradaNegada ? !entrada->get() : entrada->get()));
    cambiar();
}

auto Temporizador::interactuar(const int posX, const int posY, const INTERACCIONES interaccion) -> bool
{
    using enum INTERACCIONES;
    switch (interaccion)
    {
        case ConexionAbajo:
            controlador->marcarOrigen(&salida);
            return true;
        case ConexionArriba:
            {
                IO* origen = controlador->getOrigen();
                if(origen == nullptr || entrada != nullptr)
                    return false;

                entrada = origen;
                origen->conectado();

                controlador->añadirConexion(this, 1);
                controlador->desmarcarOrigen();
            }
            return true;
        case ConexionBorrar:
            if(posX > getImg()->getRect()->w / 2)
                controlador->borrarConexiones(&salida);
            else
            {
                if(entrada == nullptr)
                    return false;
                controlador->borrarConexion(this, entrada, 1);
            }
            return true;
        case TemporizadorSubir:
            ciclosTotales++;
            cambiar();
            return true;
        case TemporizadorBajar:
            if(ciclosTotales != 0)
                ciclosTotales--;
            cambiar();
            return true;
        case MoverAbajo:
            mover = true;
            raton->setMoviendoImg(&imagen);
            return true;
        case MoverArriba:
            mover = false;
            raton->setMoviendoImg(nullptr);
            return true;
        case MovimientoRaton:
            if(mover)
            {
                imagen.mover(imagen.getRect()->x + posX, imagen.getRect()->y + posY);

                salida.moverLineaOrigenRel(posX, posY);

                linea.first += posX;
                linea.second += posY;

                x += posX;
                y += posY;
            }
            return true;
        case InteractuarAbajo:
            interactuarAbajo = true;
            return true;
        case InteractuarArriba:
            if(!interactuarAbajo)
                return true;
            if(posX <= 50)
                entradaNegada = !entradaNegada;
            else
                salidaNegada = !salidaNegada;
            cambiar();
            return true;
        case RatonSalir:
            interactuarAbajo = false;
            mover = false;
            return true;
        default:
            return false;
    }

}

