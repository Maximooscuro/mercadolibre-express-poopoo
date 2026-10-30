//
// Created by maxim on 30/10/2026.
//

#pragma once
#include "MetodoPago.h"
#include <string>

class TarjetaCredito : public MetodoPago {
private:
    std::string numeroTarjeta;
    std::string titular;
    std::string fechaVencimiento;
    std::string cvv;

public:
    TarjetaCredito(std::string num, std::string tit, std::string venc, std::string c);
    ~TarjetaCredito() override = default;

    bool procesarPago(float monto) override;
    std::string getNombreMetodo() const override;
};
