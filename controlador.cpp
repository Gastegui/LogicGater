//
// Created by julen on 23/08/24.
//

#include "controlador.h"
#include "Elementos/salida.h"
#include "Elementos/entrada.h"

void Controlador::desmarcarOrigen()
{
    origen = nullptr;
}

void Controlador::crear(const Puerta::Tipo tipo, int x, int y, const bool arribaNegado, const bool abajoNegado, const bool salidaNegada, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);
    Puerta* puerta{nullptr};
    if(id == 0)
        puerta = new Puerta{renderer, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton()};
    else
        puerta = new Puerta{renderer, tipo, -window->getEsquinaX() + x, -window->getEsquinaY() + y, arribaNegado, abajoNegado, salidaNegada, window->getRaton(), id};

    window->añadir(puerta->getImg(), ListaIMG::MEDIO);
    simulables.insert(std::make_pair(puerta->getId(), puerta));
}

void Controlador::borrarConexiones(Puerta* puerta, const bool arriba, const bool abajo, const bool salida)
{
    if(origen != nullptr)
        origen = nullptr;

    window->borrarLineas(puerta, arriba, abajo, salida);

    if(arriba && puerta->getArriba() != nullptr)
    {
        puerta->getArriba()->desconectado();
        puerta->setArriba(nullptr);
    }

    if(abajo && puerta->getAbajo() != nullptr)
    {
        puerta->getAbajo()->desconectado();
        puerta->setAbajo(nullptr);
    }

    if(salida && puerta->getSalida()->getConexiones() != 0)
    {
        for (const auto& value : simulables | std::views::values)
        {
            if(dynamic_cast<Puerta*>(value) != nullptr)
            {
                Puerta* puerta_ = dynamic_cast<Puerta*>(value);
                if(puerta_->getArriba() == puerta->getSalida())
                {
                    puerta->getSalida()->desconectado();
                    puerta_->setArriba(nullptr);
                }
                if(puerta_->getAbajo() == puerta->getSalida())
                {
                    puerta->getSalida()->desconectado();
                    puerta_->setAbajo(nullptr);
                }
            }
            else if(dynamic_cast<Salida*>(value) != nullptr)
            {
                Salida* salida_ = dynamic_cast<Salida*>(value);
                if(salida_->getEntrada() == puerta->getSalida())
                {
                    puerta->getSalida()->desconectado();
                    salida_->setEntrada(nullptr);
                }
            }
        }
    }
}


bool Controlador::borrar(Puerta* puerta)
{
    if(!simulables.contains(puerta->getId()))
        return false;

    borrarConexiones(puerta);
    window->borrar(puerta->getImg(), ListaIMG::MEDIO);

    if(simulables.erase(puerta->getId()) == 0)
        return false;

    delete puerta;
    return true;
}

void Controlador::marcarOrigen(Puerta* puerta)
{
    origen = puerta->getSalida();
}

bool Controlador::destino(Puerta* puerta, const bool arriba)
{
    if(origen == nullptr || (arriba && puerta->getArriba() != nullptr) || (!arriba && puerta->getAbajo() != nullptr))
        return false;

    if(arriba)
        puerta->setArriba(origen);
    else
        puerta->setAbajo(origen);

    if(origen->getPadreEntrada() != nullptr)
        window->añadirLinea(origen->getPadreEntrada(), puerta, arriba);
    else
        window->añadirLinea(origen->getPadrePuerta(), puerta, arriba);

    origen->conectado();

    origen = nullptr;
    return true;
}

void Controlador::crear(const bool mantener, int x, int y, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Entrada* entrada;
    if(id == 0)
        entrada = new Entrada{renderer, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), mantener};
    else
        entrada = new Entrada{renderer, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id, mantener};
    window->añadir(entrada->getImg(), ListaIMG::MEDIO);

    simulables.insert(std::make_pair(entrada->getId(), entrada));

}

void Controlador::borrarConexiones(Entrada* entrada) const
{
    window->borrarLineas(entrada);

    for (const auto& value : simulables | std::views::values)
    {
        if(dynamic_cast<Puerta*>(value) != nullptr)
        {
            Puerta* puerta_ = dynamic_cast<Puerta*>(value);
            if(puerta_->getArriba() == entrada->getSalida())
            {
                entrada->getSalida()->desconectado();
                puerta_->setArriba(nullptr);
            }
            if(puerta_->getAbajo() == entrada->getSalida())
            {
                entrada->getSalida()->desconectado();
                puerta_->setAbajo(nullptr);
            }
        }
        else if(dynamic_cast<Salida*>(value) != nullptr)
        {
            Salida* salida_ = dynamic_cast<Salida*>(value);
            if(salida_->getEntrada() == entrada->getSalida())
            {
                entrada->getSalida()->desconectado();
                salida_->setEntrada(nullptr);
            }
        }
    }
}


bool Controlador::borrar(Entrada* entrada)
{
    if(!simulables.contains(entrada->getId()))
        return false;

    borrarConexiones(entrada);
    window->borrar(entrada->getImg(), ListaIMG::MEDIO);

    if(simulables.erase(entrada->getId()) == 0)
        return false;

    delete entrada;
    return true;
}

void Controlador::marcarOrigen(Entrada* boton)
{
    origen = boton->getSalida();
}

void Controlador::crear(int x, int y, const unsigned int id)
{
    if(x == -1 && y == -1)
        SDL_GetMouseState(&x, &y);

    Salida* salida;
    if(id == 0)
        salida = new Salida{renderer, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton()};
    else
        salida = new Salida{renderer, -window->getEsquinaX() + x, -window->getEsquinaY() + y, window->getRaton(), id};
    window->añadir(salida->getImg(), ListaIMG::MEDIO);

    simulables.insert(std::make_pair(salida->getId(), salida));

}

bool Controlador::borrar(Salida* salida)
{
    if(!simulables.contains(salida->getId()))
        return false;

    borrarConexiones(salida);
    window->borrar(salida->getImg(), ListaIMG::MEDIO);

    if(simulables.erase(salida->getId()) == 0)
        return false;

    delete salida;
    return true;
}

void Controlador::borrarConexiones(Salida* salida) const
{
    if(salida->getEntrada() == nullptr)
        return;

    window->borrarLineas(salida);
    salida->getEntrada()->desconectado();
    salida->setEntrada(nullptr);
}

bool Controlador::destino(Salida* salida)
{
    if(origen == nullptr || salida->getEntrada() != nullptr)
        return false;
    salida->setEntrada(origen);

    if(origen->getPadreEntrada() != nullptr)
        window->añadirLinea(origen->getPadreEntrada(), salida);
    else
        window->añadirLinea(origen->getPadrePuerta(), salida);

    origen->conectado();
    origen = nullptr;

    return true;
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


Puerta* Controlador::getPuerta(const unsigned int id) const
{
    const auto it = simulables.find(id);
    return it != simulables.end() ? dynamic_cast<Puerta*>(it->second) : nullptr;
}

Entrada* Controlador::getEntrada(const unsigned int id) const
{
    const auto it = simulables.find(id);
    return it != simulables.end() ? dynamic_cast<Entrada*>(it->second) : nullptr;
}

Salida* Controlador::getSalida(const unsigned int id) const
{
    const auto it = simulables.find(id);
    return it != simulables.end() ? dynamic_cast<Salida*>(it->second) : nullptr;
}
