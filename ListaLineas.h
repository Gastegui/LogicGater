//
// Created by julen on 9/04/25.
//

#ifndef LISTALINEAS_H
#define LISTALINEAS_H


class IO;
class Simulable;

class ListaLineas
{
public:
    struct Lista
    {
        Simulable* destino;
        IO* io;
        int conexion;
        Lista* siguiente;
    };

private:
    Lista* lista{};

public:
    ListaLineas() = default;

    ~ListaLineas()
    {
        const Lista* tmp{lista};
        while(lista != nullptr)
        {
            lista = lista->siguiente;
            delete tmp;
            tmp = lista;
        }
    }

    [[nodiscard]] auto getLista() const -> Lista* { return lista; }

    void añadir(Simulable* destino, IO* io, int conexion);

    void borrar(const Simulable* destino, const IO* io, int conexion);
    void borrar(const IO* involucrado);
    void borrar(const Simulable* involucrado);
};



#endif //LISTALINEAS_H
