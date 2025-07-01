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
    Simulable(const Simulable&) = delete;
    auto operator=(const Simulable&) -> Simulable& = delete;
    Simulable(Simulable&&) = delete;
    auto operator=(Simulable&&) -> Simulable& = delete;

    virtual auto simular() -> void = 0;
    virtual auto actualizar() -> void = 0;
    virtual auto interactuar(int posX, int posY, INTERACCIONES accion) -> bool = 0;
    virtual auto getImg() -> IMG* = 0;
    // ReSharper disable once CppInconsistentNaming
    virtual auto setIONull(IO* io) -> void = 0;
    virtual auto seleccionar(bool estado) -> void = 0;
    // ReSharper disable once CppInconsistentNaming
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
