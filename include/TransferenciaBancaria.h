//
// Created by maxim on 30/10/2026.
//

#pragma once
#include "MetodoPago.h"
#include <string>

class TransferenciaBancaria : public MetodoPago {
private:
    std::string banco;
    std::string numeroCuenta;

public:
    TransferenciaBancaria(std::string bco, std::string cta);
    ~TransferenciaBancaria() override = default;

    bool procesarPago(float monto) override;
    std::string getNombreMetodo() const override;
};
