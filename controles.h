//
// Created by julen on 26/09/24.
//

#ifndef CONTROLES_H
#define CONTROLES_H
#include <map>
#include <SDL_events.h>
#include <string>

#include "enums.h"

class Window;


class Controles
{
    static ACCION accionAnterior;
    static SDL_Event* evento;
    static std::map<std::string, ACCION> acciones;
    static Window* window;
    static bool escribiendo;

public:
    static auto init(SDL_Event* evento_, Window* window_) -> void
    {
        evento = evento_;
        window = window_;

        accionAnterior = ACCION::Nada;
        //Primer caracter p: pulsar s: soltar
        //Segundo caracter I: raton izquierda. M: raton medio. D: raton derecha. U: raton arriba. J: raton abajo E: espacio. R: enter. B: borrar. T: tab. C: ctrl. S: shift. A: alt. X: escape. Cualquier caracter en minusculas: su tecla en el teclado
        acciones.insert(std::make_pair("pI", ACCION::InteractuarAbajo));
        acciones.insert(std::make_pair("sI", ACCION::InteractuarArriba));
        acciones.insert(std::make_pair("pD", ACCION::MoverAbajo));
        acciones.insert(std::make_pair("sD", ACCION::MoverArriba));
        acciones.insert(std::make_pair("pE", ACCION::ConexionAbajo));
        acciones.insert(std::make_pair("sE", ACCION::ConexionArriba));
        acciones.insert(std::make_pair("pM", ACCION::ConexionAbajo));
        acciones.insert(std::make_pair("sM", ACCION::ConexionArriba));
        acciones.insert(std::make_pair("pB", ACCION::AlternarBorrando_));
        acciones.insert(std::make_pair("sB", ACCION::AlternarBorrando));
        acciones.insert(std::make_pair("sb", ACCION::BorrarSueltos));
        acciones.insert(std::make_pair("s1", ACCION::CrearAnd));
        acciones.insert(std::make_pair("s2", ACCION::CrearOr));
        acciones.insert(std::make_pair("s3", ACCION::CrearXor));
        acciones.insert(std::make_pair("s4", ACCION::CrearInterruptor));
        acciones.insert(std::make_pair("s5", ACCION::CrearBoton));
        acciones.insert(std::make_pair("s6", ACCION::CrearSalida));
        acciones.insert(std::make_pair("s7", ACCION::CrearTemporizador));
        acciones.insert(std::make_pair("s8", ACCION::CrearCondensador));
        acciones.insert(std::make_pair("sR", ACCION::SimularPaso));
        acciones.insert(std::make_pair("sq", ACCION::SimularEmpezarParar));
        acciones.insert(std::make_pair("sw", ACCION::SimularAcelerar));
        acciones.insert(std::make_pair("se", ACCION::SimularDecelerar));
        acciones.insert(std::make_pair("pX", ACCION::Cerrar));
        acciones.insert(std::make_pair("sg", ACCION::Guardar));
        acciones.insert(std::make_pair("sc", ACCION::Cargar));
        acciones.insert(std::make_pair("st", ACCION::Cuadricula));
        acciones.insert(std::make_pair("sy", ACCION::CuadriculaAgrandar));
        acciones.insert(std::make_pair("su", ACCION::CuadriculaDisminuir));
        acciones.insert(std::make_pair("sm", ACCION::MostrarControles));
        acciones.insert(std::make_pair(".U", ACCION::TemporizadorSubir));
        acciones.insert(std::make_pair(".J", ACCION::TemporizadorBajar));
        acciones.insert(std::make_pair("sz", ACCION::CamaraCentrar));
        acciones.insert(std::make_pair("sd", ACCION::DuplicarSeleccion));
        acciones.insert(std::make_pair("sp", ACCION::CambiarIdioma));
        acciones.insert(std::make_pair("sa", ACCION::AlternarOculto));
        acciones.insert(std::make_pair("sr", ACCION::Rotar));
    }

    static auto getNuevaAccion(const SDL_Event* evento) -> ACCION;
    [[nodiscard]] static auto getUltimaAccion() -> ACCION { return accionAnterior; }
    [[nodiscard]] static auto getEvent() -> SDL_Event* { return evento; }

    static auto setTextInput() -> void
    {
        escribiendo = true;
        SDL_StartTextInput();
    }

    static auto unsetTextInput() -> void
    {
        escribiendo = false;
        SDL_StopTextInput();
    }
};


#endif //CONTROLES_H
