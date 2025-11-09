//
// Created by julen on 9/10/25.
//

#include "Idiomas.h"

#include "Wrapper/text.h"

auto Texto::getTexto(IDIOMA idioma) const -> const char *
{
    switch(idioma)
    {
        case IDIOMA::ES:
            return es.c_str();
        case IDIOMA::EN:
            return en.c_str();
        case IDIOMA::EU:
            return eu.c_str();
        default:
            std::println(std::cout, "IDIOMA desconocido (%d). Texto: \"%s\"", static_cast<int>(idioma), es);
            return es.c_str();
    }
}
auto Texto::getTextoStr(IDIOMA idioma) const -> std::string
{
    switch(idioma)
    {
        case IDIOMA::ES:
            return es;
        case IDIOMA::EN:
            return en;
        case IDIOMA::EU:
            return eu;
        default:
            std::println(std::cout, "IDIOMA desconocido (%d). Texto: \"%s\"", static_cast<int>(idioma), es);
            return es;
    }
}
auto Idiomas::setPos(const int x, const int y) const -> void
{
    if(x != -1)
        txt.setX(x);
    if(y != -1)
        txt.setY(y);
}

auto Idiomas::print(const ID_TEXTO id) const -> void
{
    const Texto &texto = textos.at(id);
    setPos(texto.getX(), texto.getY());
    txt.write(texto.getTexto(idioma));
}

auto Idiomas::print(const ID_TEXTO id, std::string param) const -> void
{
    const Texto &texto = textos.at(id);
    setPos(texto.getX(), texto.getY());

    txt.write(std::vformat(texto.getTextoStr(idioma) , std::make_format_args(param)).c_str());
}

auto Idiomas::print(const ID_TEXTO id, int param) const -> void
{
    const Texto &texto = textos.at(id);
    setPos(texto.getX(), texto.getY());

    txt.write(std::vformat(texto.getTextoStr(idioma) , std::make_format_args(param)).c_str());
}
auto Idiomas::cambiar() -> void
{
    switch(idioma)
    {
        case IDIOMA::ES:
            idioma = IDIOMA::EU;
            break;
        case IDIOMA::EN:
            idioma = IDIOMA::ES;
            break;
        case IDIOMA::EU:
            idioma = IDIOMA::EN;
            break;
    }
}
