//
// Created by julen on 23/08/24.
//

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <map>
#include <ranges>

#include "puerta.h"
#include "Wrapper/window.h"

class Puerta;
class Raton;
class Window;
class IO;
class Entrada;
class Salida;
class SistemaGuardado;

class Controlador
{
    friend class SistemaGuardado;


    Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};

    IO* origen{nullptr};

    bool cuadricula{false};
    int cuadriculaTamaño{50};

    std::map<unsigned int, Simulable*> simulables;

public:
    explicit Controlador(Window* window_)
        :window{window_} ,renderer{window_->getRenderer()}
    {}

    ~Controlador()
    {
        for (const auto& value : simulables | std::views::values)
            // ReSharper disable once CppDFADeletedPointer
            delete value;
        simulables.clear();
    }

    [[nodiscard]] ListaIMG::Lista* getListaIMG(const ListaIMG::Altura altura) const
    {
        return window->getListaIMG(altura);
    }
    [[nodiscard]] Window* getWindow() const { return window; }

    //Crea una puerta
    void crear(Puerta::Tipo tipo, int x, int y, bool arribaNegado = false, bool abajoNegado = false, bool salidaNegada = false, unsigned int id = 0);
    //Borra una puerta
    bool borrar(Puerta* puerta);
    //Borra las conexiones deseadas de una puerta. (Todas por defenco)
    void borrarConexiones(Puerta* puerta, bool arriba = true, bool abajo = true, bool salida = true);

    //Crea una entrada
    void crear(bool mantener, int x, int y, unsigned int id = 0);
    //Borra una entrada
    bool borrar(Entrada* entrada);
    //Borra todas las conexiones de una entrada
    void borrarConexiones(Entrada* entrada) const;

    //Crea una salida
    void crear(int x, int y, unsigned int id = 0);
    //Borra una salida
    bool borrar(Salida* salida);
    //Bora todas las conexiones de una salida
    void borrarConexiones(Salida* salida) const;

    void marcarOrigen(Puerta* puerta);
    void marcarOrigen(Entrada* boton);
    void desmarcarOrigen();
    [[nodiscard]] bool getConectando() const { return origen != nullptr; }

    bool destino(Puerta* puerta, bool arriba);
    bool destino(Salida* salida);

    int limpiar();

    void simular() const;
    void simularInstantaneo() const;

    [[nodiscard]] Puerta* getPuerta(unsigned int id) const;
    [[nodiscard]] Entrada* getEntrada(unsigned int id) const;
    [[nodiscard]] Salida* getSalida(unsigned int id) const;

    void alternarCuadricula() { cuadricula = !cuadricula; }
    int cambiarCuadriculaRel(const int cambio) { if(cuadriculaTamaño + cambio > 0) cuadriculaTamaño += cambio; return cuadriculaTamaño; }
    [[nodiscard]] int getCuadriculaActiva() const { return cuadricula; }
    [[nodiscard]] int getCuadriculaTamaño() const { return cuadriculaTamaño; }

};

#endif //CONTROLADOR_H