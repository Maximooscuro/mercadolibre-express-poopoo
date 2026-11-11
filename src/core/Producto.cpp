#include "Producto.h"
#include "Excepciones.h"
using namespace std;

Producto::Producto(string cod, string nom, string desc, float prec, int stk) {
    if (prec < 0) {
        throw MontoInvalidoException("El precio no puede ser negativo");
    }
    this->codigo = cod;
    this->nombre = nom;
    this->descripcion = desc;
    this->precio = prec;
    this->stock = stk;
}

void Producto::reducirStock(int cantidad) {
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    if (cantidad > this->stock) {
        throw StockAgotadoException("Stock insuficiente para el producto " + this->nombre);
    }
    this->stock -= cantidad;
}

void Producto::aumentarStock(int cantidad) {
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    this->stock += cantidad;
}

// Sobrecarga del operador <<
ostream& operator<<(ostream& os, const Producto& p) {
    os << "[" << p.codigo << "] " << p.nombre << " - $" << p.precio
       << " (stock: " << p.stock << ")";
    return os;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo { return ...; }) ----
string Producto::getCodigo() const { return this->codigo; }
string Producto::getNombre() const { return this->nombre; }
string Producto::getDescripcion() const { return this->descripcion; }
float Producto::getPrecio() const { return this->precio; }
int Producto::getStock() const { return this->stock; }
