#include "ServicioNotificacion.h"
#include <iostream>
using namespace std;

// Simulacion del envio de correo
void ServicioNotificacion::enviarConfirmacion(const string& destinatario, const string& tracking, float total) {
    cout << "Correo enviado a " << destinatario << ": su pedido " << tracking
         << " por $" << total << " fue confirmado." << endl;
}
