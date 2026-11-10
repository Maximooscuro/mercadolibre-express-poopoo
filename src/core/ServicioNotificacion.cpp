// ============================================================================
// ServicioNotificacion.cpp
// Simula el envio de un correo de confirmacion al cliente.
// El Pedido lo usa solo cuando el pago sale bien (DEPENDENCIA: una relacion
// puntual y temporal, el pedido no lo guarda como atributo).
// ============================================================================
#include "ServicioNotificacion.h"
#include <iostream>
using namespace std;

// No enviamos un correo real: solo imprimimos el mensaje en consola.
void ServicioNotificacion::enviarConfirmacion(const string& destinatario, const string& tracking, float total) {
    cout << "Correo enviado a " << destinatario << ": su pedido " << tracking
         << " por $" << total << " fue confirmado." << endl;
}
