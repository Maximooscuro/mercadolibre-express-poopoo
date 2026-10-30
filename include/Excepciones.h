//
// Created by maxim on 30/10/2026.
//

#pragma once
#include <stdexcept>
#include <string>

// Si np hay billullo que salte:
class SaldoInsuficienteException : public std::runtime_error {
public:
    explicit SaldoInsuficienteException(const std::string& msg = "Error: Saldo insuficiente para procesar la transaccion.")
        : std::runtime_error(msg) {}
};

// Lanzada cuando la cantidad solicitada supera las existencias del catalogo
class StockAgotadoException : public std::runtime_error {
public:
    explicit StockAgotadoException(const std::string& msg = "Error: Stock agotado o insuficiente para el producto.")
        : std::runtime_error(msg) {}
};

// Lanzada si se intenta registrar un pedido con un tracking que ya existe
class SeguimientoDuplicadoException : public std::runtime_error {
public:
    explicit SeguimientoDuplicadoException(const std::string& msg = "Error: El numero de seguimiento ya esta registrado.")
        : std::runtime_error(msg) {}
};

// Lanzada si el monto a pagar es menor o igual a cero
class MontoInvalidoException : public std::runtime_error {
public:
    explicit MontoInvalidoException(const std::string& msg = "Error: El monto de pago ingresado no es valido.")
        : std::runtime_error(msg) {}
};
