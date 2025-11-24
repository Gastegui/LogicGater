//
// Created by julen on 11/9/25.
//

#include "condensador.h"

#include <algorithm>
#include "puerta.h"

auto Condensador::moverRel(int x, int y) -> void
{
    imagen.moverRel(x, y);
    salida.moverLineaOrigenRel(x, y);
    linea.first += x;
    linea.second += y;
}

auto Condensador::simular() -> void
{
    siguiente = false;
    if(entradas.empty())
        return;

    if(tipo == Puerta::AND)
    {
        siguiente = true;
        for(const IO* io : entradas)
        {
            if(!io->get())
            {
                siguiente = false;
                break;
            }
        }
    }
    else 
    {
        for(const IO* io : entradas)
        {
            if(io->get())
            {
                siguiente = true;
                break;
            }
        }
    }
}
auto Condensador::actualizar() -> void
{
    salida.set(siguiente);
}

auto Condensador::setIONull(IO* io) -> void
{
    auto it{std::ranges::find(entradas, io)};

    if(it != entradas.end())
        entradas.erase(it);
}

auto Condensador::interactuar(int posX, int posY, INTERACCIONES interaccion) -> bool
{
    using enum INTERACCIONES;
    switch(interaccion)
    {
        case ConexionAbajo:
            controlador->marcarOrigen(getIOSalida());
            return true;
        case ConexionArriba:
            {
                IO* origen = controlador->getOrigen();
                if(origen == nullptr || origen == getIOSalida())
                    return false;

                origen->conectado();
                entradas.push_back(origen);

                controlador->añadirConexion(this, 1);
                controlador->desmarcarOrigen();
            }
            return true;
        case ConexionBorrar:
            if(posX > getImg()->getRect()->w / 2)
                controlador->borrarConexiones(&salida);
            else
                for(IO* io : entradas)
                    controlador->borrarConexion(this, io, 1);
            return true;
        case InteractuarArriba:
            if(tipo == Puerta::AND)
                tipo = Puerta::OR;
            else
                tipo = Puerta::AND;
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
                moverRel(posX, posY);
            return true;
        case RatonSalir:
            mover = false;
            return true;
        case Rotar:
            switch(SDL_Rect rect{*imagen.getRect()}; imagen.getRotacion())
            {
                case 0:
                    imagen.setRotacion(90);
                    imagen.moverRel( -25,  25);
                    salida.moverLineaOrigenRel(-75, 75);
                    linea.first = rect.x + 25;
                    linea.second = rect.y;
                    break;
                case 90:
                    imagen.setRotacion(180);
                    imagen.moverRel( 25,  -25);
                    salida.moverLineaOrigenRel(-25, -75);
                    linea.first = rect.x + 100;
                    linea.second = rect.y + 25;
                    break;
                case 180:
                    imagen.setRotacion(270);
                    imagen.moverRel( -25,  25);
                    salida.moverLineaOrigenRel(25, -25);
                    linea.first = rect.x + 25;
                    linea.second = rect.y + 100;
                    break;
                case 270:
                    imagen.setRotacion(0);
                    imagen.moverRel( 25,  -25);
                    salida.moverLineaOrigenRel(75, 25);
                    linea.first = rect.x;
                    linea.second = rect.y + 25;
                    break;
                default:
                    return false;
            }
            return true;
        default:
            return false;
    }
}
