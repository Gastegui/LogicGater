//
// Created by julen on 23/08/24.
//


#include <ranges>
#include "controlador.h"
#include "Simulables/salida.h"
#include "Simulables/entrada.h"
#include "Simulables/temporizador.h"
#include "Simulables/condensador.h"

auto Controlador::crear(const Puerta::TIPO tipo, int x, int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada, const unsigned int id) -> unsigned int
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    std::unique_ptr<Puerta> puerta;
    if(id == 0)
        puerta = std::make_unique<Puerta>(renderer, tipo, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada);
    else
        puerta = std::make_unique<Puerta>(renderer, tipo, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, id);

    Window::get().añadir(puerta->getImg(), ListaIMG::Medio);

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
        entrada = std::make_unique<Entrada>(renderer, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, mantener);
    else
        entrada = std::make_unique<Entrada>(renderer, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, id, mantener);

    Window::get().añadir(entrada->getImg(), ListaIMG::Medio);

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
        salida = std::make_unique<Salida>(renderer, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y);
    else
        salida = std::make_unique<Salida>(renderer, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, id);

    Window::get().añadir(salida->getImg(), ListaIMG::Medio);

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
        temporizador = std::make_unique<Temporizador>(renderer, tiempo, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, entradaNegada, salidaNegada);
    else
        temporizador = std::make_unique<Temporizador>(renderer, tiempo, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, entradaNegada, salidaNegada, id);

    Window::get().añadir(temporizador->getImg(), ListaIMG::Medio);

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
        condensador = std::make_unique<Condensador>(renderer, tipo, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y);
    else
        condensador = std::make_unique<Condensador>(renderer, tipo, -Window::get().getEsquinaX() + x, -Window::get().getEsquinaY() + y, id);

    Window::get().añadir(condensador->getImg(), ListaIMG::Medio);

    const unsigned int idCondensador = condensador->getId();
    simulables[idCondensador] = std::move(condensador);
    return idCondensador;
}
auto Controlador::borrar(Simulable *simulable) -> void
{
    if(!simulables.contains(simulable->getId()))
        return;
    borrarConexiones(simulable);

    Window::get().borrar(simulable->getImg(), ListaIMG::Medio);
    simulables.erase(simulable->getId());

    // delete simulable;
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
            if(auto it = std::ranges::find(Raton::getSeleccionados(), simulable); it != Raton::getSeleccionados().end())
                Raton::getSeleccionados().erase(it);
            borrar(simulable);
            simulable = nullptr;
            limpiados++;
        }

        if(value.get()->getDesconectado())
            simulable = value.get();
    }

    if(simulable != nullptr)
    {

        if(const auto it = std::ranges::find(Raton::getSeleccionados(), simulable); it != Raton::getSeleccionados().end())
            Raton::getSeleccionados().erase(it);
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
    Raton::getSeleccionados().clear();
    for(const auto& value : simulables | std::views::values)
    {
        if(SDL_HasIntersection(&region, value.get()->getImg()->getRect()))
        {
            Raton::getSeleccionados().push_back(value.get());
            value.get()->seleccionar(true);
        }
        else
            value.get()->seleccionar(false);
    }
}

auto Controlador::desseleccionar() const -> void
{
    for(Simulable* simulable : Raton::getSeleccionados())
    {
        simulable->seleccionar(false);
    }
    Raton::getSeleccionados().clear();
}

auto Controlador::duplicarSeleccion() -> void
{
    std::map<unsigned int, unsigned int> nuevos;
    Puerta* puerta{};
    Entrada* entrada{};
    Salida* salida{};
    Temporizador* temporizador{};
    Condensador* condensador{};

    for(Simulable* simulable : Raton::getSeleccionados())
    {
        if((puerta = dynamic_cast<Puerta*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[puerta->getId()] = crear(puerta->getTipoPuerta(), Window::get().getEsquinaX() + puerta->getImg()->getRect()->x + 10, Window::get().getEsquinaY() + puerta->getImg()->getRect()->y + 10, puerta->getArribaNegado(), puerta->getAbajoNegado(), puerta->getSalidaNegada());
        else if((entrada = dynamic_cast<Entrada*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[entrada->getId()] = crear(entrada->getMantener(), Window::get().getEsquinaX() + entrada->getImg()->getRect()->x + 10, Window::get().getEsquinaY() + entrada->getImg()->getRect()->y + 10);
        else if((salida = dynamic_cast<Salida*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[salida->getId()] = crear(Window::get().getEsquinaX() + salida->getImg()->getRect()->x + 10, Window::get().getEsquinaY() + salida->getImg()->getRect()->y + 10);
        else if((temporizador = dynamic_cast<Temporizador*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[temporizador->getId()] = crear(Window::get().getEsquinaX() + temporizador->getImg()->getRect()->x + 10, Window::get().getEsquinaY() + temporizador->getImg()->getRect()->y + 10, temporizador->getTiempoTotal(), temporizador->getEntradaNegada(), temporizador->getSalidaNegada());
        else if((condensador = dynamic_cast<Condensador*>(simulable)) != nullptr) // NOLINT(*-assignment-in-if-condition)
            nuevos[condensador->getId()] = crear(condensador->getImg()->getRect()->x + 10, condensador->getImg()->getRect()->y + 10, condensador->getTipoPuerta());
        }

    for(const ListaLineas::Linea linea : *listaLineas.getLista())
    {
        if(nuevos.contains(linea.io->getIdSimulable()) && std::ranges::find(Raton::getSeleccionados(), linea.destino) != Raton::getSeleccionados().end())
        {
            Simulable* destino = getSimulable(nuevos[linea.destino->getId()]);
            marcarOrigen(getSimulable(nuevos[linea.io->getIdSimulable()])->getIOSalida());
            destino->interactuar(destino->getLinea(linea.destino->getConexion(linea.io))->first - destino->getImg()->getRect()->x, destino->getLinea(linea.destino->getConexion(linea.io))->second - destino->getImg()->getRect()->y, INTERACCIONES::ConexionArriba);
        }
    }

    desseleccionar();
    for(const unsigned int id : nuevos | std::views::values)
    {
        Raton::getSeleccionados().push_back(getSimulable(id));
        getSimulable(id)->seleccionar(true);
    }
}
