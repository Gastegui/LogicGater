#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

#include "./Wrapper/text.h"
#include "./Wrapper/window.h"
#include "Wrapper/image.h"
#include "controlador.h"
#include "controles.h"
#include "SistemaGuardado.h"

#include <print>

#include "Idiomas.h"
#include "enums.h"

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

    Window& window = Window::get();
    TXT& txt = TXT::get();
    Controlador& controlador = Controlador::get();
    Raton& raton = Raton::get();

    Idiomas idiomas{IDIOMA::ES};

    window.render(true);

    SDL_Event event;
    // ReSharper disable CppDFALocalValueEscapesFunction
    Controles::init(&event);
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

    IMG imagenTeclado{"./img/Teclado.png", Window::getRenderer(), 500, 100};
    window.añadir(ESTADOS::Controles, &imagenTeclado, ListaIMG::Frente);

    while(controlador.getEstado() != ESTADOS::Cerrar && controlador.getEstado() != ESTADOS::Error)
    {
        window.limpiar();
        switch(controlador.getEstado())
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
                            controlador.crear(TIPOS_PUERTA::AND, -1, -1);
                            break;
                        case ACCION::CrearOr:
                            controlador.crear(TIPOS_PUERTA::OR, -1, -1);
                            break;
                        case ACCION::CrearXor:
                            controlador.crear(TIPOS_PUERTA::XOR, -1, -1);
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
                        case ACCION::CrearCondensador:
                            controlador.crear(-1, -1, TIPOS_PUERTA::OR);
                            break;
                        case ACCION::AlternarBorrando:
                            raton.setBorrando(!raton.getBorrando());
                            break;
                        case ACCION::SimularPaso:
                            controlador.simular();
                            simulando = false;
                            break;
                        case ACCION::SimularEmpezarParar:
                            simulando = !simulando;
                            break;
                        case ACCION::SimularAcelerar:
                            if(valores.velocidadSimulacion != -5)
                                valores.velocidadSimulacion -= 5;
                            break;
                        case ACCION::SimularDecelerar:
                            valores.velocidadSimulacion += 5;
                            break;
                        case ACCION::Cerrar:
                            controlador.setEstado(ESTADOS::Cerrar);
                            simulando = false;
                            break;
                        case ACCION::MostrarControles:
                            controlador.setEstado(ESTADOS::Controles);
                            simulando = false;
                            break;
                        case ACCION::Guardar:
                            controlador.setEstado(ESTADOS::Guardar);
                            simulando = false;
                            Controles::setTextInput();
                            inputStr = "";
                            escribiendo = true;
                            break;
                        case ACCION::Cargar:
                            controlador.setEstado(ESTADOS::Cargar);
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
                                        controlador.setEstado(ESTADOS::Cerrar);
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
                        case ACCION::AlternarOculto:
                            controlador.alternarConexionesOcultas();
                            break;
                        case ACCION::Nada:
                        default:
                            break;
                    }
                    if(controlador.getEstado() != ESTADOS::Normal)
                        break;
                    raton.manejarRaton();
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

                if(controlador.getConexionesOcultas())
                    idiomas.print(ID_TEXTO::ConexionesOcultas);

                txt.setPos(10, 10);
                txt << std::format("X: {} Y: {}", -window.getEsquinaX(), -window.getEsquinaY());

                idiomas.print(ID_TEXTO::MostrarControles);
                idiomas.print(ID_TEXTO::Elementos, controlador.getCantidadSimulables());
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
                                        controlador.setEstado(ESTADOS::Cerrar);
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
                            controlador.setEstado(ESTADOS::Normal);
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
                    if(SistemaGuardado::guardar(&valores, inputStr))
                        mensaje = ID_TEXTO::GuardadoSi;
                    else
                        mensaje = ID_TEXTO::GuardadoNo;
                    mensajeTiempo = SDL_GetTicks64();
                    controlador.setEstado(ESTADOS::Normal);
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
                                        controlador.setEstado(ESTADOS::Cerrar);
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
                            controlador.setEstado(ESTADOS::Normal);
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
                    controlador.setEstado(ESTADOS::Normal);
                    const int ret = SistemaGuardado::cargar(&valores, inputStr);
                    if(ret == -1)
                        mensaje = ID_TEXTO::CargadoNo;
                    else if(ret == -2)
                        mensaje = ID_TEXTO::CargadoNoPosible;
                    else
                        mensaje = ID_TEXTO::CargadoSi;

                    mensajeTiempo = SDL_GetTicks64();
                }
                break;
            case ESTADOS::Controles:
                while(SDL_PollEvent(&event) != 0)
                {
                    switch(Controles::getNuevaAccion(&event))
                    {
                        case ACCION::Cerrar:
                        case ACCION::MostrarControles:
                            controlador.setEstado(ESTADOS::Normal);
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

                if(controlador.getEstado() == ESTADOS::Controles)
                {
                    idiomas.print(ID_TEXTO::ControlesCreacion);
                    idiomas.print(ID_TEXTO::ControlesSimulacion);
                    idiomas.print(ID_TEXTO::ControlesModificadores);
                    idiomas.print(ID_TEXTO::ControlesRaton);
                    idiomas.print(ID_TEXTO::ControlesRatonBorrando);
                    idiomas.print(ID_TEXTO::ControlesCuadricula);
                    idiomas.print(ID_TEXTO::ControlesOtros);
                }
                break;
            default:
                controlador.setEstado(ESTADOS::Error);
                std::println(std::cout, "Máquina de estados rota");
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

        window.render(controlador.getEstado() == ESTADOS::Normal || controlador.getEstado() == ESTADOS::Controles);
        SDL_Delay(16);
    }
    //Mix_CloseAudio();
    return 0;
}
