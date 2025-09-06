#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "./Wrapper/text.h"
#include "./Wrapper/window.h"
#include "controlador.h"
#include "controles.h"
#include "SistemaGuardado.h"

#include <print>

auto main() -> int
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::println(std::cerr, "Error initializing SDL: {}", SDL_GetError());
        return 1;
    }

    std::atexit(SDL_Quit);

    if(TTF_Init() != 0)
    {
        std::println(std::cerr, "Error initializing SDL_ttf: {}", TTF_GetError());
        return 1;
    }
    std::atexit(TTF_Quit);

    Window window{1920, 1080, "/home/julen/Descargas/windows_xp_bliss-wide.png"};

    TXT txt{"ttf/8bitOperatorPlusSC-Regular.ttf", 20, SDL_Color{255, 255, 255, 255}, window.getRenderer()};
    if(!txt)
    {
        std::println(std::cerr, "Error al inicializar el texto. Error {}", SDL_GetError());
        return 1;
    }

    Controlador controlador{&window, &txt};
    window.setControlador(&controlador);

    if(!window)
    {
        std::println(std::cerr, "Error al inicializar la ventana. Error {}", SDL_GetError());
        return 1;
    }


    window.render();

    SDL_Event event;
    // ReSharper disable CppDFALocalValueEscapesFunction
    Controles::init(&event, &window);
    // ReSharper restore CppDFALocalValueEscapesFunction
    bool enMarcha = true;
    bool simulando = false;
    bool mostrarControles = true;
    bool guardar = false;
    bool cargar = false;

    std::string strTmp;

    Uint64 ultimaSimulacion = SDL_GetTicks64();
    SistemaGuardado::Valores valores;

    std::string inputStr;
    bool escribiendo = false;

    std::string mensaje;
    constexpr int mensajeDuracion = 5000;
    Uint64 mensajeTiempo = -mensajeDuracion;

    while(enMarcha)
    {
        while(SDL_PollEvent(&event) != 0)
        {
            switch(Controles::getNuevaAccion(&event))
            {
                case ACCION::BorrarSueltos:
                    mensaje = std::format("Se han borrado {} elemento(s)", controlador.limpiar());
                    mensajeTiempo = SDL_GetTicks64();
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
                case ACCION::CrearTemporizador:
                    controlador.crear(-1, -1, 10);
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
                case ACCION::CamaraCentrar:
                    window.centrar();
                    break;
                case ACCION::TextInput:
                    {
                        const SDL_Event* evento = Controles::getEvent();
                        switch(evento->type)
                        {
                            case SDL_KEYDOWN:
                                switch(evento->key.keysym.sym)
                                {
                                    case SDLK_RETURN:
                                        Controles::unsetTextInput();
                                        escribiendo = false;
                                        break;
                                    case SDLK_BACKSPACE:
                                        if(!inputStr.empty())
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
                                inputStr += evento->text.text; // NOLINT(*-pro-bounds-array-to-pointer-decay)
                                break;
                            default:
                                break;
                        }
                    }
                    break;
                case ACCION::CerrarTextInput:
                    Controles::unsetTextInput();
                    escribiendo = false;
                    guardar = false;
                    cargar = false;
                    break;
                case ACCION::DuplicarSeleccion:
                    controlador.duplicarSeleccion();
                    break;
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
                mensaje = "Se ha guardado";
            else
                mensaje = "No se ha podido guardar";
            mensajeTiempo = SDL_GetTicks64();
            guardar = false;
        }

        if(cargar && !escribiendo)
        {
            window.limpiar();
            txt.setPos(500, 500);
            txt << "Cargando...";
            window.render();
            const int ret = SistemaGuardado::cargar(&controlador, &window, &valores, inputStr);
            if(ret == -1)
                mensaje = "No se ha podido guardar";
            else if(ret == -2)
                mensaje = "No se puede cargar si se ha creado algo";
            else
                mensaje = "Se ha cargado";

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
            txt.setPos(0, 30);
            txt << "Creación:" << "    A: puerta AND" << "    O: puerta OR" << "    X: puerta XOR" << "    I: interruptor" << "    B: botón" << "    S: salida" << "    T: temporizador";
            txt << "Modificadores:" << "    Espacio: crear conexión" << "    Retroceso: modo borrar";
            if(!window.getRaton()->getBorrando())
                txt << "Ratón:" << "    Izquierda: interactuar" << "    Medio: crear conexión" << "    Derecha: mover" << "    Arriba: temporizador +" << "    Abajo: temporizador -";
            else
                txt << "Ratón:" << "    Izquierda: borrar elemento" << "    Medio: borrar conexión" << "    Arriba: temporizador +" << "    Abajo: temporizador -";
            txt << "Simulación:" << "    Entrar: simular una vez" << "    Q: empezar simulación" << "    W: parar simulación" << "    -: acelerar simulación" << "    +: decelerar simulación";
            txt << "Cuadrícula:" << "    E: alternar" << "    R: aumentar" << "    F: disminuir";
            txt << "Otros:" << "    L: borrar elementos desconectados" << "    H: centrar cámara" << "    D: Duplicar selección" << "    G: guardar" << "    C: cargar" << "    Escape: cerrar" << "    M: ocultar controles";
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
                txt.setPos(1700, 10);
                strTmp = std::format("SIMULANDO ({} ms)", valores.velocidadSimulacion);
            }
            else
            {
                txt.setPos(1640, 10);
                strTmp = std::format("SIMULANDO (instantáneo");
            }
            txt << &strTmp;
        }

        txt.setPos(10, 10);
        strTmp = std::format("X: {} Y: {}", -window.getEsquinaX(), -window.getEsquinaY());
        txt << &strTmp;

        if(controlador.getCuadriculaActiva())
        {
            strTmp = std::format("Cuadríacula activa. Tamaño: {}", controlador.getCuadriculaTamaño());
            txt << &strTmp;
        }

        if(mensajeTiempo + mensajeDuracion > SDL_GetTicks64())
        {
            txt.setPos(900, 1050);
            txt << mensaje.c_str();
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

