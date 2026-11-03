#include "Cliente.h"
#include "Excepciones.h"
using namespace std;

Cliente::Cliente(string nom, string cor, string dir, float saldo) {
    this->nombre = nom;
    this->correo = cor;
    this->direccionEnvio = dir;
    this->saldoDisponible = saldo;
}

void Cliente::descontarSaldo(float monto) {
    if (monto <= 0) {
        throw MontoInvalidoException("El monto a descontar debe ser mayor que cero");
    }
    if (monto > this->saldoDisponible) {
        throw SaldoInsuficienteException("Saldo insuficiente para realizar la compra");
    }
    this->saldoDisponible -= monto;
}

void Cliente::recargarSaldo(float monto) {
    if (monto <= 0) {
        throw MontoInvalidoException("El monto a recargar debe ser mayor que cero");
    }
    this->saldoDisponible += monto;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
string Cliente::getNombre() const { return this->nombre; }
string Cliente::getCorreo() const { return this->correo; }
string Cliente::getDireccionEnvio() const { return this->direccionEnvio; }
float Cliente::getSaldoDisponible() const { return this->saldoDisponible; }
