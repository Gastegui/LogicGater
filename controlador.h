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
    TXT* txt;

    IO* origen{nullptr};

    bool cuadricula{false};
    int cuadriculaTamaño{50};

    std::map<unsigned int, Simulable*> simulables;

    ListaLineas listaLineas;
    std::vector<Simulable*>* seleccionados{};

public:
    explicit Controlador(Window* window_, TXT* txt_)
        : window{window_}, renderer{window_->getRenderer()}, txt{txt_} {}

    ~Controlador()
    {
        for(const auto& value : simulables | std::views::values)
            // ReSharper disable once CppDFADeletedPointer
            delete value;
        simulables.clear();
    }

    [[nodiscard]] auto getListaIMG(const ListaIMG::Altura altura) const -> ListaIMG::Lista*
    {
        return window->getListaIMG(altura);
    }

    [[nodiscard]] auto getWindow() const -> Window* { return window; }

    [[nodiscard]] auto getSimulables() const -> std::map<unsigned int, Simulable*> { return simulables; }
    [[nodiscard]] auto getListaLineas() const -> const ListaLineas* { return &listaLineas; }

    //Crea una puerta
    void crear(Puerta::Tipo tipo, int x, int y, bool arribaNegado = false, bool abajoNegado = false, bool salidaNegada = false, unsigned int id = 0);

    //Crea una entrada
    void crear(bool mantener, int x, int y, unsigned int id = 0);

    //Crea una salida
    void crear(int x, int y, unsigned int id = 0);

    //Crea un temporizador
    void crear(int x, int y, int tiempo, bool entradaNegada = false, bool salidaNegada = false, unsigned int id = 0);


    void borrar(Simulable* simulable)
    {
        if(!simulables.contains(simulable->getId()))
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

    void marcarOrigen(IO* origen_) { origen = origen_; }
    [[nodiscard]] auto getOrigen() const -> IO* { return origen; }

    void desmarcarOrigen() { origen = nullptr; }
    [[nodiscard]] auto getConectando() const -> bool { return origen != nullptr; }


    auto limpiar() -> int;

    void simular() const;
    void simularInstantaneo() const;

    [[nodiscard]] auto getSimulable(unsigned int id) const -> Simulable*;

    void alternarCuadricula() { cuadricula = !cuadricula; }

    auto cambiarCuadriculaRel(const int cambio) -> int
    {
        if(cuadriculaTamaño + cambio > 0)
            cuadriculaTamaño += cambio;
        return cuadriculaTamaño;
    }

    [[nodiscard]] auto getCuadriculaActiva() const -> bool { return cuadricula; }
    [[nodiscard]] auto getCuadriculaTamaño() const -> int { return cuadriculaTamaño; }
    [[nodiscard]] auto getSimulablesLenght() const -> unsigned long { return simulables.size(); }

    void setSeleccionados(std::vector<Simulable*>* seleccionados_) { seleccionados = seleccionados_; }
    void seleccionar(SDL_Rect region, std::vector<Simulable*>* vector) const;
    void desseleccionar(std::vector<Simulable*>* vector) const;
};

#endif //CONTROLADOR_H
