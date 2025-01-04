//
// Created by julen on 3/01/25.
//

#ifndef SISTEMAGUARDADO_H
#define SISTEMAGUARDADO_H

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

    static bool guardar(const Controlador* controlador, const Window* window, Valores* valores);

    static int cargar(Controlador* controlador, Window* window, Valores* valores);
};



#endif //SISTEMAGUARDADO_H
