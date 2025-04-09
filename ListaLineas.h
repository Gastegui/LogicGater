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
        Simulable* simulable_origen;
        Simulable* simulable_destino;
        IO* io_origen; //TODO: en realidad estos dos son el mismo IO
        IO* io_destino; //ya que uno es la salida y el otro es un puntero a la salida (la entrada)
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

    void añadir(Simulable* origen, Simulable* destino, IO* io_origen, IO* io_destino);

    void borrar(const Simulable* origen, const Simulable* destino, const IO* io_origen, const IO* io_destino);
    void borrar(const IO* involucrado);
    void borrar(const Simulable* involucrado);
};



#endif //LISTALINEAS_H
