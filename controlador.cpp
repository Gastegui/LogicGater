//
// Created by julen on 23/08/24.
//

#include <ranges>

#include "controlador.h"
#include "Simulables/salida.h"
#include "Simulables/entrada.h"
#include "Simulables/temporizador.h"
#include "Simulables/condensador.h"
#include "enums.h"

auto Controlador::crear(const Puerta::TIPO tipo, int x, int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada, const unsigned int id) -> unsigned int
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    std::unique_ptr<Puerta> puerta;
    if(id == 0)
        puerta = std::make_unique<Puerta>(renderer, this, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton());
    else
        puerta = std::make_unique<Puerta>(renderer, this, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton(), id);

    window->añadir(puerta->getImg(), ListaIMG::Medio);

    const unsigned int idPuerta = puerta->getId();
    simulables[idPuerta] = std::move(puerta);
    return idPuerta;
}

auto Controlador::crear(const bool mantener, int x, int y, const unsigned int id) -> unsigned int
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    std::unique_ptr<Entrada> entrada{};
    if(id == 0)
        entrada = std::make_unique<Entrada>(renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), mantener);
    else
        entrada = std::make_unique<Entrada>(renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id, mantener);

    window->añadir(entrada->getImg(), ListaIMG::Medio);

    const unsigned int idEntrada = entrada->getId();
    simulables[idEntrada] = std::move(entrada);
    return idEntrada;
}

auto Controlador::crear(int x, int y, const unsigned int id) -> unsigned int
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    std::unique_ptr<Salida> salida{};
    if(id == 0)
        salida = std::make_unique<Salida>(renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton());
    else
        salida = std::make_unique<Salida>(renderer, this, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id);

    window->añadir(salida->getImg(), ListaIMG::Medio);

    const unsigned int idSalida = salida->getId();
    simulables[idSalida] = std::move(salida);
    return idSalida;
}

auto Controlador::crear(int x, int y, const int tiempo, const bool entradaNegada, const bool salidaNegada, const unsigned int id) -> unsigned int
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    std::unique_ptr<Temporizador> temporizador{};
    if(id == 0)
        temporizador = std::make_unique<Temporizador>(renderer, this, window->getRaton(), tiempo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, entradaNegada, salidaNegada, txt);
    else
        temporizador = std::make_unique<Temporizador>(renderer, this, window->getRaton(), tiempo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, entradaNegada, salidaNegada, txt, id);

    window->añadir(temporizador->getImg(), ListaIMG::Medio);

    const unsigned int idTemporizaador = temporizador->getId();
    simulables[idTemporizaador] = std::move(temporizador);
    return idTemporizaador;
}

auto Controlador::crear(int x, int y, Puerta::TIPO tipo, unsigned int id) -> unsigned int
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);
    std::unique_ptr<Condensador> condensador{};
    if(id == 0)
        condensador = std::make_unique<Condensador>(renderer, this, window->getRaton(), tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y);
    else
        condensador = std::make_unique<Condensador>(renderer, this, window->getRaton(), tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, id);

    window->añadir(condensador->getImg(), ListaIMG::Medio);

    const unsigned int idCondensador = condensador->getId();
    simulables[idCondensador] = std::move(condensador);
    return idCondensador;
}

auto Controlador::simular() const -> void
{
    for(const auto& value : std::views::values(simulables))
        if(value.get()->getTipo() == TIPOS_SIMULABLES::Puerta || value.get()->getTipo() == TIPOS_SIMULABLES::Temporizador || value.get()->getTipo() == TIPOS_SIMULABLES::Condensador)
            value.get()->simular();

    for(const auto& value : simulables | std::views::values)
        if(value.get()->getTipo() == TIPOS_SIMULABLES::Puerta || value.get()->getTipo() == TIPOS_SIMULABLES::Temporizador || value.get()->getTipo() == TIPOS_SIMULABLES::Condensador)
            value.get()->actualizar();

    for(const auto& value : simulables | std::views::values)
        if(value.get()->getTipo() == TIPOS_SIMULABLES::Salida)
            value.get()->simular();
}

auto Controlador::simularInstantaneo() const -> void
{
    for(const auto& value : simulables | std::views::values)
        if(value.get()->getTipo() == TIPOS_SIMULABLES::Salida)
            dynamic_cast<Salida*>(value.get())->simularAntiguo();

    for(const auto& value : simulables | std::views::values)
        if(value.get()->getTipo() == TIPOS_SIMULABLES::Puerta)
            dynamic_cast<Puerta*>(value.get())->simulacionAntiguaTermindada();
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

        if(value.get()->getDesconectado())
            simulable = value.get();
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
    return it != simulables.end() ? it->second.get() : nullptr;
}

auto Controlador::seleccionar(const SDL_Rect region) const -> void
{
    seleccionados->clear();
    for(const auto& value : simulables | std::views::values)
    {
        if(SDL_HasIntersection(&region, value.get()->getImg()->getRect()))
        {
            seleccionados->push_back(value.get());
            value.get()->seleccionar(true);
        }
        else
            value.get()->seleccionar(false);
    }
}

auto Controlador::desseleccionar() const -> void
{
    for(Simulable* simulable : *seleccionados)
    {
        simulable->seleccionar(false);
    }
    seleccionados->clear();
}

auto Controlador::duplicarSeleccion() -> void
{
    std::map<unsigned int, unsigned int> nuevos;
    Puerta* puerta{};
    Entrada* entrada{};
    Salida* salida{};
    Temporizador* temporizador{};
    Condensador* condensador{};

    for(Simulable* simulable : *seleccionados)
    {
        if((puerta = dynamic_cast<Puerta*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[puerta->getId()] = crear(puerta->getTipoPuerta(), window->getEsquinaX() + puerta->getImg()->getRect()->x + 10, window->getEsquinaY() + puerta->getImg()->getRect()->y + 10, puerta->getArribaNegado(), puerta->getAbajoNegado(), puerta->getSalidaNegada());
        else if((entrada = dynamic_cast<Entrada*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[entrada->getId()] = crear(entrada->getMantener(), window->getEsquinaX() + entrada->getImg()->getRect()->x + 10, window->getEsquinaY() + entrada->getImg()->getRect()->y + 10);
        else if((salida = dynamic_cast<Salida*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[salida->getId()] = crear(window->getEsquinaX() + salida->getImg()->getRect()->x + 10, window->getEsquinaY() + salida->getImg()->getRect()->y + 10);
        else if((temporizador = dynamic_cast<Temporizador*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[temporizador->getId()] = crear(window->getEsquinaX() + temporizador->getImg()->getRect()->x + 10, window->getEsquinaY() + temporizador->getImg()->getRect()->y + 10, temporizador->getTiempoTotal(), temporizador->getEntradaNegada(), temporizador->getSalidaNegada());
        else if((condensador = dynamic_cast<Condensador*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[condensador->getId()] = crear(condensador->getImg()->getRect()->x + 10, condensador->getImg()->getRect()->y + 10, condensador->getTipoPuerta());
        }

    for(const ListaLineas::Linea linea : *listaLineas.getLista())
    {
        if(nuevos.contains(linea.io->getIdSimulable()) && std::ranges::find(*seleccionados, linea.destino) != seleccionados->end())
        {
            Simulable* destino = getSimulable(nuevos[linea.destino->getId()]);
            marcarOrigen(getSimulable(nuevos[linea.io->getIdSimulable()])->getIOSalida());
            destino->interactuar(destino->getLinea(linea.destino->getConexion(linea.io))->first - destino->getImg()->getRect()->x, destino->getLinea(linea.destino->getConexion(linea.io))->second - destino->getImg()->getRect()->y, INTERACCIONES::ConexionArriba);
        }
    }

    desseleccionar();
    for(const unsigned int id : nuevos | std::views::values)
    {
        seleccionados->push_back(getSimulable(id));
        getSimulable(id)->seleccionar(true);
    }
}
