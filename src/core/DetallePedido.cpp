// ============================================================================
// DetallePedido.cpp
// Una "linea" del pedido: que producto se compro y cuantas unidades.
// Ejemplo: "2 audifonos". Un pedido tiene varios detalles (composicion: el
// Pedido es el dueno de sus detalles y los destruye).
//
// OJO: el detalle NO es dueno del producto. Solo guarda un puntero a el
// (const Producto*) porque el producto pertenece al catalogo.
// ============================================================================
#include "DetallePedido.h"
#include "Excepciones.h"
using namespace std;

// Constructor: valida los datos y calcula el subtotal.
DetallePedido::DetallePedido(const Producto* prod, int cant) {
    // Si el puntero es nulo (no apunta a nada) no hay producto que comprar.
    if (prod == nullptr) {
        throw MontoInvalidoException("El detalle necesita un producto");
    }
    // La cantidad tiene que ser positiva.
    if (cant <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    // No se puede pedir mas de lo que hay en inventario.
    if (cant > prod->getStock()) {
        throw StockAgotadoException("Stock insuficiente para " + prod->getNombre());
    }
    this->producto = prod;   // solo apunta al producto, no lo copia
    this->cantidad = cant;
    calcularSubtotal();      // deja el subtotal listo apenas se crea el detalle
}

// Calcula cuanto cuesta esta linea.
// POLIMORFISMO: "producto" es un puntero a Producto, pero C++ ejecuta la version
// del tipo real (fisico suma envio, digital no) sin que tengamos que preguntar
// de que tipo es.
void DetallePedido::calcularSubtotal() {
    this->subtotal = this->producto->calcularCostoTotal(this->cantidad);
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
float DetallePedido::getSubtotal() const { return this->subtotal; }
int DetallePedido::getCantidad() const { return this->cantidad; }
const Producto* DetallePedido::getProducto() const { return this->producto; }
