#include "ProductoDigital.h"
#include "Excepciones.h"
using namespace std;

ProductoDigital::ProductoDigital(string cod, string nom, string desc, float prec,
                                 int stk, float tam, string url)
    : Producto(cod, nom, desc, prec, stk) {
    if (tam <= 0) {
        throw MontoInvalidoException("El tamano del archivo debe ser mayor que cero");
    }
    this->tamanoArchivo = tam;
    this->urlDescarga = url;
}

// Un producto digital no tiene costo de envio
float ProductoDigital::calcularCostoTotal(int cantidad) const {
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    return this->precio * cantidad;
}

void ProductoDigital::mostrarDetalles() const {
    cout << *this << endl;
    cout << "  Tipo: Digital | Tamano: " << this->tamanoArchivo << " MB | URL: "
         << this->urlDescarga << endl;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
float ProductoDigital::getTamanoArchivo() const { return this->tamanoArchivo; }
string ProductoDigital::getUrlDescarga() const { return this->urlDescarga; }
