#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "./Wrapper/text.h"
#include "./Wrapper/window.h"
#include "controlador.h"
#include "controles.h"

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::atexit(SDL_Quit);

    if (TTF_Init() != 0)
    {
        std::cerr << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return 1;
    }
    std::atexit(TTF_Quit);

    Window window{"/home/julen/Descargas/windows_xp_bliss-wide.png"};
    Controlador controlador{&window};
    window.setControlador(&controlador);

    if(!window)
    {
        std::cerr << "Error al inicializar la ventana. Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    TXT txt {"ttf/8bitOperatorPlusSC-Regular.ttf", 20, SDL_Color{255, 255, 255, 255}, window.getRenderer()};
    if(!txt)
    {
        std::cerr << "Error al inicializar el texto. Error " << SDL_GetError() << std::endl;
        return 1;
    }

    window.render();

    SDL_Event event;
    Controles::init(&event, &window);
    bool enMarcha = true;
    bool simulando = false;
    while (enMarcha)
    {
        while (SDL_PollEvent(&event))
        {
            switch (Controles::getNuevaAccion(&event))
            {
                case Controles::BorrarSueltos:
                    printf("Se han borrado %d elementos\n", controlador.limpiar());
                    break;
                case Controles::CrearAnd:
                    controlador.crear(Puerta::AND, -1, -1);
                    break;
                case Controles::CrearOr:
                    controlador.crear(Puerta::OR, -1, -1);
                    break;
                case Controles::CrearXor:
                    controlador.crear(Puerta::XOR, -1, -1);
                    break;
                case Controles::CrearInterruptor:
                    controlador.crear(false, -1, -1);
                    break;
                case Controles::CrearBoton:
                    controlador.crear(true, -1, -1);
                    break;
                case Controles::CrearSalida:
                    controlador.crear(-1, -1);
                    break;
                case Controles::AlternarBorrando:
                    window.getRaton()->setBorrando(!window.getRaton()->getBorrando());
                    break;
                case Controles::SimularPaso:
                    controlador.simular();
                    simulando = false;
                    break;
                case Controles::SimularEmpezar:
                    simulando = true;
                    break;
                case Controles::SimularParar:
                    simulando = false;
                    break;
                case Controles::Cerrar:
                    enMarcha = false;
                    break;
                case Controles::Nada:
                default:
                    break;
            }
            if(enMarcha)
                window.manejarRaton();
        }

        if(simulando)
            controlador.simular();

        window.limpiar();
        txt.setPos(0, 460);
        txt << "Creación:" << "    A: puerta AND" << "    O: puerta OR" << "    X: puerta XOR" << "    I: interruptor" << "    B: botón" << "    S: salida";
        txt << "Modificadores:" << "    Espacio: crear conexión" << "    Retroceso: modo borrar";
        if(!window.getRaton()->getBorrando())
            txt << "Ratón:" << "    Izquierda: interactuar" << "    Medio: crear conexión" << "    Derecha: Mover";
        else
            txt << "Ratón:" << "    Izquierda: borrar elemento" << "    Medio: borrar conexión";
        txt << "Simulación:" << "    Entrar: simular una vez" << "    Q: empezar simulación" << "    W: parar simulación";
        txt << "Otros:" << "    L: borrar elementos desconectados";

        window.render();
        SDL_Delay(16);
    }
    //Mix_CloseAudio();
    return 0;
}
