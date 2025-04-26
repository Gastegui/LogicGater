//
// Created by julen on 28/01/25.
//

#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

#include "simulable.h"
#include "../io.h"
#include "../Wrapper/image.h"


class Temporizador final : public Simulable
{
    friend class SistemaGuardado;
    Controlador* controlador;
    Raton* raton;

    bool entradaNegada;
    bool salidaNegada;

    IO* entrada{};
    IO salida;

    std::pair<int, int> linea{0, 25};

    int x;
    int y;

    bool mover{false};

    int ciclosTotales;
    int ciclosActuales{0};

    IMG imagen;

    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + get_offset(ID_TIPOS::Temporizador);
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

    bool cambiar();

public:

    Temporizador(SDL_Renderer* renderer, Controlador* controlador_, Raton* raton_, const int tiempo_, const int x_, const int y_, const bool entradaNegada_, const bool salidaNegada_, TXT* txt_)
        : Temporizador(renderer, controlador_, raton_, tiempo_, x_, y_, entradaNegada_, salidaNegada_, txt_, idGenerator())
    {}

    Temporizador(SDL_Renderer* renderer, Controlador* controlador_, Raton* raton_, const int tiempo_, const int x_, const int y_, const bool entradaNegada_, const bool salidaNegada_, TXT* txt_, const unsigned int id_)
        :Simulable(id_), controlador{controlador_}, raton{raton_}, entradaNegada{entradaNegada_}, salidaNegada{salidaNegada_}, salida{this, x_ + 100, y_ + 25}, x{x_}, y{y_}, ciclosTotales{tiempo_},
    imagen(renderer, x_, y_,
        entradaNegada_ ? "./img/temporizador/entrada_negada.png" : "./img/temporizador/entrada_normal.png",
        "./img/temporizador/cuerpo.png",
        salidaNegada_ ? "./img/temporizador/salida_negada.png" : "./img/temporizador/salida_normal.png",
        "./img/temporizador/0.png", ciclosTotales, txt_)
    {
        imagen.setClickable(this);
        linea.first += x_;
        linea.second += y_;
    }

    ~Temporizador() override = default;

    void simular() override;
    void actualizar() override;
    bool interactuar(int posX, int posY, INTERACCIONES interaccion) override;
    IMG* getImg() override { return &imagen; }
    void setIONull(IO* io) override
    {
        if(entrada != nullptr && entrada == io)
            entrada = nullptr;
    }
    [[nodiscard]] IO* getIOSalida() override { return &salida; }
    [[nodiscard]] bool getDesconectado() const override { return entrada == nullptr && salida.getConexiones() == 0; }
    [[nodiscard]] std::pair<int, int>* getLinea(const int conexion) override
    {
        if(conexion == 1)
            return &linea;
        return nullptr;
    }
    [[nodiscard]] int getConexion(IO* io) const override
    {
        if(entrada != nullptr && entrada == io)
            return 1;
        return 0;
    }
};



#endif //TEMPORIZADOR_H
