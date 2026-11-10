// ============================================================================
// Pedido.cpp
// Una compra. Tiene numero de seguimiento, fecha, estado, monto total y una
// lista de DetallePedido.
//
// COMPOSICION: el Pedido crea sus detalles con "new" y es su dueno. Por eso en
// el destructor los borra con "delete". Si el pedido desaparece, sus detalles
// tambien (si no los borraramos quedaria memoria perdida: "memory leak").
// ============================================================================
#include "Pedido.h"
#include "Excepciones.h"
using namespace std;

// Lista de numeros de seguimiento que ya se usaron. Es "static" a nivel de
// archivo: solo se ve dentro de Pedido.cpp y existe una sola para todos los
// pedidos. Sirve para detectar numeros repetidos.
static vector<string> numerosUsados;

// Constructor: crea un pedido vacio y "Pendiente".
Pedido::Pedido(string numSeg, string fch) {
    // Recorremos la lista de numeros usados; si encontramos el mismo, error.
    for (size_t i = 0; i < numerosUsados.size(); i++) {
        if (numerosUsados[i] == numSeg) {
            throw SeguimientoDuplicadoException("El numero de seguimiento " + numSeg + " ya existe");
        }
    }
    numerosUsados.push_back(numSeg);   // lo anotamos como usado
    this->numeroSeguimiento = numSeg;
    this->fecha = fch;
    this->estado = "Pendiente";        // todo pedido nuevo empieza asi
    this->montoTotal = 0;
}

// Destructor: se ejecuta solo cuando el pedido se borra.
// Recorre el vector y hace delete de cada DetallePedido (los creamos con new).
Pedido::~Pedido() {
    for (size_t i = 0; i < this->detalles.size(); i++) {
        delete this->detalles[i];
    }
    this->detalles.clear();   // deja el vector vacio
}

// Agrega un producto al pedido (una nueva linea).
void Pedido::agregarDetalle(const Producto* prod, int cant) {
    // "new" crea el detalle en memoria dinamica. Si el constructor lanza una
    // excepcion (sin stock, cantidad mala...) no se crea nada y el error sube.
    DetallePedido* d = new DetallePedido(prod, cant);
    this->detalles.push_back(d);   // lo guardamos en el vector
    calcularTotal();               // actualizamos el total
}

// Suma los subtotales de todos los detalles.
void Pedido::calcularTotal() {
    this->montoTotal = 0;
    for (size_t i = 0; i < this->detalles.size(); i++) {
        this->montoTotal += this->detalles[i]->getSubtotal();
    }
}

// Paga el pedido. Recibe el metodo de pago por REFERENCIA (MetodoPago&): puede
// ser tarjeta o transferencia, no importa cual (polimorfismo).
// Tambien recibe el servicio de notificacion para avisar al cliente
// (dependencia: lo usa solo en este momento).
bool Pedido::procesarOrden(MetodoPago& metodo, ServicioNotificacion& servicio, const string& correoCliente) {
    // No se puede pagar un pedido sin productos.
    if (this->detalles.empty()) {
        throw MontoInvalidoException("El pedido no tiene productos");
    }
    // Llamamos al pago. Cada metodo lo hace a su manera.
    bool aprobado = metodo.procesarPago(this->montoTotal);
    if (!aprobado) {
        this->estado = "Rechazado";
        return false;
    }
    // Pago aprobado: cambiamos el estado y mandamos el correo.
    this->estado = "Pagado";
    servicio.enviarConfirmacion(correoCliente, this->numeroSeguimiento, this->montoTotal);
    return true;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
string Pedido::getNumeroSeguimiento() const { return this->numeroSeguimiento; }
string Pedido::getEstado() const { return this->estado; }
float Pedido::getMontoTotal() const { return this->montoTotal; }
// Devuelve el vector por referencia constante: se puede leer pero no modificar.
const vector<DetallePedido*>& Pedido::getDetalles() const { return this->detalles; }
