//
// Created by julen on 3/11/24.
//

#ifndef LISTAIMG_H
#define LISTAIMG_H

class IMG;

class ListaIMG
{
public:
    struct Lista
    {
        IMG* img;
        Lista* siguiente;
    };
private:
    Lista* frente{nullptr};
    Lista* medio{nullptr};
    Lista* fondo{nullptr};

public:
    enum Altura
    {
        INVALIDO,
        FRENTE,
        MEDIO,
        FONDO
    };

    ~ListaIMG()
    {
        while (frente != nullptr)
        {
            const Lista* aux = frente;
            frente = frente->siguiente;
            delete aux;
        }

        while (medio != nullptr)
        {
            const Lista* aux = medio;
            medio = medio->siguiente;
            delete aux;
        }

        while (fondo != nullptr)
        {
            const Lista* aux = fondo;
            fondo = fondo->siguiente;
            delete aux;
        }
    }

    [[nodiscard]] Lista* getLista(Altura altura) const;

    bool añadir(IMG* img, Altura altura);
    bool quitar(const IMG* img, Altura altura);
    bool quitar(unsigned int id, Altura altura);
};



#endif //LISTAIMG_H
