//
// Created by julen on 23/08/24.
//

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <map>
#include <ranges>

#include "ListaLineas.h"
#include "Simulables/puerta.h"
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

    ListaLineas listaLineas{};

public:
    explicit Controlador(Window* window_)
        :window{window_}, renderer{window_->getRenderer()}
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

    [[nodiscard]] std::map<unsigned int, Simulable*> getSimulables() const { return simulables; }
    [[nodiscard]] const ListaLineas* getListaLineas() const { return &listaLineas; }

    //Crea una puerta
    void crear(Puerta::Tipo tipo, int x, int y, bool arribaNegado = false, bool abajoNegado = false, bool salidaNegada = false, unsigned int id = 0);

    //Crea una entrada
    void crear(bool mantener, int x, int y, unsigned int id = 0);

    //Crea una salida
    void crear(int x, int y, unsigned int id = 0);


    void borrar(Simulable* simulable)
    {
        if (!simulables.contains(simulable->getId()))
            return;
        borrarConexiones(simulable);

        window->borrar(simulable->getImg(), ListaIMG::MEDIO);
        simulables.erase(simulable->getId());

        delete simulable;
    }

    void añadirConexion(Simulable* simulable_destino, const int conexion) { listaLineas.añadir(simulable_destino, origen, conexion); }
    void borrarConexiones(const IO* io) { listaLineas.borrar(io); }
    void borrarConexiones(const Simulable* simulable) { listaLineas.borrar(simulable); }
    void borrarConexion(const Simulable* destino, const IO* io, const int conexion) { listaLineas.borrar(destino, io, conexion); }

    //Crea un temporizador
    void crear(int x, int y, int duracion, unsigned int id = 0);

    void marcarOrigen(IO* origen_) { origen = origen_; }
    [[nodiscard]] IO* getOrigen() const { return origen; }

    void desmarcarOrigen() { origen = nullptr; }
    [[nodiscard]] bool getConectando() const { return origen != nullptr; }


    int limpiar();

    void simular() const;
    void simularInstantaneo() const;

    [[nodiscard]] Simulable* getSimulable(unsigned int id) const;

    void alternarCuadricula() { cuadricula = !cuadricula; }
    int cambiarCuadriculaRel(const int cambio) { if(cuadriculaTamaño + cambio > 0) cuadriculaTamaño += cambio; return cuadriculaTamaño; }
    [[nodiscard]] int getCuadriculaActiva() const { return cuadricula; }
    [[nodiscard]] int getCuadriculaTamaño() const { return cuadriculaTamaño; }
    [[nodiscard]] unsigned long getSimulablesLenght() const {return simulables.size(); }
};

#endif //CONTROLADOR_H