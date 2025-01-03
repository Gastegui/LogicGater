#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "./Wrapper/text.h"
#include "./Wrapper/window.h"
#include "controlador.h"
#include "controles.h"
#include "SistemaGuardado.h"

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

    window.render(false);

    SDL_Event event;
    Controles::init(&event, &window);
    bool enMarcha = true;
    bool simulando = false;
    bool mostrarControles = true;
    char strTMP[25] = {" "};
    Uint64 ultimaSimulacion = SDL_GetTicks64();
    int velocidadSimulacion = 50;

    char mensajeStr[40] = {" "};
    constexpr int mensajeDuracion = 5000;
    Uint64 mensajeTiempo = SDL_GetTicks64() + mensajeDuracion + 1;

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
                case Controles::SimularAcelerar:
                    if(velocidadSimulacion != 0)
                        velocidadSimulacion -= 5;
                    break;
                case Controles::SimularDecelerar:
                    velocidadSimulacion += 5;
                    break;
                case Controles::Cerrar:
                    enMarcha = false;
                    break;
                case Controles::MostrarControles:
                    mostrarControles = !mostrarControles;
                    break;
                case Controles::Guardar:
                    if(SistemaGuardado::guardar(&controlador, &window, velocidadSimulacion))
                        snprintf(mensajeStr, 40, "Se ha guardado");
                    else
                        snprintf(mensajeStr, 40, "No se ha podido guardar");
                    mensajeTiempo = SDL_GetTicks64();
                    break;
                case Controles::Cargar:
                    {
                        const int ret = SistemaGuardado::cargar(&controlador, &window);
                        if(ret == -1)
                            snprintf(mensajeStr, 40, "No se ha podido cargar");
                        else if(ret == -2)
                            snprintf(mensajeStr, 40, "No se puede cargar si se ha creado algo");
                        else
                        {
                            snprintf(mensajeStr, 40, "Se ha cargado");
                            velocidadSimulacion = ret;
                        }
                        mensajeTiempo = SDL_GetTicks64();
                    }
                    break;
                case Controles::Nada:
                default:
                    break;
            }
            if(enMarcha)
                window.manejarRaton();
        }

        if(simulando && ultimaSimulacion + velocidadSimulacion <= SDL_GetTicks64())
        {
            controlador.simular();
            ultimaSimulacion = SDL_GetTicks64();
        }

        window.limpiar();
        if(mostrarControles)
        {
            txt.setPos(0, 300);
            txt << "Creación:" << "    A: puerta AND" << "    O: puerta OR" << "    X: puerta XOR" << "    I: interruptor" << "    B: botón" << "    S: salida";
            txt << "Modificadores:" << "    Espacio: crear conexión" << "    Retroceso: modo borrar";
            if(!window.getRaton()->getBorrando())
                txt << "Ratón:" << "    Izquierda: interactuar" << "    Medio: crear conexión" << "    Derecha: Mover";
            else
                txt << "Ratón:" << "    Izquierda: borrar elemento" << "    Medio: borrar conexión";
            txt << "Simulación:" << "    Entrar: simular una vez" << "    Q: empezar simulación" << "    W: parar simulación" << "    -: acelerar simulación" << "    +: decelerar simulación";
            txt << "Otros:" << "    L: borrar elementos desconectados" << "    M: ocultar controles" << "    G: guardar" << "    C: cargar" << "    Escape: cerrar";
        }
        else
        {
            txt.setPos(0, 1050);
            txt << "M: mostrar controles";
        }

        if(simulando)
        {
            txt.setPos(1700, 1050);
            snprintf(strTMP, 25, "SIMULANDO (%d ms)", velocidadSimulacion);
            txt << strTMP;
        }

        txt.setPos(10, 10);
        snprintf(strTMP, 25, "X: %d Y: %d", -window.getEsquinaX(), -window.getEsquinaY());
        txt << strTMP;

        if(mensajeTiempo + mensajeDuracion > SDL_GetTicks64())
        {
            txt.setPos(900, 1050);
            txt << mensajeStr;
        }

        window.render(simulando);
        SDL_Delay(16);
    }
    //Mix_CloseAudio();
    return 0;
}

