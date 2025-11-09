//
// Created by julen on 9/10/25.
//

#ifndef LOGICGATER_IDIOMAS_H
#define LOGICGATER_IDIOMAS_H
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <utility>

#include "enums.h"


class TXT;
class Texto
{
    int x;
    int y;
    std::string es;
    std::string en;
    std::string eu;

public:
    Texto(const int x_, const int y_, std::string es_, std::string en_, std::string eu_)
        : x{x_}, y{y_}, es{std::move(es_)}, en{std::move(en_)}, eu{std::move(eu_)}
    {}

    [[nodiscard ]] auto getX() const -> int { return x; }
    [[nodiscard ]] auto getY() const -> int { return y; }
    [[nodiscard ]] auto getTexto(IDIOMA idioma) const -> const char*;
    [[nodiscard ]] auto getTextoStr(IDIOMA idioma) const -> std::string;

    auto operator==(const Texto& other) const -> bool
    {
        return es == other.getTexto(IDIOMA::ES) && x == other.getX() && y == other.getY();
    }
};

class Idiomas
{
    TXT& txt;
    std::map<ID_TEXTO, Texto> textos;
    IDIOMA idioma;

    auto setPos(int x, int y) const -> void;

public:
    Idiomas(TXT& txt_, IDIOMA idioma_)
        :txt{txt_}, idioma{idioma_}
    {
        std::ifstream inf{"./textos.csv"};

        if(!inf)
        {
            std::println(std::cout, "No se ha podido crear el file stream de los textos");
            return;
        }

        std::string input{};
        std::string tmp{};
        std::getline(inf, input);
        while(std::getline(inf, input))
        {
            std::stringstream ss{input};
            int i{0};
            int id{0};
            int x{0};
            int y{0};
            std::string es;
            std::string en;
            std::string eu;
            while(std::getline(ss, tmp, ','))
            {
                switch(i++)
                {
                    case 0:
                        id = std::stoi(tmp);
                        break;
                    case 1:
                        x = std::stoi(tmp);
                        break;
                    case 2:
                        y = std::stoi(tmp);
                        break;
                    case 3:
                        es = tmp;
                        break;
                    case 4:
                        en = tmp;
                        break;
                    case 5:
                        eu = tmp;
                        break;
                    default:
                        std::println(std::cout, "Entrada de más en textos.csv \"%s\"", tmp);
                }
            }
            Texto texto{x, y, es, en, eu};
            if(static_cast<ID_TEXTO>(id) == ID_TEXTO::NADA || static_cast<ID_TEXTO>(id) == ID_TEXTO::Vacio)
                texto = Texto{x, y, " ", " ", " "};
            textos.insert_or_assign(static_cast<ID_TEXTO>(id), std::move(texto));
        }
    }

    auto print(ID_TEXTO id) const -> void;
    auto print(ID_TEXTO id, std::string param) const -> void;
    auto print(ID_TEXTO id, int param) const -> void;

    auto cambiar() -> void;
};


#endif // LOGICGATER_IDIOMAS_H
