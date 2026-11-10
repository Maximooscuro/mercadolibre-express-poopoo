#include "Pedido.h"
#include "Excepciones.h"
using namespace std;

// Lista de numeros de seguimiento ya usados (solo visible en este archivo)
static vector<string> numerosUsados;

Pedido::Pedido(string numSeg, string fch) {
    for (size_t i = 0; i < numerosUsados.size(); i++) {
        if (numerosUsados[i] == numSeg) {
            throw SeguimientoDuplicadoException("El numero de seguimiento " + numSeg + " ya existe");
        }
    }
    numerosUsados.push_back(numSeg);
    this->numeroSeguimiento = numSeg;
    this->fecha = fch;
    this->estado = "Pendiente";
    this->montoTotal = 0;
}

// Composicion: el pedido destruye sus detalles
Pedido::~Pedido() {
    for (size_t i = 0; i < this->detalles.size(); i++) {
        delete this->detalles[i];
    }
    this->detalles.clear();
}

void Pedido::agregarDetalle(const Producto* prod, int cant) {
    DetallePedido* d = new DetallePedido(prod, cant);   // si lanza excepcion, no se crea nada
    this->detalles.push_back(d);
    calcularTotal();
}

void Pedido::calcularTotal() {
    this->montoTotal = 0;
    for (size_t i = 0; i < this->detalles.size(); i++) {
        this->montoTotal += this->detalles[i]->getSubtotal();
    }
}

bool Pedido::procesarOrden(MetodoPago& metodo, ServicioNotificacion& servicio, const string& correoCliente) {
    if (this->detalles.empty()) {
        throw MontoInvalidoException("El pedido no tiene productos");
    }
    bool aprobado = metodo.procesarPago(this->montoTotal);
    if (!aprobado) {
        this->estado = "Rechazado";
        return false;
    }
    this->estado = "Pagado";
    servicio.enviarConfirmacion(correoCliente, this->numeroSeguimiento, this->montoTotal);
    return true;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
string Pedido::getNumeroSeguimiento() const { return this->numeroSeguimiento; }
string Pedido::getEstado() const { return this->estado; }
float Pedido::getMontoTotal() const { return this->montoTotal; }
const vector<DetallePedido*>& Pedido::getDetalles() const { return this->detalles; }
