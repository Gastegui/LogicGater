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
    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + 1<<28;
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
        :id{idGenerator()}, origen{posX, posY}, simulable(simulable_), tipoSimulable(simulable_->getTipo()), idSimulable(simulable_->getId())
    {}

    void set(const bool b) { valor = b; }
    [[nodiscard]] bool get() const { return valor; }
    [[nodiscard]] unsigned int getId() const { return id; }

    [[nodiscard]] TIPOS_SIMULABLES getTipoSimulable() const { return tipoSimulable; }
    [[nodiscard]] Simulable* getSimulable() const { return simulable; }
    [[nodiscard]] unsigned int getIdSimulable() const { return idSimulable; }

    [[nodiscard]] std::pair<int, int>* getLineaOrigen() { return &origen; }
    void moverLineaOrigenRel(const int x, const int y) { origen.first += x; origen.second += y; }

    void conectado() { conexiones++; }
    void desconectado() { conexiones--; }
    [[nodiscard]] int getConexiones() const { return conexiones; }

    explicit operator bool() const
    {
        return valor;
    }

    IO& operator=(const bool a)
    {
        valor = a;
        return *this;
    }

    bool operator==(const IO& a) const
    {
        return this->getId() == a.getId();
    }
};

#endif //IO_H
