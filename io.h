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
        static unsigned int id = 1 + get_offset(ID_TIPOS::IO);
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

    void set(const bool b) { valor = b; }
    [[nodiscard]] auto get() const -> bool { return valor; }
    [[nodiscard]] auto getId() const -> unsigned int { return id; }

    [[nodiscard]] auto getTipoSimulable() const -> TIPOS_SIMULABLES { return tipoSimulable; }
    [[nodiscard]] auto getSimulable() const -> Simulable* { return simulable; }
    [[nodiscard]] auto getIdSimulable() const -> unsigned int { return idSimulable; }

    [[nodiscard]] auto getLineaOrigen() -> std::pair<int, int>* { return &origen; }

    void moverLineaOrigenRel(const int x, const int y)
    {
        origen.first += x;
        origen.second += y;
    }

    void conectado() { conexiones++; }
    void desconectado() { conexiones--; }
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
