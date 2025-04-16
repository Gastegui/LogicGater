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

    window.render();

    SDL_Event event;
    Controles::init(&event, &window);
    bool enMarcha = true;
    bool simulando = false;
    bool mostrarControles = true;
    bool guardar = false;
    bool cargar = false;

    char strTMP[50] = {" "};

    Uint64 ultimaSimulacion = SDL_GetTicks64();
    SistemaGuardado::Valores valores;

    std::string inputStr = {" "};
    bool escribiendo = false;

    char mensajeStr[40] = {" "};
    constexpr int mensajeDuracion = 5000;
    Uint64 mensajeTiempo = SDL_GetTicks64() + mensajeDuracion + 1;

    while (enMarcha)
    {
        while (SDL_PollEvent(&event))
        {
            switch (Controles::getNuevaAccion(&event))
            {
                case ACCION::BorrarSueltos:
                    printf("Se han borrado %d elementos\n", controlador.limpiar());
                    break;
                case ACCION::CrearAnd:
                    controlador.crear(Puerta::AND, -1, -1);
                    break;
                case ACCION::CrearOr:
                    controlador.crear(Puerta::OR, -1, -1);
                    break;
                case ACCION::CrearXor:
                    controlador.crear(Puerta::XOR, -1, -1);
                    break;
                case ACCION::CrearInterruptor:
                    controlador.crear(false, -1, -1);
                    break;
                case ACCION::CrearBoton:
                    controlador.crear(true, -1, -1);
                    break;
                case ACCION::CrearSalida:
                    controlador.crear(-1, -1);
                    break;
                case ACCION::AlternarBorrando:
                    window.getRaton()->setBorrando(!window.getRaton()->getBorrando());
                    break;
                case ACCION::SimularPaso:
                    controlador.simular();
                    simulando = false;
                    break;
                case ACCION::SimularEmpezar:
                    simulando = true;
                    break;
                case ACCION::SimularParar:
                    simulando = false;
                    break;
                case ACCION::SimularAcelerar:
                    if(valores.velocidadSimulacion != -5)
                        valores.velocidadSimulacion -= 5;
                    break;
                case ACCION::SimularDecelerar:
                    valores.velocidadSimulacion += 5;
                    break;
                case ACCION::Cerrar:
                    enMarcha = false;
                    break;
                case ACCION::MostrarControles:
                    mostrarControles = !mostrarControles;
                    break;
                case ACCION::Guardar:
                    Controles::setTextInput();
                    inputStr = "";
                    guardar = true;
                    escribiendo = true;
                    break;
                case ACCION::Cargar:
                    Controles::setTextInput();
                    inputStr = "";
                    cargar = true;
                    escribiendo = true;
                    break;
                case ACCION::Cuadricula:
                    controlador.alternarCuadricula();
                    break;
                case ACCION::CuadriculaAgrandar:
                    valores.cuadriculaTamaño = controlador.cambiarCuadriculaRel(5);
                    break;
                case ACCION::CuadriculaDisminuir:
                    valores.cuadriculaTamaño = controlador.cambiarCuadriculaRel(-5);
                    break;
                case ACCION::TextInput:
                    {
                        const SDL_Event* evento = Controles::getEvent();
                        switch (evento->type)
                        {
                            case SDL_KEYDOWN:
                                switch(evento->key.keysym.sym)
                                {
                                    case SDLK_RETURN:
                                        Controles::unsetTextInput();
                                        escribiendo = false;
                                        break;
                                    case SDLK_BACKSPACE:
                                        if (!inputStr.empty())
                                            inputStr.pop_back();
                                        break;
                                    default:
                                        break;
                                }
                                break;
                            case SDL_QUIT:
                                enMarcha = false;
                                break;
                            case SDL_TEXTINPUT:
                                inputStr += evento->text.text;
                                break;
                            default:
                                break;
                        }
                    }
                case ACCION::Nada:
                default:
                    break;
            }
            if(enMarcha)
                window.getRaton()->manejarRaton();
        }

        if(guardar && !escribiendo)
        {
            if(SistemaGuardado::guardar(&controlador, &window, &valores, inputStr))
                snprintf(mensajeStr, 40, "Se ha guardado");
            else
                snprintf(mensajeStr, 40, "No se ha podido guardar");
            mensajeTiempo = SDL_GetTicks64();
            guardar = false;
        }

        if(cargar && !escribiendo)
        {
            window.limpiar();
            txt.setPos(500, 500);
            txt << "Cargando...";
            window.render();
            printf("%s\n", inputStr.c_str());
            const int ret = SistemaGuardado::cargar(&controlador, &window, &valores, inputStr);
            if(ret == -1)
                snprintf(mensajeStr, 40, "No se ha podido cargar");
            else if(ret == -2)
                snprintf(mensajeStr, 40, "No se puede cargar si se ha creado algo");
            else
                snprintf(mensajeStr, 40, "Se ha cargado");

            mensajeTiempo = SDL_GetTicks64();
            cargar = false;
        }

        if(simulando)
        {
            if(valores.velocidadSimulacion >= 0 && ultimaSimulacion + valores.velocidadSimulacion <= SDL_GetTicks64())
            {
                controlador.simular();
                ultimaSimulacion = SDL_GetTicks64();
            }
            else if(valores.velocidadSimulacion < 0)
                controlador.simularInstantaneo();
        }

        window.limpiar();
        if(mostrarControles)
        {
            txt.setPos(0, 180);
            txt << "Creación:" << "    A: puerta AND" << "    O: puerta OR" << "    X: puerta XOR" << "    I: interruptor" << "    B: botón" << "    S: salida";
            txt << "Modificadores:" << "    Espacio: crear conexión" << "    Retroceso: modo borrar";
            if(!window.getRaton()->getBorrando())
                txt << "Ratón:" << "    Izquierda: interactuar" << "    Medio: crear conexión" << "    Derecha: Mover";
            else
                txt << "Ratón:" << "    Izquierda: borrar elemento" << "    Medio: borrar conexión";
            txt << "Simulación:" << "    Entrar: simular una vez" << "    Q: empezar simulación" << "    W: parar simulación" << "    -: acelerar simulación" << "    +: decelerar simulación";
            txt << "Cuadrícula:" << "    E: alternar" << "    R: aumentar" << "    F: disminuir";
            txt << "Otros:" << "    L: borrar elementos desconectados" << "    M: ocultar controles" << "    G: guardar" << "    C: cargar" << "    Escape: cerrar";
        }
        else
        {
            txt.setPos(0, 1050);
            txt << "M: mostrar controles";
        }

        if(simulando)
        {
            if(valores.velocidadSimulacion >= 0)
            {
                txt.setPos(1700, 1050);
                snprintf(strTMP, 50, "SIMULANDO (%d ms)", valores.velocidadSimulacion);
            }
            else
            {
                txt.setPos(1640, 1050);
                snprintf(strTMP, 50, "SIMULANDO (instantáneo)");
            }
            txt << strTMP;
        }

        txt.setPos(10, 10);
        snprintf(strTMP, 50, "X: %d Y: %d", -window.getEsquinaX(), -window.getEsquinaY());
        txt << strTMP;

        if(controlador.getCuadriculaActiva())
        {
            snprintf(strTMP, 50, "Cuadrícula activa. Tamaño: %d", controlador.getCuadriculaTamaño());
            txt << strTMP;
        }

        if(mensajeTiempo + mensajeDuracion > SDL_GetTicks64())
        {
            txt.setPos(900, 1050);
            txt << mensajeStr;
        }

        if(cargar && escribiendo)
        {
            txt.setPos(500, 500);
            txt << "Escribe el nombre del archivo: ";
            txt.setPos(840, 500);
            txt << (inputStr.empty() ? " " : inputStr.c_str());
        }
        if(guardar && escribiendo)
        {
            txt.setPos(500, 500);
            txt << "Escribe el nombre del archivo: ";
            txt.setPos(840, 500);
            txt << (inputStr.empty() ? " " : inputStr.c_str());
        }

        window.render();
        SDL_Delay(16);
    }
    //Mix_CloseAudio();
    return 0;
}

