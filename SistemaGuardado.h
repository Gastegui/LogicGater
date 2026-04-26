//
// Created by julen on 3/01/25.
//

#ifndef SISTEMAGUARDADO_H
#define SISTEMAGUARDADO_H
#include <string>

class Controlador;
class Window;

class SistemaGuardado
{
public:
    struct Valores
    {
        int velocidadSimulacion = 50;
        int cuadriculaTamaño = 50;
    };

    static auto guardar(const Valores* valores, const std::string& archivo) -> bool;

    static auto cargar(Valores* valores, const std::string& archivo) -> int;
};


#endif //SISTEMAGUARDADO_H
