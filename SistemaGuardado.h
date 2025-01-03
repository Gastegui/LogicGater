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
    static bool guardar(const Controlador* controlador, const Window* window, int velocidadSimulacion);

    static int cargar(Controlador* controlador, Window* window);
};



#endif //SISTEMAGUARDADO_H
