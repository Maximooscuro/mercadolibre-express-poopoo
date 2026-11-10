// ============================================================================
// ProductoFisico.cpp
// Un producto fisico (audifonos, un libro, una nevera...). HEREDA de Producto,
// o sea que ya tiene codigo, nombre, precio, stock, etc. Aqui solo agregamos
// lo propio: peso, dimensiones y el costo de envio.
// ============================================================================
#include "ProductoFisico.h"
#include "Excepciones.h"
using namespace std;

// Constructor. Despues de los ":" llamamos al constructor de la clase padre
// (Producto) para que guarde los datos comunes. Luego guardamos los propios.
ProductoFisico::ProductoFisico(string cod, string nom, string desc, float prec,
                               int stk, float peso, string dim)
    : Producto(cod, nom, desc, prec, stk) {
    // Un producto fisico tiene que pesar algo.
    if (peso <= 0) {
        throw MontoInvalidoException("El peso debe ser mayor que cero");
    }
    this->peso = peso;
    this->dimensiones = dim;
}

// Costo de envio: 5000 de base + 1000 por cada kilo.
// Ejemplo: 0.3 kg -> 5000 + 300 = 5300.
// Es const porque solo calcula, no modifica el producto.
float ProductoFisico::calcularCostoEnvio() const {
    return 5000 + (this->peso * 1000);
}

// POLIMORFISMO: este metodo es la version del producto fisico.
// "override" (en el .h) indica que reescribimos el metodo virtual del padre.
// Costo total = precio por la cantidad + el envio.
float ProductoFisico::calcularCostoTotal(int cantidad) const {
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    return (this->precio * cantidad) + calcularCostoEnvio();
}

// Muestra los datos en consola.
// "*this" es el objeto actual; al usar cout << *this se llama al operator<<
// que hicimos en Producto.cpp (imprime codigo, nombre, precio y stock).
void ProductoFisico::mostrarDetalles() const {
    cout << *this << endl;
    cout << "  Tipo: Fisico | Peso: " << this->peso << " kg | Dimensiones: "
         << this->dimensiones << " | Envio: $" << calcularCostoEnvio() << endl;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
float ProductoFisico::getPeso() const { return this->peso; }
string ProductoFisico::getDimensiones() const { return this->dimensiones; }
