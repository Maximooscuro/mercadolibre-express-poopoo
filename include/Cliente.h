//
// Created by maxim on 30/10/2026.
//

#pragma once
#include <string>

class Cliente {
private:
    std::string nombre;
    std::string correo;
    std::string direccionEnvio;
    float saldoDisponible;

public:
    Cliente(std::string nom, std::string cor, std::string dir, float saldo);
    ~Cliente() = default;

    void descontarSaldo(float monto);
    void recargarSaldo(float monto);

    std::string getNombre() const;
    std::string getCorreo() const;
    std::string getDireccionEnvio() const;
    float getSaldoDisponible() const;
};
