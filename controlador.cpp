//
// Created by julen on 23/08/24.
//

#include "controlador.h"
#include "Simulables/salida.h"
#include "Simulables/entrada.h"
//#include "Simulables/Temporizador.h"

void Controlador::crear(const Puerta::Tipo tipo, int x, int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);
    Puerta* puerta{nullptr};
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

    Entrada* entrada;
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

    Salida* salida;
    if(id == 0)
        salida = new Salida{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton()};
    else
        salida = new Salida{renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id};
    window->añadir(salida->getImg(), ListaIMG::MEDIO);

    simulables.insert(std::make_pair(salida->getId(), salida));

}


void Controlador::simular() const
{
    for (const auto& value : std::views::values(simulables))
        if(dynamic_cast<Puerta*>(value) != nullptr)
            value->simular();

    for (const auto& value : simulables | std::views::values)
        if(dynamic_cast<Puerta*>(value) != nullptr)
            value->actualizar();

    for (const auto& value : simulables | std::views::values)
        if(dynamic_cast<Salida*>(value) != nullptr)
            value->simular();

}

void Controlador::simularInstantaneo() const
{
    for (const auto& value : simulables | std::views::values)
        if(dynamic_cast<Salida*>(value) != nullptr)
            dynamic_cast<Salida*>(value)->simularAntiguo();

    for (const auto& value : simulables | std::views::values)
        if(dynamic_cast<Puerta*>(value) != nullptr)
            dynamic_cast<Puerta*>(value)->simulacionAntiguaTermindada();
}


int Controlador::limpiar()
{
    int limpiados = 0;
    Puerta* borrarPuerta{nullptr};
    Salida* borrarSalida{nullptr};
    Entrada* borrarEntrada{nullptr};

    for (const auto& value : simulables | std::views::values)
    {
        if(borrarPuerta != nullptr)
        {
            borrar(borrarPuerta);
            borrarPuerta = nullptr;
            limpiados++;
        }
        if(borrarSalida != nullptr)
        {
            borrar(borrarSalida);
            borrarSalida = nullptr;
            limpiados++;
        }

        if(borrarEntrada != nullptr)
        {
            borrar(borrarEntrada);
            borrarEntrada = nullptr;
            limpiados++;
        }

        if(dynamic_cast<Puerta*>(value) != nullptr)
        {
            borrarPuerta = dynamic_cast<Puerta*>(value); //Se usa como tmp
            if(!borrarPuerta->getDesconectado())
                borrarPuerta = nullptr;
        }

        if(dynamic_cast<Salida*>(value) != nullptr)
        {
            borrarSalida = dynamic_cast<Salida*>(value); //Se usa como tmp
            if(!borrarSalida->getDesconectado())
                borrarSalida = nullptr;
        }
        if(dynamic_cast<Entrada*>(value) != nullptr)
        {
            borrarEntrada = dynamic_cast<Entrada*>(value); //Se usa como tmp
            if(!borrarEntrada->getDesconectado())
                borrarEntrada = nullptr;
        }
    }
    return limpiados;
}


Simulable* Controlador::getSimulable(const unsigned int id) const
{
    const auto it = simulables.find(id);
    return it != simulables.end() ? it->second : nullptr;
}
