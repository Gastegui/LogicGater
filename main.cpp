#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "./Wrapper/text.h"
#include "./Wrapper/window.h"
#include "controlador.h"

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
    bool running = true;
    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
            window.manejarRaton(&event);

            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    controlador.simular();
                    break;
                case SDLK_BACKSPACE:
                    window.getRaton()->setBorrando(!window.getRaton()->getBorrando());
                    break;
                case 'a':
                    controlador.crear(Puerta::AND, 0, 0);
                    break;
                case 'o':
                    controlador.crear(Puerta::OR, 0, 0);
                    break;
                case 'x':
                    controlador.crear(Puerta::XOR, 0, 0);
                    break;
                case 'i':
                    controlador.crear(false, 0, 0);
                    break;
                case 'b':
                    controlador.crear(true, 0, 0);
                    break;
                case 's':
                    controlador.crear(0, 0);
                    break;
                default:
                    break;
                }
            }
        }

        window.limpiar();
        txt.write(0, 1020, "a: puerta AND | o: uerta OR | x: puerta XOR | i: interruptor | b: botón | s: salida | borrar: modo borrar | espacio: simular un paso | alt + f4: salir");
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
