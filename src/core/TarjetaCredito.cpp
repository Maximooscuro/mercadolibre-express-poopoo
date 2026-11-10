// ============================================================================
// TarjetaCredito.cpp
// Una forma de pago. IMPLEMENTA la interfaz MetodoPago, es decir, esta obligada
// a escribir procesarPago() y getNombreMetodo(). Lo mismo hace
// TransferenciaBancaria, y por eso el pedido puede pagar con cualquiera de las
// dos sin saber cual es (polimorfismo).
// ============================================================================
#include "TarjetaCredito.h"
#include "Excepciones.h"
#include <iostream>
using namespace std;

// Constructor: solo guarda los datos de la tarjeta.
TarjetaCredito::TarjetaCredito(string num, string tit, string venc, string c) {
    this->numeroTarjeta = num;
    this->titular = tit;
    this->fechaVencimiento = venc;
    this->cvv = c;
}

// Procesa el pago. Devuelve true si se aprobo.
// Antes de aprobar revisa las reglas; si alguna falla, lanza una excepcion.
bool TarjetaCredito::procesarPago(float monto) {
    // 1. El monto debe ser positivo.
    if (monto <= 0) {
        throw MontoInvalidoException("El monto del pago debe ser mayor que cero");
    }
    // 2. El numero de tarjeta debe tener exactamente 16 digitos.
    //    size() devuelve cuantos caracteres tiene el string.
    if (this->numeroTarjeta.size() != 16) {
        throw MontoInvalidoException("Pago rechazado: el numero de tarjeta debe tener 16 digitos");
    }
    // 3. El CVV debe tener 3 digitos.
    if (this->cvv.size() != 3) {
        throw MontoInvalidoException("Pago rechazado: el CVV debe tener 3 digitos");
    }
    // Si todo esta bien, simulamos el cobro (aqui no hay banco real).
    cout << "Pago de $" << monto << " aprobado con tarjeta de " << this->titular << endl;
    return true;
}

// Devuelve el nombre de este metodo de pago, para mostrarlo en pantalla.
string TarjetaCredito::getNombreMetodo() const {
    return "Tarjeta de Credito";
}
