//
// Created by julen on 23/08/24.
//

#ifndef RATON_H
#define RATON_H
#include "ListaIMG.h"


class IMG;
class Controlador;

class Raton
{
    int posX{0};
    int posY{0};
    IMG* imgAnterior{nullptr};

    Controlador* controlador{nullptr};

    bool borrando{false};

public:
    enum Evento
    {
        NADA,
        ABAJO,
        ARRIBA,
        ENTRAR,
        SALIR,
        MOVIMIENTO
    };

    enum Boton
    {
        NINGUNO,
        IZQUIERDO,
        DERECHO,
        RUEDA
    };

private:

    void interactuar(IMG* actual);
    bool interactuarConexion(IMG* actual);

public:

    void setControlador(Controlador* controlador_) { controlador = controlador_; }

    void setBorrando(bool borrando_);
    [[nodiscard]] bool getBorrando() const { return borrando; }

    void manejarRaton();
    bool añadir(IMG* img, int altura_, bool final);
    bool borrar(const IMG* img, int altura_);
    bool borrarTodo(int altura_);
};



#endif //RATON_H
