//
// Created by julen on 9/04/25.
//

#ifndef LISTALINEAS_H
#define LISTALINEAS_H
#include <vector>


class IO;
class Simulable;

class ListaLineas
{
public:
    struct Linea
    {
        Simulable* destino;
        IO* io;
        int conexion;
        bool oculta;
    };

private:
    std::vector<Linea> lista;

public:
    ListaLineas() = default;


    [[nodiscard]] auto getLista() -> std::vector<Linea>* { return &lista; }

    auto añadir(Simulable* destino, IO* io, int conexion, bool oculta) -> void;

    auto borrar(const Simulable* destino, const IO* io, int conexion) -> void;
    auto borrar(const IO* involucrado) -> void;
    auto borrar(const Simulable* involucrado) -> void;
};


#endif //LISTALINEAS_H
