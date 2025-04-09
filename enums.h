//
// Created by julen on 26/03/25.
//

#ifndef ENUMS_H
#define ENUMS_H
#include <string>

#define ACCIONES \
        Nada = 0,           \
        TextInput,          /*SDL_TEXTINPUT*/ \
        InteractuarArriba,  /*Soltar clic izquierdo    (sirve para encender/apagar, invertir polaridad, borrar) */ \
        InteractuarAbajo,   /*Pulsar click izquierdo */ \
        MovimientoRaton,    /*SDL_MOUSEMOTION */ \
        MoverAbajo,         /*Pulsar clic derecho */ \
        MoverArriba,        /*Soltar clic derecho */ \
        ConexionAbajo,      /*Pulsar clic central o espacio */ \
        ConexionArriba,     /*Soltar clic central o espacio */ \
        AlternarBorrando,   /*Backspace */ \
        BorrarSueltos,      /*L */ \
        CrearAnd,           /*A */ \
        CrearOr,            /*O */ \
        CrearXor,           /*X */ \
        CrearInterruptor,   /*I */ \
        CrearBoton,         /*B */ \
        CrearSalida,        /*S */ \
        SimularPaso,        /*Enter */ \
        SimularEmpezar,     /*Q */ \
        SimularParar,       /*W */ \
        SimularAcelerar,    /*- */ \
        SimularDecelerar,   /*+ */ \
        Cerrar,             /*Escape */ \
        Guardar,            /*G */ \
        Cargar,             /*C */ \
        Cuadricula,         /*E */ \
        CuadriculaAgrandar, /*R */ \
        CuadriculaDisminuir,/*F */ \
        MostrarControles,   /*M */ \
        TemporizadorSubir,  /*Rueda arriba */ \
        TemporizadorBajar,  /*/Rueda abajo */

enum class ACCION
{
    ACCIONES
};

enum class TIPOS_SIMULABLES
{
    Nada,
    Puerta,
    Salida,
    Entrada,
    Temporizador
};

constexpr static std::string toString(const TIPOS_SIMULABLES tipo)
{
    switch (tipo) {
        case TIPOS_SIMULABLES::Nada:
            return "NADA";
        case TIPOS_SIMULABLES::Puerta:
            return "Puerta";
        case TIPOS_SIMULABLES::Salida:
            return "Salida";
        case TIPOS_SIMULABLES::Entrada:
            return "Entrada";
        case TIPOS_SIMULABLES::Temporizador:
            return "Temporizador";
        default:
            return "Desconocido";
    }
}

enum class TIPOS
{
    Nada,
    Puerta,
    Salida,
    Entrada,
    IO,
    IMG,
    Temporizador
};

constexpr static std::string toString(const TIPOS tipo)
{
    switch (tipo)
    {
        case TIPOS::Nada:
            return "NADA";
        case TIPOS::Puerta:
            return "Puerta";
        case TIPOS::Salida:
            return "Salida";
        case TIPOS::Entrada:
            return "Entrada";
        case TIPOS::Temporizador:
            return "Temporizador";
        case TIPOS::IO:
            return "IO";
        case TIPOS::IMG:
            return "IMG";
        default:
            return "Desconocido";
    }
}

enum class ID_TIPOS : unsigned int
{
    Nada = 0,
    Puerta = static_cast<unsigned int>(1 << 31),
    Entrada = static_cast<unsigned int>(1 << 30),
    Salida = static_cast<unsigned int>(1 << 29),
    IO = static_cast<unsigned int>(1 << 28),
    IMG = static_cast<unsigned int>(1 << 27),
    Temporizador = static_cast<unsigned int>(1 << 26)
};

enum class INTERACCIONES
{
    ACCIONES
    ConexionBorrar,
    RatonEntrar, //Solo hacen falta entrar y salir, para poder gestionar cosas como hover o así. Arriba y abajo NO
    RatonSalir,
};

#endif //ENUMS_H
