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
    };

private:
    std::vector<Linea> lista;

public:
    ListaLineas() = default;


    [[nodiscard]] auto getLista() -> std::vector<Linea>* { return &lista; }

    void añadir(Simulable* destino, IO* io, int conexion);

    void borrar(const Simulable* destino, const IO* io, int conexion);
    void borrar(const IO* involucrado);
    void borrar(const Simulable* involucrado);
};


#endif //LISTALINEAS_H
