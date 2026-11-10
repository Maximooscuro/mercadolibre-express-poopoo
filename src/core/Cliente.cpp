// ============================================================================
// Cliente.cpp
// El comprador. Tiene nombre, correo, direccion de envio y saldo disponible.
// Aqui estan las reglas del saldo: no se puede descontar mas de lo que tiene.
// ============================================================================
#include "Cliente.h"
#include "Excepciones.h"
using namespace std;

// Constructor: guarda los datos del cliente.
Cliente::Cliente(string nom, string cor, string dir, float saldo) {
    this->nombre = nom;
    this->correo = cor;
    this->direccionEnvio = dir;
    this->saldoDisponible = saldo;
}

// Resta plata del saldo (por ejemplo al pagar una compra).
void Cliente::descontarSaldo(float monto) {
    // El monto a descontar debe ser positivo.
    if (monto <= 0) {
        throw MontoInvalidoException("El monto a descontar debe ser mayor que cero");
    }
    // Si el monto es mayor que lo que tiene, no alcanza.
    if (monto > this->saldoDisponible) {
        throw SaldoInsuficienteException("Saldo insuficiente para realizar la compra");
    }
    this->saldoDisponible -= monto;
}

// Suma plata al saldo (recarga).
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
