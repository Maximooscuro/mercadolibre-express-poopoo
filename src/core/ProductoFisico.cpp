#include "ProductoFisico.h"
#include "Excepciones.h"
using namespace std;

ProductoFisico::ProductoFisico(string cod, string nom, string desc, float prec,
                               int stk, float peso, string dim)
    : Producto(cod, nom, desc, prec, stk) {
    if (peso <= 0) {
        throw MontoInvalidoException("El peso debe ser mayor que cero");
    }
    this->peso = peso;
    this->dimensiones = dim;
}

// Envio: 5000 de base + 1000 por cada kilo
float ProductoFisico::calcularCostoEnvio() const {
    return 5000 + (this->peso * 1000);
}

float ProductoFisico::calcularCostoTotal(int cantidad) const {
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    return (this->precio * cantidad) + calcularCostoEnvio();
}

void ProductoFisico::mostrarDetalles() const {
    cout << *this << endl;
    cout << "  Tipo: Fisico | Peso: " << this->peso << " kg | Dimensiones: "
         << this->dimensiones << " | Envio: $" << calcularCostoEnvio() << endl;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
float ProductoFisico::getPeso() const { return this->peso; }
string ProductoFisico::getDimensiones() const { return this->dimensiones; }
