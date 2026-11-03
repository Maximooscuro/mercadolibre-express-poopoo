#include "DetallePedido.h"
#include "Excepciones.h"
using namespace std;

DetallePedido::DetallePedido(const Producto* prod, int cant) {
    if (prod == nullptr) {
        throw MontoInvalidoException("El detalle necesita un producto");
    }
    if (cant <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    if (cant > prod->getStock()) {
        throw StockAgotadoException("Stock insuficiente para " + prod->getNombre());
    }
    this->producto = prod;   // solo lo apunta, no es dueno del producto
    this->cantidad = cant;
    calcularSubtotal();
}

void DetallePedido::calcularSubtotal() {
    // polimorfismo: cada tipo de producto calcula distinto
    this->subtotal = this->producto->calcularCostoTotal(this->cantidad);
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
float DetallePedido::getSubtotal() const { return this->subtotal; }
int DetallePedido::getCantidad() const { return this->cantidad; }
const Producto* DetallePedido::getProducto() const { return this->producto; }
