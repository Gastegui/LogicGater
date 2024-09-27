//
// Created by julen on 26/09/24.
//

#include "controles.h"

#include <iostream>

Controles::Accion Controles::accionAnterior = Nada;
SDL_Event* Controles::evento = nullptr;
std::map<std::string, Controles::Accion> Controles::acciones;
Window* Controles::window = nullptr;

Controles::Accion Controles::getNuevaAccion(const SDL_Event* evento)
{
    accionAnterior = Nada;
    std::string str = "--";
    if(evento->type == SDL_KEYDOWN || evento->type == SDL_MOUSEBUTTONDOWN)
        str[0] = 'p';
    else if (evento->type == SDL_KEYUP || evento->type == SDL_MOUSEBUTTONUP)
        str[0] = 's';
    switch (evento->type)
    {
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            if(evento->button.button == SDL_BUTTON_LEFT)
                str[1] = 'I';
            else if(evento->button.button == SDL_BUTTON_MIDDLE)
                str[1] = 'M';
            else if(evento->button.button == SDL_BUTTON_RIGHT)
                str[1] = 'D';
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if((evento->key.keysym.sym >= 'a' && evento->key.keysym.sym <= 'z') || (evento->key.keysym.sym >= '0' && evento->key.keysym.sym <= '9'))
                str[1] = static_cast<char>(evento->key.keysym.sym);
            else
            {
                switch(evento->key.keysym.sym)
                {
                    case SDLK_SPACE:
                        str[1] = 'E';
                        break;
                    case SDLK_RETURN:
                        str[1] = 'R';
                        break;
                    case SDLK_BACKSPACE:
                        str[1] = 'B';
                        break;
                    case SDLK_TAB:
                        str[1] = 'T';
                        break;
                    case SDLK_LCTRL:
                    case SDLK_RCTRL:
                        str[1] = 'C';
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        str[1] = 'S';
                        break;
                    case SDLK_LALT:
                    case SDLK_RALT:
                        str[1] = 'A';
                        break;
                    case SDLK_ESCAPE:
                        str[1] = 'X';
                        break;
                    default:
                        break;
                }
            }
            break;
        case SDL_QUIT:
            accionAnterior = Cerrar;
            break;
        case SDL_MOUSEMOTION:
            accionAnterior = MovimientoRaton;
            break;
        default:
            break;
    }
    if(str != "--" && acciones.contains(str))
        accionAnterior = acciones[str];
    if(window != nullptr && window->getRaton()->getBorrando() && !(accionAnterior == Interactuar || accionAnterior == InteractuarAbajo ||
                                                                    accionAnterior == ConexionArriba || accionAnterior == ConexionAbajo ||
                                                                    accionAnterior == MovimientoRaton))
        window->getRaton()->setBorrando(false);
    return accionAnterior;
}
