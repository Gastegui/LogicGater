//
// Created by julen on 23/08/24.
//

#ifndef CONTROLADOR_H
#define CONTROLADOR_H

#include <map>
#include <memory>

#include "ListaLineas.h"
#include "Simulables/puerta.h"
#include "Wrapper/window.h"

class Puerta;
class Raton;
class Window;
class IO;
class Entrada;
class Salida;

class Controlador
{
    friend class SistemaGuardado;


    Window* window{nullptr};
    SDL_Renderer* renderer{nullptr};
    TXT* txt;

    IO* origen{nullptr};

    bool cuadricula{false};
    int cuadriculaTamaño{50};

    std::map<unsigned int, std::unique_ptr<Simulable>> simulables;

    ListaLineas listaLineas;
    std::vector<Simulable*>* seleccionados{};

    bool conexionesOcultas{false};

public:
    explicit Controlador(Window* window_, TXT* txt_)
        : window{window_}, renderer{window_->getRenderer()}, txt{txt_} {}

    ~Controlador() = default;
    Controlador(const Controlador&) = delete;
    auto operator=(const Controlador&) -> Controlador& = delete;
    Controlador(Controlador&&) = delete;
    auto operator=(Controlador&&) -> Controlador& = delete;

    [[nodiscard]] auto getListaIMG(const ListaIMG::ALTURA altura) const -> const std::vector<IMG*>*
    {
        return window->getListaIMG(altura);
    }

    [[nodiscard]] auto getWindow() const -> Window* { return window; }
    [[nodiscard]] auto getListaLineas() -> ListaLineas* { return &listaLineas; }

    //Crea una puerta
    auto crear(Puerta::TIPO tipo, int x, int y, bool arribaNegado = false, bool abajoNegado = false, bool salidaNegada = false, unsigned int id = 0) -> unsigned int;

    //Crea una entrada
    auto crear(bool mantener, int x, int y, unsigned int id = 0) -> unsigned int;

    //Crea una salida
    auto crear(int x, int y, unsigned int id = 0) -> unsigned int;

    //Crea un temporizador
    auto crear(int x, int y, int tiempo, bool entradaNegada = false, bool salidaNegada = false, unsigned int id = 0) -> unsigned int;

    //Crea un condensador
    auto crear(int x, int y, Puerta::TIPO tipo, unsigned int id = 0) -> unsigned int;

    auto borrar(Simulable* simulable) -> void
    {
        if(!simulables.contains(simulable->getId()))
            return;
        borrarConexiones(simulable);

        window->borrar(simulable->getImg(), ListaIMG::Medio);
        simulables.erase(simulable->getId());

        //delete simulable;
    }

    auto añadirConexion(Simulable* simulableDestino, const int conexion) -> void { listaLineas.añadir(simulableDestino, origen, conexion, conexionesOcultas); }
    auto borrarConexiones(const IO* io) -> void { listaLineas.borrar(io); }
    auto borrarConexiones(const Simulable* simulable) -> void { listaLineas.borrar(simulable); }
    auto borrarConexion(const Simulable* destino, const IO* io, const int conexion) -> void { listaLineas.borrar(destino, io, conexion); }

    auto marcarOrigen(IO* origen_) -> void { origen = origen_; }
    [[nodiscard]] auto getOrigen() const -> IO* { return origen; }

    auto desmarcarOrigen() -> void { origen = nullptr; }
    [[nodiscard]] auto getConectando() const -> bool { return origen != nullptr; }


    auto limpiar() -> int;

    auto simular() const -> void;
    auto simularInstantaneo() const -> void;

    [[nodiscard]] auto getSimulable(unsigned int id) const -> Simulable*;

    auto alternarCuadricula() -> void { cuadricula = !cuadricula; }

    auto cambiarCuadriculaRel(const int cambio) -> int
    {
        if(cuadriculaTamaño + cambio > 0)
            cuadriculaTamaño += cambio;
        return cuadriculaTamaño;
    }

    [[nodiscard]] auto getCuadriculaActiva() const -> bool { return cuadricula; }
    [[nodiscard]] auto getCuadriculaTamaño() const -> int { return cuadriculaTamaño; }
    [[nodiscard]] auto getSimulablesLenght() const -> unsigned long { return simulables.size(); }

    auto setSeleccionados(std::vector<Simulable*>* seleccionados_) -> void { seleccionados = seleccionados_; }
    auto seleccionar(SDL_Rect region) const -> void;
    auto desseleccionar() const -> void;

    auto duplicarSeleccion() -> void;

    [[nodiscard]] auto getConexionesOcultas() const -> bool { return conexionesOcultas; }
    auto alternarConexionesOcultas() -> void
    {
        conexionesOcultas = !conexionesOcultas;
    }

    [[nodiscard]] auto getCantidadSimulables() const -> int { return static_cast<int>(simulables.size()); }
};

#endif //CONTROLADOR_H
