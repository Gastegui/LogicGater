//
// Created by julen on 9/01/25.
//

#ifndef SIMULABLE_H
#define SIMULABLE_H

class Simulable
{
    unsigned int id{0};
public:

    explicit Simulable(const unsigned int id) : id(id) {}

    virtual ~Simulable() = default;
    virtual void simular() = 0;
    virtual void actualizar() = 0;

    [[nodiscard]] virtual unsigned int getId() const { return id; }
};

#endif //SIMULABLE_H
