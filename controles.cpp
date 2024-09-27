//
// Created by julen on 26/09/24.
//

#include "controles.h"

Controles::Accion Controles::accionAnterior = Nada;
SDL_Event* Controles::evento = nullptr;

Controles::Accion Controles::getNuevaAccion(const SDL_Event* evento)
{
    accionAnterior = Nada;
    switch (evento->type)
    {
        case SDL_QUIT:
            accionAnterior = Cerrar;
            break;
        case SDL_MOUSEMOTION:
            accionAnterior = MovimientoRaton;
            break;
        case SDL_MOUSEBUTTONDOWN:
            switch (evento->button.button)
            {
                case SDL_BUTTON_LEFT:
                    accionAnterior = InteractuarAbajo;
                    break;
                case SDL_BUTTON_MIDDLE:
                    accionAnterior = ConexionAbajo;
                    break;
                case SDL_BUTTON_RIGHT:
                    accionAnterior = MoverAbajo;
                    break;
                default:
                    break;
            }
            break;
        case SDL_MOUSEBUTTONUP:
            switch (evento->button.button)
            {
                case SDL_BUTTON_LEFT:
                    accionAnterior = Interactuar;
                    break;
                case SDL_BUTTON_MIDDLE:
                    accionAnterior = ConexionArriba;
                    break;
                case SDL_BUTTON_RIGHT:
                    accionAnterior = MoverArriba;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYDOWN:
            switch (evento->key.keysym.sym)
            {
                case SDLK_SPACE:
                    accionAnterior = ConexionAbajo;
                    break;
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch(evento->key.keysym.sym)
            {
                case SDLK_SPACE:
                    accionAnterior = ConexionArriba;
                    break;
                case SDLK_BACKSPACE:
                    accionAnterior = AlternarBorrando;
                    break;
                case SDLK_RETURN:
                    accionAnterior = SimularPaso;
                    break;
                case 'q':
                    accionAnterior = SimularEmpezar;
                    break;
                case 'w':
                    accionAnterior = SimularParar;
                    break;
                case 'l':
                    accionAnterior = BorrarSueltos;
                    break;
                case 'a':
                    accionAnterior = CrearAnd;
                    break;
                case 'o':
                    accionAnterior = CrearOr;
                    break;
                case 'x':
                    accionAnterior = CrearXor;
                    break;
                case 'i':
                    accionAnterior = CrearInterruptor;
                    break;
                case 'b':
                    accionAnterior = CrearBoton;
                    break;
                case 's':
                    accionAnterior = CrearSalida;
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }
    return accionAnterior;
}
