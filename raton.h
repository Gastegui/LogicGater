//
// Created by julen on 23/08/24.
//

#ifndef RATON_H
#define RATON_H
#include "ListaIMG.h"


class IMG;
class Controlador;
class Window;

class Raton
{
    int posX{0};
    int posY{0};
    IMG* imgAnterior{nullptr};

    Controlador* controlador{nullptr};
    Window* window{nullptr};

    bool borrando{false};
    bool moviendoPantalla{false};
    IMG* moviendoImg{nullptr};

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
    IMG* buscarLista(const ListaIMG::Lista* lista, int posX, int posY) const;

public:

    void setControlador(Controlador* controlador_);

    void setBorrando(bool borrando_);
    [[nodiscard]] bool getBorrando() const { return borrando; }

    void manejarRaton();
    void setMoviendoImg(IMG* img) { moviendoImg = img; }
};



#endif //RATON_H
