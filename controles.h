//
// Created by julen on 26/09/24.
//

#ifndef CONTROLES_H
#define CONTROLES_H
#include <SDL_events.h>


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
public:
    static void init(SDL_Event* event)
    {
        evento = event;
        accionAnterior = Nada;
    }
    static Accion getNuevaAccion(const SDL_Event* evento);
    [[nodiscard]] static Accion getUltimaAccion() { return accionAnterior; }
    [[nodiscard]] static SDL_Event* getEvent() { return evento; }
};



#endif //CONTROLES_H
