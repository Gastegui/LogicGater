//
// Created by julen on 3/11/24.
//

#ifndef LISTAIMG_H
#define LISTAIMG_H
#include <cstdint>
#include <vector>

class IMG;

class ListaIMG
{
    std::vector<IMG*> frente;
    std::vector<IMG*> medio;
    std::vector<IMG*> fondo;

public:
    enum ALTURA : uint8_t
    {
        Invalido,
        Frente,
        Medio,
        Fondo
    };

    [[nodiscard]] auto getLista(ALTURA altura) const -> const std::vector<IMG*>*;

    auto añadir(IMG* img, ALTURA altura) -> bool;
    auto quitar(const IMG* img, ALTURA altura) -> bool;
    auto quitar(unsigned int id, ALTURA altura) -> bool;
};


#endif //LISTAIMG_H
