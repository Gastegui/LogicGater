//
// Created by julen on 23/08/24.
//

#ifndef RATON_H
#define RATON_H

#include <SDL2/SDL.h>

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

    struct lista_t
    {
        IMG* img;
        lista_t* siguiente;
    };

    enum Altura
    {
        INVALIDO,
        FONDO,
        MEDIO,
        FRENTE
    };

private:
    lista_t* frente{nullptr};
    lista_t* medio{nullptr};
    lista_t* fondo{nullptr};

    void interactuar(IMG* actual, const SDL_Event* evento);
    bool interactuarConexion(IMG* actual, const SDL_Event* evento);

public:
    ~Raton()
    {
        lista_t* tmp;
        if(frente != nullptr)
        {
            while(frente != nullptr)
            {
                tmp = frente;
                frente = frente->siguiente;
                delete tmp;
            }
        }
        if(medio != nullptr)
        {
            while(medio != nullptr)
            {
                tmp = medio;
                medio = medio->siguiente;
                delete tmp;
            }
        }
        if(fondo != nullptr)
        {
            while(fondo != nullptr)
            {
                tmp = fondo;
                fondo = fondo->siguiente;
                delete tmp;
            }
        }
    }

    void setControlador(Controlador* controlador_) { controlador = controlador_; }

    void setBorrando(bool borrando_);
    [[nodiscard]] bool getBorrando() const { return borrando; }

    void manejarRaton(const SDL_Event* evento);
    bool añadir(IMG* img, int altura_, bool final);
    bool borrar(const IMG* img, int altura_);
    bool borrarTodo(int altura_);
};



#endif //RATON_H
