//
// Created by julen on 22/08/24.
//

#ifndef GATE_H
#define GATE_H

#include "../Wrapper/image.h"
#include "../raton.h"
#include "../io.h"
#include "simulable.h"

class IMG;
class Window;
class Raton;
class IO;
class SistemaGuardado;
class Controlador;

class Puerta final : public Simulable
{
    friend class SistemaGuardado;
    Controlador* controlador;
    bool arribaNegado;
    bool abajoNegado;
    bool salidaNegada;

    bool siguiente{false};
    bool simulandoAntiguo{false};

    IO* arriba{nullptr};
    IO* abajo{nullptr};
    IO salida;

    std::pair<int, int> lineaArriba{0, 11};
    std::pair<int, int> lineaAbajo{0, 37};
    //std::pair<int, int> lineaSalida{99, 24};


    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + get_offset(ID_TIPOS::Puerta);
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

    int x;
    int y;
    bool mover{false};

    Raton* raton;
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

    Puerta(SDL_Renderer* renderer, Controlador* controlador_, const Tipo tipo_, const int x_, const int y_, Raton* raton)
        : Puerta(renderer, controlador_, tipo_, x_, y_, false, false, false, raton)
    {}

    Puerta(SDL_Renderer* renderer, Controlador* controlador_, const Tipo tipo_, const int x_, const int y_, const bool arribaNegado_, const bool abajoNegado_, const bool salidaNegada_, Raton* raton_)
        :Puerta(renderer, controlador_, tipo_, x_, y_, arribaNegado_, abajoNegado_, salidaNegada_, raton_, idGenerator())
    {}

    Puerta(SDL_Renderer* renderer, Controlador* controlador_, const Tipo tipo_, const int x_, const int y_, const bool arribaNegado_, const bool abajoNegado_, const bool salidaNegada_, Raton* raton_, const unsigned int id_)
        : Simulable(id_), controlador(controlador_), arribaNegado{arribaNegado_}, abajoNegado{abajoNegado_}, salidaNegada{salidaNegada_}, salida(this, 100+x_, 25+y_), x{x_}, y{y_}, raton{raton_}, tipo{tipo_},
          imagen{
              renderer, x_, y_,
              arribaNegado_ ? "./img/puertas/entrada_arriba_negada.png" : "./img/puertas/entrada_arriba_normal.png",
              abajoNegado_ ? "./img/puertas/entrada_abajo_negada.png" : "./img/puertas/entrada_abajo_normal.png",
              tipo_ == AND ? "./img/puertas/and.png" : tipo_ == OR ? "./img/puertas/or.png" : "./img/puertas/xor.png",
              salidaNegada_ ? "./img/puertas/salida_negada.png" : "./img/puertas/salida_normal.png"
          }
    {
        imagen.setClickable(this);
        lineaArriba.first += x_;
        lineaArriba.second += y_;
        lineaAbajo.first += x_;
        lineaAbajo.second += y_;
    }

    ~Puerta() override = default;

    void setArriba(IO* arriba_) { arriba = arriba_; }
    void setAbajo(IO* abajo_) { abajo = abajo_; }
    [[nodiscard]] IO* getSalida() { return &salida; }
    [[nodiscard]] IO* getArriba() const { return arriba; }
    [[nodiscard]] IO* getAbajo() const { return abajo; }
    [[nodiscard]] bool getDesconectado() const override { return arriba == nullptr && abajo == nullptr && salida.getConexiones() == 0; }
    [[nodiscard]] IMG* getImg() override { return &imagen; }

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


    void simular() override;
    void actualizar() override;
    bool simularAntiguo();
    void simulacionAntiguaTermindada();
    void moverRel(int x_, int y_);
    bool interactuar(int posX, int posY, INTERACCIONES interaccion) override;
    void setIONull(IO* io) override;
    [[nodiscard]] IO* getIOSalida() override { return &salida; }
    [[nodiscard]] std::pair<int, int>* getLinea(const int conexion) override
    {
        if(conexion == 1)
            return &lineaArriba;
        if(conexion == 2)
            return &lineaAbajo;
        return nullptr;
    }
};

#endif //GATE_H
