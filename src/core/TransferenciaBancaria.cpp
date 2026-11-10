#include "TransferenciaBancaria.h"
#include "Excepciones.h"
#include <iostream>
using namespace std;

TransferenciaBancaria::TransferenciaBancaria(string bco, string cta) {
    this->banco = bco;
    this->numeroCuenta = cta;
}

bool TransferenciaBancaria::procesarPago(float monto) {
    if (monto <= 0) {
        throw MontoInvalidoException("El monto del pago debe ser mayor que cero");
    }
    if (this->banco.empty() || this->numeroCuenta.empty()) {
        throw MontoInvalidoException("Pago rechazado: faltan los datos de la cuenta bancaria");
    }
    cout << "Transferencia de $" << monto << " realizada desde " << this->banco << endl;
    return true;
}

string TransferenciaBancaria::getNombreMetodo() const {
    return "Transferencia Bancaria";
}
