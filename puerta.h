//
// Created by julen on 22/08/24.
//

#ifndef GATE_H
#define GATE_H

#include "./Wrapper/image.h"
#include "raton.h"
#include "io.h"

class IMG;
class Window;
class Raton;
class IO;

class Puerta
{
    bool arribaNegado;
    bool abajoNegado;
    bool salidaNegada;

    bool simulando{false};

    IO* arriba{nullptr};
    IO* abajo{nullptr};
    IO salida{this};

    std::pair<int, int> lineaArriba{0, 11};
    std::pair<int, int> lineaAbajo{0, 37};
    std::pair<int, int> lineaSalida{99, 24};

    unsigned int id;

    static unsigned int idGenerator()
    {
        static unsigned int id = 0;
        return id++;
    }

    int x;
    int y;
    bool mover{false};
public:
    enum Tipo
    {
        AND,
        OR,
        XOR
    };
private:
    Tipo tipo{};
    IMG imagen;

public:

    Puerta(SDL_Renderer* renderer, const Tipo tipo_, const int x_, const int y_)
        : Puerta(renderer, tipo_, x_, y_, false, false, false)
    {}

    Puerta(SDL_Renderer* renderer, const Tipo tipo_, const int x_, const int y_, const bool arribaNegado_, const bool abajoNegado_, const bool salidaNegada_)
        :arribaNegado{arribaNegado_}, abajoNegado{abajoNegado_}, salidaNegada{salidaNegada_}, id{idGenerator()}, x{x_}, y{y_}, tipo{tipo_},
            imagen{renderer, x_, y_, arribaNegado_ ? "./img/puertas/entrada_arriba_negada.png" : "./img/puertas/entrada_arriba_normal.png",
                             abajoNegado_ ? "./img/puertas/entrada_abajo_negada.png" : "./img/puertas/entrada_abajo_normal.png",
                             tipo_ == AND ? "./img/puertas/and.png" : tipo_ == OR ? "./img/puertas/or.png" : "./img/puertas/xor.png",
                             salidaNegada_ ? "./img/puertas/salida_negada.png" : "./img/puertas/salida_normal.png"}
    {
        imagen.setClickable(this);
        lineaArriba.first += x;
        lineaArriba.second += y;
        lineaAbajo.first += x;
        lineaAbajo.second += y;
        lineaSalida.first += x;
        lineaSalida.second += y;
    }

    void setArriba(IO* arriba_) { arriba = arriba_; }
    void setAbajo(IO* abajo_) { abajo = abajo_; }
    [[nodiscard]] IO* getSalida() { return &salida; }
    [[nodiscard]] IO* getArriba() const { return arriba; }
    [[nodiscard]] IO* getAbajo() const { return abajo; }
    [[nodiscard]] bool getDesconectado() const { return arriba == nullptr && abajo == nullptr && salida.getConexiones() == 0; }
    [[nodiscard]] IMG* getIMG() { return &imagen; }

    [[nodiscard]] std::pair<int, int>* getLineaArribaPos() { return &lineaArriba; }
    [[nodiscard]] std::pair<int, int>* getLineaAbajoPos() { return &lineaAbajo; }
    [[nodiscard]] std::pair<int, int>* getLineaSalidePos() { return &lineaSalida; }

    bool cambiar(const int arribaNegado_, const int abajoNegado_, const Tipo tipo_, const int salidaNegada_)
    {
        arribaNegado = arribaNegado_;
        abajoNegado = abajoNegado_;
        tipo = tipo_;
        salidaNegada = salidaNegada_;

        return cambiar();
    }

    bool cambiar()
    {
        return imagen.cambiarPuerta(arribaNegado ? "./img/puertas/entrada_arriba_negada.png" : "./img/puertas/entrada_arriba_normal.png",
                             abajoNegado ? "./img/puertas/entrada_abajo_negada.png" : "./img/puertas/entrada_abajo_normal.png",
                             tipo == AND ? "./img/puertas/and.png" : tipo == OR ? "./img/puertas/or.png" : "./img/puertas/xor.png",
                             salidaNegada ? "./img/puertas/salida_negada.png" : "./img/puertas/salida_normal.png");
    }

    void click(int x, int y, Raton::Evento evento);

    bool simular();
    void simulacionTermindada();
};

#endif //GATE_H
