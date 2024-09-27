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

    const TXT txt {"ttf/8bitOperatorPlusSC-Regular.ttf", 20, SDL_Color{255, 255, 255, 255}, &window};
    if(!txt)
    {
        std::cerr << "Error al inicializar el texto. Error " << SDL_GetError() << std::endl;
        return 1;
    }

    window.render();

    SDL_Event event;
    Controles::init(&event);
    bool running = true;
    bool simulando = false;
    while (running)
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
                    running = false;
                    break;
                case Controles::Nada:
                default:
                    break;
            }
            if(running)
                window.manejarRaton();
        }

        if(simulando)
            controlador.simular();

        window.limpiar();
        txt.write(0, 1020, "a: puerta AND | o: puerta OR | x: puerta XOR | i: interruptor | b: botón | s: salida | borrar: modo borrar | espacio: simular un paso | l: borrar todos los elementos no conectados | alt + f4: salir");
        if(!window.getRaton()->getBorrando())
            txt.write(0, 1050, "ratón izquierdo: mover | ratón rueda: conectar | ratón derecha: cambiar puerta o invertir polaridad o encender/apagar botones e interruptores");
        else
            txt.write(0, 1050, "ratón izquierdo: borrar puerta/entrada/salida | ratón rueda: borrar conexión");

        window.render();
        SDL_Delay(16);
    }
    //Mix_CloseAudio();
    return 0;
}
