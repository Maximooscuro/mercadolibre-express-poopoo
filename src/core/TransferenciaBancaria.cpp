// ============================================================================
// TransferenciaBancaria.cpp
// Otra forma de pago. Tambien IMPLEMENTA la interfaz MetodoPago, igual que
// TarjetaCredito, pero con sus propias validaciones.
// ============================================================================
#include "TransferenciaBancaria.h"
#include "Excepciones.h"
#include <iostream>
using namespace std;

// Constructor: guarda el banco y el numero de cuenta.
TransferenciaBancaria::TransferenciaBancaria(string bco, string cta) {
    this->banco = bco;
    this->numeroCuenta = cta;
}

// Procesa la transferencia. Devuelve true si se aprobo.
bool TransferenciaBancaria::procesarPago(float monto) {
    // El monto debe ser positivo.
    if (monto <= 0) {
        throw MontoInvalidoException("El monto del pago debe ser mayor que cero");
    }
    // Hacen falta los datos de la cuenta (empty() es true si el string esta vacio).
    if (this->banco.empty() || this->numeroCuenta.empty()) {
        throw MontoInvalidoException("Pago rechazado: faltan los datos de la cuenta bancaria");
    }
    // Simulamos la transferencia.
    cout << "Transferencia de $" << monto << " realizada desde " << this->banco << endl;
    return true;
}

// Nombre del metodo de pago.
string TransferenciaBancaria::getNombreMetodo() const {
    return "Transferencia Bancaria";
}
