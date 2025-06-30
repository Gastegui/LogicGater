//
// Created by julen on 24/08/24.
//

#ifndef IO_H
#define IO_H
#include "enums.h"
#include "Simulables/simulable.h"

class Puerta;
class Entrada;
class Salida;
class Temporizador;

class IO
{
    static auto idGenerator() -> unsigned int
    {
        static unsigned int id = 1 + getOffset(ID_TIPOS::IO);
        return id++;
    }

    unsigned int id;
    bool valor{false};

    int conexiones{0};

    std::pair<int, int> origen;

    Simulable* simulable;
    TIPOS_SIMULABLES tipoSimulable;
    unsigned int idSimulable;

public:
    explicit IO(Simulable* simulable_, const int posX, const int posY)
        : id{idGenerator()}, origen{posX, posY}, simulable(simulable_), tipoSimulable(simulable_->getTipo()), idSimulable(simulable_->getId()) {}

    auto set(const bool b) -> void { valor = b; }
    [[nodiscard]] auto get() const -> bool { return valor; }
    [[nodiscard]] auto getId() const -> unsigned int { return id; }

    [[nodiscard]] auto getTipoSimulable() const -> TIPOS_SIMULABLES { return tipoSimulable; }
    [[nodiscard]] auto getSimulable() const -> Simulable* { return simulable; }
    [[nodiscard]] auto getIdSimulable() const -> unsigned int { return idSimulable; }

    [[nodiscard]] auto getLineaOrigen() -> std::pair<int, int>* { return &origen; }

    auto moverLineaOrigenRel(const int x, const int y) -> void
    {
        origen.first += x;
        origen.second += y;
    }

    auto conectado() -> void { conexiones++; }
    auto desconectado() -> void { conexiones--; }
    [[nodiscard]] auto getConexiones() const -> int { return conexiones; }

    explicit operator bool() const
    {
        return valor;
    }

    auto operator=(const bool a) -> IO&
    {
        valor = a;
        return *this;
    }

    auto operator==(const IO& a) const -> bool
    {
        return this->getId() == a.getId();
    }
};

#endif //IO_H
