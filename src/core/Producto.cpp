// ============================================================================
// Producto.cpp
// Implementacion de la clase base ABSTRACTA Producto.
//
// Producto es la "plantilla" de todos los productos de la tienda. Guarda lo
// que tienen en comun (codigo, nombre, descripcion, precio, stock). Es abstracta
// porque tiene metodos virtuales puros (mostrarDetalles y calcularCostoTotal),
// que NO se implementan aqui: los implementan las hijas (ProductoFisico y
// ProductoDigital). Por eso nunca se puede hacer "new Producto(...)".
// ============================================================================
#include "Producto.h"
#include "Excepciones.h"   // para poder usar throw con nuestras excepciones
using namespace std;

// Constructor: recibe los datos y los guarda en los atributos.
// Usamos "this->" para diferenciar el atributo de la clase del parametro
// (por ejemplo this->precio es el atributo y prec es lo que llega).
Producto::Producto(string cod, string nom, string desc, float prec, int stk) {
    // Regla de negocio: un precio negativo no tiene sentido.
    // throw "lanza" la excepcion y el constructor se detiene: el objeto no se crea.
    if (prec < 0) {
        throw MontoInvalidoException("El precio no puede ser negativo");
    }
    this->codigo = cod;
    this->nombre = nom;
    this->descripcion = desc;
    this->precio = prec;
    this->stock = stk;
}

// Quita unidades del inventario (por ejemplo cuando se vende algo).
void Producto::reducirStock(int cantidad) {
    // No se puede reducir 0 ni una cantidad negativa.
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    // Si piden mas de lo que hay, no se puede: stock agotado.
    if (cantidad > this->stock) {
        throw StockAgotadoException("Stock insuficiente para el producto " + this->nombre);
    }
    // Si pasa las dos validaciones, ahora si se resta.
    this->stock -= cantidad;
}

// Agrega unidades al inventario (por ejemplo cuando llega mercancia).
void Producto::aumentarStock(int cantidad) {
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    this->stock += cantidad;
}

// SOBRECARGA DEL OPERADOR <<
// Permite escribir:  cout << producto;  y que salga algo legible.
// Es "friend" (amiga) en el .h, por eso puede leer los atributos protegidos.
// Recibe el flujo de salida (os) y el producto, y devuelve el mismo flujo
// para poder encadenar:  cout << p1 << p2;
ostream& operator<<(ostream& os, const Producto& p) {
    os << "[" << p.codigo << "] " << p.nombre << " - $" << p.precio
       << " (stock: " << p.stock << ")";
    return os;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo { return ...; }) ----
// Un getter solo devuelve el valor de un atributo privado/protegido.
// Llevan "const" porque no modifican el objeto.
string Producto::getCodigo() const { return this->codigo; }
string Producto::getNombre() const { return this->nombre; }
string Producto::getDescripcion() const { return this->descripcion; }
float Producto::getPrecio() const { return this->precio; }
int Producto::getStock() const { return this->stock; }
