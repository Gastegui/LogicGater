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
private:
    std::vector<IMG*> frente;
    std::vector<IMG*> medio;
    std::vector<IMG*> fondo;

public:
    enum Altura : uint8_t
    {
        INVALIDO,
        FRENTE,
        MEDIO,
        FONDO
    };

    [[nodiscard]] auto getLista(Altura altura) const -> const std::vector<IMG*>*;

    auto añadir(IMG* img, Altura altura) -> bool;
    auto quitar(const IMG* img, Altura altura) -> bool;
    auto quitar(unsigned int id, Altura altura) -> bool;
};


#endif //LISTAIMG_H
