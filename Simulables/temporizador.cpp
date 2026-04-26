//
// Created by julen on 25/04/25.
//

#include "temporizador.h"
#include "../controlador.h"

auto Temporizador::cambiar() -> bool
{
    std::string relleno = "./img/temporizador/" + std::to_string(ciclosActuales == 0 ? 0 : static_cast<int>(static_cast<float>(ciclosActuales) / ciclosTotales * 10)); // NOLINT(*-narrowing-conversions)
    relleno.append(".png");
    return imagen.crearImagen<IMG::Capa>(
        IMG::Capa{.img = entradaNegada ? "./img/temporizador/entrada_negada.png" : "./img/temporizador/entrada_normal.png"},
        IMG::Capa{.img = "./img/temporizador/cuerpo.png"},
        IMG::Capa{.img = salidaNegada ? "./img/temporizador/salida_negada.png" : "./img/temporizador/salida_normal.png"},
        IMG::Capa{.img = relleno},
        IMG::Capa{.img = seleccionado ? "./img/temporizador/seleccionado.png" : "./img/temporizador/no_seleccionado.png"},
        IMG::Capa{.texto = std::to_string(ciclosActuales), .textoX = 40, .textoY = 27},
        IMG::Capa{.texto = std::to_string(ciclosTotales), .textoX = 40, .textoY = -8}
    );
}


auto Temporizador::simular() -> void
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

auto Temporizador::actualizar() -> void
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
    switch(interaccion)
    {
        case ConexionAbajo:
            Controlador::get().marcarOrigen(&salida);
            return true;
        case ConexionArriba:
            {
                IO* origen = Controlador::get().getOrigen();
                if(origen == nullptr || entrada != nullptr)
                    return false;

                entrada = origen;
                origen->conectado();

                Controlador::get().añadirConexion(this, 1);
                Controlador::get().desmarcarOrigen();
            }
            return true;
        case ConexionBorrar:
            if(posX > getImg()->getRect()->w / 2)
                Controlador::get().borrarConexiones(&salida);
            else
            {
                if(entrada == nullptr)
                    return false;
                Controlador::get().borrarConexion(this, entrada, 1);
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
            Raton::get().setMoviendoImg(&imagen);
            return true;
        case MoverArriba:
            mover = false;
            Raton::get().setMoviendoImg(nullptr);
            return true;
        case MovimientoRaton:
            if(mover)
            {
                imagen.moverRel(posX, posY);

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

