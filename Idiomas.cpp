//
// Created by julen on 9/10/25.
//

#include "Idiomas.h"
#include <vector>

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
        TXT::get().setX(x);
    if(y != -1)
        TXT::get().setY(y);
}

auto Idiomas::printReal(std::string string) const -> void
{
    std::vector<std::string> strings;

    while(string.contains('\\'))
    {
        auto pos{string.find('\\')};
        strings.push_back(string.substr(0, pos));
        string = string.substr(pos+1);
    }
    if(!strings.empty())
        strings.push_back(string);

    if(!strings.empty())
    {
        for(std::string str : strings)
        {
            if(str != strings[0])
            {
                switch(str[0])
                {
                    case 'n':
                        str = str.substr(1);
                        break;
                    case 't':
                        str = std::format("    {}", str.substr(1));
                        break;
                    default:
                        std::println(std::cout, "Carácter de escape inválido en idiomas: {}", str);
                        break;
                }
            }
            if(str.empty())
                TXT::get().write(" ");
            else
                TXT::get().write(str.c_str());
        }
    }
    else 
        TXT::get().write(string.c_str());

}

auto Idiomas::print(const ID_TEXTO id) const -> void
{
    const Texto& texto = textos.at(id);
    setPos(texto.getX(), texto.getY());
    printReal(texto.getTexto(idioma));
}

auto Idiomas::print(const ID_TEXTO id, std::string param) const -> void
{
    const Texto& texto = textos.at(id);
    setPos(texto.getX(), texto.getY());

    printReal(std::vformat(texto.getTextoStr(idioma) , std::make_format_args(param)));
}

auto Idiomas::print(const ID_TEXTO id, int param) const -> void
{
    print(id, std::to_string(param));
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
