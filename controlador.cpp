//
// Created by julen on 23/08/24.
//

#include "controlador.h"
#include "Simulables/salida.h"
#include "Simulables/entrada.h"
#include "Simulables/temporizador.h"

auto Controlador::crear(const Puerta::TIPO tipo, int x, int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada, const unsigned int id) -> void
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Puerta* puerta{};
    if(id == 0)
        puerta = new Puerta{renderer, this, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton()};
    else
        puerta = new Puerta{renderer, this, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton(), id};

    window->añadir(puerta->getImg(), ListaIMG::Medio);
    simulables.insert(std::make_pair(puerta->getId(), puerta));
}

auto Controlador::crear(const bool mantener, int x, int y, const unsigned int id) -> void
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Entrada* entrada{};
    if(id == 0)
        entrada = new Entrada{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), mantener};
    else
        entrada = new Entrada{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id, mantener};
    window->añadir(entrada->getImg(), ListaIMG::Medio);

    simulables.insert(std::make_pair(entrada->getId(), entrada));
}

auto Controlador::crear(int x, int y, const unsigned int id) -> void
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Salida* salida{};
    if(id == 0)
        salida = new Salida{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton()};
    else
        salida = new Salida{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id};
    window->añadir(salida->getImg(), ListaIMG::Medio);

    simulables.insert(std::make_pair(salida->getId(), salida));
}

auto Controlador::crear(int x, int y, const int tiempo, const bool entradaNegada, const bool salidaNegada, const unsigned int id) -> void
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Temporizador* temporizador{};
    if(id == 0)
        temporizador = new Temporizador(renderer, this, window->getRaton(), tiempo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, entradaNegada, salidaNegada, txt);
    else
        temporizador = new Temporizador(renderer, this, window->getRaton(), tiempo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, entradaNegada, salidaNegada, txt, id);
    window->añadir(temporizador->getImg(), ListaIMG::Medio);

    simulables.insert(std::make_pair(temporizador->getId(), temporizador));
}


auto Controlador::simular() const -> void
{
    for(const auto& value : std::views::values(simulables))
        if(value->getTipo() == TIPOS_SIMULABLES::Puerta || value->getTipo() == TIPOS_SIMULABLES::Temporizador)
            value->simular();

    for(const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Puerta || value->getTipo() == TIPOS_SIMULABLES::Temporizador)
            value->actualizar();

    for(const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Salida)
            value->simular();
}

auto Controlador::simularInstantaneo() const -> void
{
    for(const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Salida)
            dynamic_cast<Salida*>(value)->simularAntiguo();

    for(const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Puerta)
            dynamic_cast<Puerta*>(value)->simulacionAntiguaTermindada();
}


auto Controlador::limpiar() -> int
{
    int limpiados = 0;
    Simulable* simulable{nullptr};
    for(const auto& value : simulables | std::views::values)
    {
        if(simulable != nullptr)
        {
            if(auto it = std::ranges::find(*seleccionados, simulable); it != seleccionados->end())
                seleccionados->erase(it);
            borrar(simulable);
            simulable = nullptr;
            limpiados++;
        }

        if(value->getDesconectado())
            simulable = value;
    }

    if(simulable != nullptr)
    {
        if(const auto it = std::ranges::find(*seleccionados, simulable); it != seleccionados->end())
            seleccionados->erase(it);
        borrar(simulable);
        limpiados++;
    }

    return limpiados;
}


auto Controlador::getSimulable(const unsigned int id) const -> Simulable*
{
    const auto it = simulables.find(id);
    return it != simulables.end() ? it->second : nullptr;
}

auto Controlador::seleccionar(const SDL_Rect region, std::vector<Simulable*>* vector) const -> void
{
    vector->clear();
    for(const auto& value : simulables | std::views::values)
    {
        if(SDL_HasIntersection(&region, value->getImg()->getRect()))
        {
            vector->push_back(value);
            value->seleccionar(true);
        }
        else
            value->seleccionar(false);
    }
}

auto Controlador::desseleccionar(std::vector<Simulable*>* vector) const -> void
{
    for(Simulable* simulable : *vector)
    {
        simulable->seleccionar(false);
    }
    vector->clear();
}
