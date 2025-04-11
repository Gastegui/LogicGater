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
        Simulable* origen;
        Simulable* destino;
        IO* io;
        Lista* siguiente;
    };

private:
    Lista* lista;

public:
    ListaLineas()
        :lista{}
    {}

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

    [[nodiscard]] Lista* getLista() const { return lista; }

    void añadir(Simulable* origen, Simulable* destino, IO* io);

    void borrar(const Simulable* origen, const Simulable* destino, const IO* io);
    void borrar(const IO* involucrado);
    void borrar(const Simulable* involucrado);
};



#endif //LISTALINEAS_H
