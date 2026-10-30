//
// Created by maxim on 30/10/2026.
//

#pragma once
#include <string>

class MetodoPago {
public:
    virtual ~MetodoPago() = default;

    // Operación virtual pura obligatoria
    virtual bool procesarPago(float monto) = 0;
    virtual std::string getNombreMetodo() const = 0;
};
