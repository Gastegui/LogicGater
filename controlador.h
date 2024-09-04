//
// Created by julen on 23/08/24.
//

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include "puerta.h"
#include "entrada.h"
#include "salida.h"
#include "io.h"
#include "Wrapper/window.h"

class Puerta;
class Raton;
class Window;
class IO;
class Entrada;
class Salida;

class Controlador
{
    struct ListaPuertas
    {
        Puerta* puerta{nullptr};
        ListaPuertas* siguiente{nullptr};
    };

    struct ListaEntradas
    {
        Entrada* entrada{nullptr};
        ListaEntradas* siguiente{nullptr};
    };

    struct ListaSalidas
    {
        Salida* salida{nullptr};
        ListaSalidas* siguiente{nullptr};
    };

    Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};

    IO* origen{nullptr};

    ListaPuertas* listaPuertas{nullptr};
    ListaEntradas* listaEntradas{nullptr};
    ListaSalidas* listaSalidas{nullptr};

public:
    explicit Controlador(Window* window_)
        :window{window_} ,renderer{window_->getRenderer()}
    {}

    ~Controlador()
    {
        if(listaPuertas != nullptr)
        {
            // ReSharper disable once CppJoinDeclarationAndAssignment
            const ListaPuertas* tmp;
            while(listaPuertas != nullptr)
            {
                tmp = listaPuertas;
                listaPuertas = listaPuertas->siguiente;
                delete tmp->puerta;
                delete tmp;
            }
        }
        if(listaEntradas != nullptr)
        {
            // ReSharper disable once CppJoinDeclarationAndAssignment
            const ListaEntradas* tmp;
            while(listaEntradas != nullptr)
            {
                tmp = listaEntradas;
                listaEntradas = listaEntradas->siguiente;
                delete tmp->entrada;
                delete tmp;
            }
        }
        if(listaSalidas != nullptr)
        {
            // ReSharper disable once CppJoinDeclarationAndAssignment
            const ListaSalidas* tmp;
            while(listaSalidas != nullptr)
            {
                tmp = listaSalidas;
                listaSalidas = listaSalidas->siguiente;
                delete tmp->salida;
                delete tmp;
            }
        }
    }

    //Crea una puerta
    void crear(Puerta::Tipo tipo, int x, int y, bool arribaNegado = false, bool abajoNegado = false, bool salidaNegada = false);
    //Borra una puerta
    bool borrar(Puerta* puerta);
    //Borra las conexiones deseadas de una puerta. (Todas por defenco)
    void borrarConexiones(Puerta* puerta, bool arriba = true, bool abajo = true, bool salida = true);

    //Crea una entrada
    void crear(bool mantener, int x, int y);
    //Borra una entrada
    bool borrar(Entrada* entrada);
    //Borra todas las conexiones de una entrada
    void borrarConexiones(Entrada* entrada) const;

    //Crea una salida
    void crear(int x, int y);
    //Borra una salida
    bool borrar(Salida* salida);
    //Bora todas las conexiones de una salida
    void borrarConexiones(Salida* salida) const;

    void marcarOrigen(Puerta* puerta);
    void marcarOrigen(Entrada* boton);
    void desmarcarOrigen();
    [[nodiscard]] bool getConectando() const { return origen != nullptr; }

    void destino(Puerta* puerta, bool arriba);
    void destino(Salida* salida);

    void simular() const;

};

#endif //CONTROLADOR_H