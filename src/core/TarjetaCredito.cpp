#include "TarjetaCredito.h"
#include "Excepciones.h"
#include <iostream>
using namespace std;

TarjetaCredito::TarjetaCredito(string num, string tit, string venc, string c) {
    this->numeroTarjeta = num;
    this->titular = tit;
    this->fechaVencimiento = venc;
    this->cvv = c;
}

bool TarjetaCredito::procesarPago(float monto) {
    if (monto <= 0) {
        throw MontoInvalidoException("El monto del pago debe ser mayor que cero");
    }
    if (this->numeroTarjeta.size() != 16) {
        throw MontoInvalidoException("Pago rechazado: el numero de tarjeta debe tener 16 digitos");
    }
    if (this->cvv.size() != 3) {
        throw MontoInvalidoException("Pago rechazado: el CVV debe tener 3 digitos");
    }
    cout << "Pago de $" << monto << " aprobado con tarjeta de " << this->titular << endl;
    return true;
}

string TarjetaCredito::getNombreMetodo() const {
    return "Tarjeta de Credito";
}
