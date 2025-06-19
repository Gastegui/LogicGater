//
// Created by julen on 23/08/24.
//

#include "controlador.h"
#include "Simulables/salida.h"
#include "Simulables/entrada.h"
#include "Simulables/temporizador.h"

void Controlador::crear(const Puerta::Tipo tipo, int x, int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Puerta* puerta{}; //TODO: QUITAR PUNTEROS DE AQUÍ, METERLOS DIRECTAMENTE AL MAPA? QUITAR PUNTEROS EN GENERAL (LAS LINKED LISTS QUE QUEDAN HAY QUE SUSTITUIRLAS POR VECTORES)
    if(id == 0)
        puerta = new Puerta{renderer, this, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton()};
    else
        puerta = new Puerta{renderer, this, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton(), id};

    window->añadir(puerta->getImg(), ListaIMG::MEDIO);
    simulables.insert(std::make_pair(puerta->getId(), puerta));
}

void Controlador::crear(const bool mantener, int x, int y, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Entrada* entrada{};
    if(id == 0)
        entrada = new Entrada{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), mantener};
    else
        entrada = new Entrada{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id, mantener};
    window->añadir(entrada->getImg(), ListaIMG::MEDIO);

    simulables.insert(std::make_pair(entrada->getId(), entrada));

}

void Controlador::crear(int x, int y, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Salida* salida{};
    if(id == 0)
        salida = new Salida{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton()};
    else
        salida = new Salida{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id};
    window->añadir(salida->getImg(), ListaIMG::MEDIO);

    simulables.insert(std::make_pair(salida->getId(), salida));

}

void Controlador::crear(int x, int y, const int tiempo, const bool entradaNegada, const bool salidaNegada, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Temporizador* temporizador{};
    if(id == 0)
        temporizador = new Temporizador(renderer, this, window->getRaton(), tiempo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, entradaNegada, salidaNegada, txt);
    else
        temporizador = new Temporizador(renderer, this, window->getRaton(), tiempo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, entradaNegada, salidaNegada, txt, id);
    window->añadir(temporizador->getImg(), ListaIMG::MEDIO);

    simulables.insert(std::make_pair(temporizador->getId(), temporizador));
}


void Controlador::simular() const
{
    for (const auto& value : std::views::values(simulables))
        if(value->getTipo() == TIPOS_SIMULABLES::Puerta || value->getTipo() == TIPOS_SIMULABLES::Temporizador)
            value->simular();

    for (const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Puerta || value->getTipo() == TIPOS_SIMULABLES::Temporizador)
            value->actualizar();

    for (const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Salida)
            value->simular();

}

void Controlador::simularInstantaneo() const
{
    for (const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Salida)
            dynamic_cast<Salida*>(value)->simularAntiguo();

    for (const auto& value : simulables | std::views::values)
        if(value->getTipo() == TIPOS_SIMULABLES::Puerta)
            dynamic_cast<Puerta*>(value)->simulacionAntiguaTermindada();
}


auto Controlador::limpiar() -> int
{
    int limpiados = 0;
    Simulable* simulable{nullptr};
    for (const auto& value : simulables | std::views::values)
    {
        if(simulable != nullptr)
        {
            borrar(simulable);
            simulable = nullptr;
            limpiados++;
        }

        if(value->getDesconectado())
            simulable = value;
    }

    if(simulable != nullptr)
    {
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
