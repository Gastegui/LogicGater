//
// Created by julen on 26/09/24.
//

#ifndef CONTROLES_H
#define CONTROLES_H
#include <map>
#include <SDL_events.h>
#include <string>


class Controles
{
public:

    enum Accion
    {
        Nada,               //
        Interactuar,        //Soltar clic izquierdo    (sirve para encender/apagar, invertir polaridad, borrar)
        InteractuarAbajo,   //Pulsar click izquierdo
        MovimientoRaton,    //SDL_MOUSEMOTION
        MoverAbajo,         //Pulsar clic derecho
        MoverArriba,        //Soltar clic derecho
        ConexionAbajo,      //Pulsar clic central o espacio
        ConexionArriba,     //Soltar clic central o espacio
        AlternarBorrando,   //Backspace
        BorrarSueltos,      //L
        CrearAnd,           //A
        CrearOr,            //O
        CrearXor,           //X
        CrearInterruptor,   //I
        CrearBoton,         //B
        CrearSalida,        //S
        SimularPaso,        //Enter
        SimularEmpezar,     //q
        SimularParar,       //w
        Cerrar              //Alt + F4
    };

private:
    static Accion accionAnterior;
    static SDL_Event* evento;
    static std::map<std::string, Accion> acciones;
    //Primer caracter p: pulsar s: soltar
    //Segundo caracter I: raton izquierda. M: raton medio. D: raton derecha. E: espacio. R: enter. B: borrar. T: tab. C: ctrl. S: shift. A: alt. X: escape. Cualquier caracter en minusculas: su tecla en el teclado
public:
    static void init(SDL_Event* event)
    {
        evento = event;
        accionAnterior = Nada;
        acciones.insert(std::make_pair("pI", InteractuarAbajo));
        acciones.insert(std::make_pair("sI", Interactuar));
        acciones.insert(std::make_pair("pD", MoverAbajo));
        acciones.insert(std::make_pair("sD", MoverArriba));
        acciones.insert(std::make_pair("pE", ConexionAbajo));
        acciones.insert(std::make_pair("sE", ConexionArriba));
        acciones.insert(std::make_pair("pM", ConexionAbajo));
        acciones.insert(std::make_pair("sM", ConexionArriba));
        acciones.insert(std::make_pair("sB", AlternarBorrando));
        acciones.insert(std::make_pair("sl", BorrarSueltos));
        acciones.insert(std::make_pair("sa", CrearAnd));
        acciones.insert(std::make_pair("so", CrearOr));
        acciones.insert(std::make_pair("sx", CrearXor));
        acciones.insert(std::make_pair("si", CrearInterruptor));
        acciones.insert(std::make_pair("sb", CrearBoton));
        acciones.insert(std::make_pair("ss", CrearSalida));
        acciones.insert(std::make_pair("sR", SimularPaso));
        acciones.insert(std::make_pair("sq", SimularEmpezar));
        acciones.insert(std::make_pair("sw", SimularParar));
    }
    static Accion getNuevaAccion(const SDL_Event* evento);
    [[nodiscard]] static Accion getUltimaAccion() { return accionAnterior; }
    [[nodiscard]] static SDL_Event* getEvent() { return evento; }
};



#endif //CONTROLES_H
