//
// Created by julen on 9/01/25.
//

#ifndef SIMULABLE_H
#define SIMULABLE_H
#include "../enums.h"

class IMG;
class IO;

class Simulable
{
    unsigned int id{0};
public:
    
    explicit Simulable(const unsigned int id) : id(id) {}

    virtual ~Simulable() = default;
    virtual void simular() = 0;
    virtual void actualizar() = 0;
    virtual auto interactuar(int posX, int posY, INTERACCIONES accion) -> bool = 0;
    virtual auto getImg() -> IMG* = 0;
    virtual void setIONull(IO* io) = 0;
    virtual void seleccionar(bool estado) = 0;
    [[nodiscard]] virtual auto getIOSalida() -> IO* = 0;
    [[nodiscard]] virtual auto getLinea(int conexion) -> std::pair<int, int>* = 0;
    [[nodiscard]] virtual auto getDesconectado() const -> bool = 0;
    [[nodiscard]] virtual auto getConexion(IO* io) const -> int = 0;

    [[nodiscard]] virtual auto getTipo() const -> TIPOS_SIMULABLES
    {
        if(id & 1 << 31)
            return TIPOS_SIMULABLES::Puerta;
        if(id & 1 << 30)
            return TIPOS_SIMULABLES::Entrada;
        if(id & 1 << 29)
            return TIPOS_SIMULABLES::Salida;
        if(id & 1 << 26)
            return TIPOS_SIMULABLES::Temporizador;

        return TIPOS_SIMULABLES::Nada;
    }


    [[nodiscard]] virtual auto getId() const -> unsigned int { return id; }
};

#endif //SIMULABLE_H
