//
// Created by julen on 24/08/24.
//

#ifndef IO_H
#define IO_H

class Puerta;
class Entrada;
class Salida;

class IO
{
    static unsigned int idGenerator()
    {
        static unsigned int id{0};
        return id++;
    }

    unsigned int id;
    bool valor{false};

    int lineaX{};
    int lineaY{};

    int conexiones{0};

    Puerta* puerta{nullptr};
    Entrada* entrada{nullptr};
    Salida* salida{nullptr};

public:

    explicit IO(Puerta* puerta_)
        :id{idGenerator()}, puerta{puerta_}
    {}

    explicit IO(Entrada* entrada_)
        :id{idGenerator()}, entrada{entrada_}
    {}

    explicit IO(Salida* salida_)
        :id{idGenerator()}, salida{salida_}
    {}

    void set(const bool b) { valor = b; }
    [[nodiscard]] bool get() const { return valor; }
    [[nodiscard]] unsigned int getId() const { return id; }

    [[nodiscard]] Puerta* getPadrePuerta() const { return puerta; }
    [[nodiscard]] Entrada* getPadreEntrada() const { return entrada; }
    [[nodiscard]] Salida* getPadreSalida() const { return salida; }

    void conectado() { conexiones++; }
    void desconectado() { conexiones--; }
    [[nodiscard]] int getConexiones() const { return conexiones; }

    explicit operator bool() const
    {
        return valor;
    }

    IO& operator=(const bool a)
    {
        valor = a;
        return *this;
    }

    bool operator==(const IO& a) const
    {
        return this->getId() == a.getId();
    }
};

#endif //IO_H
