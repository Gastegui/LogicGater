//
// Created by julen on 25/08/24.
//

#ifndef BOTON_H
#define BOTON_H

#include "../io.h"
#include "../Wrapper/image.h"
#include "../raton.h"
#include "simulable.h"

class IO;
class IMG;
class Raton;
class SistemaGuardado;

class Entrada final : public Simulable
{
    friend class SistemaGuardado;
    Controlador* controlador;

    IO salida;
    bool mantener;
    IMG img;
    bool mover{false};

    //std::pair<int, int> linea{25, 25};
    Raton* raton;

    static unsigned int idGenerator()
    {
        static unsigned int id = 1 + get_offset(ID_TIPOS::Entrada);
        return id++;
    }

    static void gastarIds(const unsigned int cantidad)
    {
        for(unsigned int i = 0; i < cantidad; i++)
            idGenerator();
    }

public:

    Entrada(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const bool mantener_ = false)
        :Entrada(renderer, controlador_, x, y, raton_, idGenerator(), mantener_)
    {}

    Entrada(SDL_Renderer* renderer, Controlador* controlador_, const int x, const int y, Raton* raton_, const unsigned int id_, const bool mantener_ = false)
        :Simulable{id_}, controlador{controlador_}, salida{this, x + 25, y + 25}, mantener{mantener_}, img{"./img/entrada/apagado.png", renderer, x, y}, raton{raton_}
    {
        img.setClickable(this);
    }

    ~Entrada() override = default;

    [[nodiscard]] IO* getSalida() { return &salida; }
    [[nodiscard]] IMG* getImg() override { return &img; }
    [[nodiscard]] bool getDesconectado() const override { return salida.getConexiones() == 0; }
    [[nodiscard]] bool getMantener() const { return mantener; }
    void setMantener(const bool mantener_) { mantener = mantener_; }

    [[nodiscard]] bool get() const { return salida.get(); }
    void set(const bool valor)
    {
        img.cambiarImagen(valor ? "./img/entrada/encendido.png" : "./img/entrada/apagado.png");
        salida.set(valor);
    }

    void moverRel(int x, int y);

    void simular() override {}
    void actualizar() override {}
    bool interactuar(int posX, int posY, INTERACCIONES interaccion) override;
    void setIONull(IO* io) override {}
    [[nodiscard]] IO* getIOSalida() override { return &salida; }
    [[nodiscard]] std::pair<int, int>* getLinea(int conexion) override { return nullptr; }
    [[nodiscard]] int getConexion(IO* io) const override { return 0; }
};



#endif //BOTON_H
