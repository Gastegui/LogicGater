//
// Created by julen on 9/01/25.
//

#ifndef SIMULABLE_H
#define SIMULABLE_H
#include "../controles.h"
#include "../enums.h"

class Simulable
{
    unsigned int id{0};
public:
    
    explicit Simulable(const unsigned int id) : id(id) {}

    virtual ~Simulable() = default;
    virtual void simular() = 0;
    virtual void actualizar() = 0;
    virtual bool interactuar(int posX, int posY, INTERACCIONES accion) = 0;
    virtual IMG* getImg() = 0;
    virtual void setIONull(IO* io) = 0;
    [[nodiscard]] virtual IO* getIOSalida() = 0;
    [[nodiscard]] virtual std::pair<int, int>* getLinea(IO* io) = 0;

    [[nodiscard]] virtual TIPOS_SIMULABLES getTipo() const
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


    [[nodiscard]] virtual unsigned int getId() const { return id; }
};

#endif //SIMULABLE_H
