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

#include "Idiomas.h"

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

    Idiomas idiomas{txt, IDIOMA::ES};

    window.render(true);

    SDL_Event event;
    // ReSharper disable CppDFALocalValueEscapesFunction
    Controles::init(&event, &window);
    // ReSharper restore CppDFALocalValueEscapesFunction
    bool simulando = false;

    Uint64 ultimaSimulacion = SDL_GetTicks64();
    SistemaGuardado::Valores valores;

    std::string inputStr;
    bool escribiendo = false;

    ID_TEXTO mensaje{ID_TEXTO::NADA};
    std::string mensajeParam;
    constexpr int mensajeDuracion = 5000;
    Uint64 mensajeTiempo = -mensajeDuracion;


    ESTADOS estado = ESTADOS::Normal;
    while(estado != ESTADOS::Cerrar && estado != ESTADOS::Error)
    {
        window.limpiar();
        switch(estado)
        {
            case ESTADOS::Cerrar:
            case ESTADOS::Error:
                break;
            case ESTADOS::Normal:
                while(SDL_PollEvent(&event) != 0)
                {
                    switch(Controles::getNuevaAccion(&event))
                    {
                        case ACCION::BorrarSueltos:
                            mensaje = ID_TEXTO::ElementoBorrados;
                            mensajeParam = std::to_string(controlador.limpiar());
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
                            estado = ESTADOS::Cerrar;
                            simulando = false;
                            break;
                        case ACCION::MostrarControles:
                            estado = ESTADOS::Controles;
                            simulando = false;
                            break;
                        case ACCION::Guardar:
                            estado = ESTADOS::Guardar;
                            simulando = false;
                            Controles::setTextInput();
                            inputStr = "";
                            escribiendo = true;
                            break;
                        case ACCION::Cargar:
                            estado = ESTADOS::Cargar;
                            simulando = false;
                            Controles::setTextInput();
                            inputStr = "";
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
                                        estado = ESTADOS::Cerrar;
                                        simulando = false;
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
                            break;
                        case ACCION::DuplicarSeleccion:
                            controlador.duplicarSeleccion();
                            break;
                        case ACCION::CambiarIdioma:
                            idiomas.cambiar();
                            mensaje = ID_TEXTO::IdiomaCambiado;
                            mensajeTiempo = SDL_GetTicks64();
                            break;
                        case ACCION::Nada:
                        default:
                            break;
                    }
                    if(estado != ESTADOS::Cerrar)
                        window.getRaton()->manejarRaton();
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

                    if(valores.velocidadSimulacion >= 0)
                        idiomas.print(ID_TEXTO::SimulandoMs, valores.velocidadSimulacion);
                    else
                        idiomas.print(ID_TEXTO::SimulandoInstantaneo);
                }

                txt.setPos(10, 10);
                txt << std::format("X: {} Y: {}", -window.getEsquinaX(), -window.getEsquinaY());

                idiomas.print(ID_TEXTO::MostrarControles);
                break;
            case ESTADOS::Guardar:
                while(SDL_PollEvent(&event) != 0)
                {
                    switch(Controles::getNuevaAccion(&event))
                    {
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
                                        estado = ESTADOS::Cerrar;
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
                            estado = ESTADOS::Normal;
                            Controles::unsetTextInput();
                            escribiendo = false;
                            break;
                        case ACCION::Nada:
                        default:
                            break;
                    }
                }

                if(escribiendo)
                {
                    idiomas.print(ID_TEXTO::NombreArchivo);
                    txt.setPos(840, -2);
                    txt << (inputStr.empty() ? " " : inputStr.c_str());
                }
                else
                {
                    if(SistemaGuardado::guardar(&controlador, &window, &valores, inputStr))
                        mensaje = ID_TEXTO::GuardadoSi;
                    else
                        mensaje = ID_TEXTO::GuardadoNo;
                    mensajeTiempo = SDL_GetTicks64();
                    estado = ESTADOS::Normal;
                }

                break;
            case ESTADOS::Cargar:
                while(SDL_PollEvent(&event) != 0)
                {
                    switch(Controles::getNuevaAccion(&event))
                    {
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
                                        estado = ESTADOS::Cerrar;
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
                            estado = ESTADOS::Normal;
                            Controles::unsetTextInput();
                            escribiendo = false;
                            break;
                        case ACCION::Nada:
                        default:
                            break;
                    }
                }

                if(escribiendo)
                {
                    idiomas.print(ID_TEXTO::NombreArchivo);
                    txt.setPos(840, -2);
                    txt << (inputStr.empty() ? " " : inputStr.c_str());
                }
                else
                {
                    window.limpiar();
                    idiomas.print(ID_TEXTO::Cargando);
                    window.render(false);
                    const int ret = SistemaGuardado::cargar(&controlador, &window, &valores, inputStr);
                    if(ret == -1)
                        mensaje = ID_TEXTO::CargadoNo;
                    else if(ret == -2)
                        mensaje = ID_TEXTO::CargadoNoPosible;
                    else
                        mensaje = ID_TEXTO::CargadoSi;

                    mensajeTiempo = SDL_GetTicks64();
                    estado = ESTADOS::Normal;
                }
                break;
            case ESTADOS::Controles:
                while(SDL_PollEvent(&event) != 0)
                {
                    switch(Controles::getNuevaAccion(&event))
                    {
                        case ACCION::Cerrar:
                        case ACCION::MostrarControles:
                            estado = ESTADOS::Normal;
                            break;
                        case ACCION::CambiarIdioma:
                            idiomas.cambiar();
                            mensaje = ID_TEXTO::IdiomaCambiado;
                            mensajeTiempo = SDL_GetTicks64();
                            break;
                        case ACCION::Nada:
                        default:
                            break;
                    }
                }

                if(estado == ESTADOS::Controles)
                {
                    //CREACIÓN
                    idiomas.print(ID_TEXTO::Creacion);
                    idiomas.print(ID_TEXTO::AND);
                    idiomas.print(ID_TEXTO::OR);
                    idiomas.print(ID_TEXTO::XOR);
                    idiomas.print(ID_TEXTO::Interruptor);
                    idiomas.print(ID_TEXTO::Boton);
                    idiomas.print(ID_TEXTO::Salida);
                    idiomas.print(ID_TEXTO::Temporizador);
                    idiomas.print(ID_TEXTO::Vacio1);
                    idiomas.print(ID_TEXTO::Vacio1);
                    //SIMULACIÓN
                    idiomas.print(ID_TEXTO::Simulacion);
                    idiomas.print(ID_TEXTO::SimularPaso);
                    idiomas.print(ID_TEXTO::SimularEmpezar);
                    idiomas.print(ID_TEXTO::SimularParar);
                    idiomas.print(ID_TEXTO::SimularAcelerar);
                    idiomas.print(ID_TEXTO::SimularDecelerar);
                    //MODIFICADORES
                    idiomas.print(ID_TEXTO::Modificadores);
                    idiomas.print(ID_TEXTO::CrearConexionEspacio);
                    idiomas.print(ID_TEXTO::ModoBorrar);
                    idiomas.print(ID_TEXTO::Vacio1);
                    //RATÓN
                    idiomas.print(ID_TEXTO::Raton);
                    idiomas.print(ID_TEXTO::Interactuar);
                    idiomas.print(ID_TEXTO::CrearConexionRaton);
                    idiomas.print(ID_TEXTO::Mover);
                    idiomas.print(ID_TEXTO::TemporizadorArriba);
                    idiomas.print(ID_TEXTO::TemporizadorAbajo);
                    //RATÓN (BORRANDO)
                    idiomas.print(ID_TEXTO::RatonBorrando);
                    idiomas.print(ID_TEXTO::BorrarElemento);
                    idiomas.print(ID_TEXTO::BorrarConexion);
                    idiomas.print(ID_TEXTO::TemporizadorArribaBorrando);
                    idiomas.print(ID_TEXTO::TemporizadorAbajoBorrando);
                    idiomas.print(ID_TEXTO::Vacio1);
                    //CUADRÍCULA
                    idiomas.print(ID_TEXTO::Cuadricula);
                    idiomas.print(ID_TEXTO::CuadriculaAlternar);
                    idiomas.print(ID_TEXTO::CuadriculaAumentar);
                    idiomas.print(ID_TEXTO::CuadriculaDisminuir);
                    idiomas.print(ID_TEXTO::Vacio1);
                    //OTROS
                    idiomas.print(ID_TEXTO::Otros);
                    idiomas.print(ID_TEXTO::BorrarElementosDesconectados);
                    idiomas.print(ID_TEXTO::CentrarCamara);
                    idiomas.print(ID_TEXTO::Duplicar);
                    idiomas.print(ID_TEXTO::Guardar);
                    idiomas.print(ID_TEXTO::Cargar);
                    idiomas.print(ID_TEXTO::Cerrar);
                    idiomas.print(ID_TEXTO::CambiarIdioma);
                    idiomas.print(ID_TEXTO::OcultarControles);
                }
                break;
            default:
                estado = ESTADOS::Error;
                std::cout << "Máquina de estados rota";
        }

        if(controlador.getCuadriculaActiva())
            idiomas.print(ID_TEXTO::CuadriculaActiva, controlador.getCuadriculaTamaño());

        if(mensajeTiempo + mensajeDuracion > SDL_GetTicks64())
        {
            switch(mensaje)
            {
                case ID_TEXTO::ElementoBorrados:
                    idiomas.print(mensaje, mensajeParam);
                    break;
                default:
                    idiomas.print(mensaje);
                    break;
            }
        }

        window.render(estado == ESTADOS::Normal);
        SDL_Delay(16);
    }
    //Mix_CloseAudio();
    return 0;
}

