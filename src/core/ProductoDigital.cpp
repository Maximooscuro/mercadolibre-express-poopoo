// ============================================================================
// ProductoDigital.cpp
// Un producto digital (un curso en video, un ebook, un software...).
// HEREDA de Producto. Lo propio: tamano del archivo y URL de descarga.
// A diferencia del fisico, NO tiene costo de envio porque se descarga.
// ============================================================================
#include "ProductoDigital.h"
#include "Excepciones.h"
using namespace std;

// Constructor: primero llama al del padre (Producto) y luego guarda lo suyo.
ProductoDigital::ProductoDigital(string cod, string nom, string desc, float prec,
                                 int stk, float tam, string url)
    : Producto(cod, nom, desc, prec, stk) {
    // El archivo tiene que tener un tamano valido.
    if (tam <= 0) {
        throw MontoInvalidoException("El tamano del archivo debe ser mayor que cero");
    }
    this->tamanoArchivo = tam;
    this->urlDescarga = url;
}

// POLIMORFISMO: version del producto digital. Es distinta a la del fisico:
// aqui NO se suma envio, el total es solo precio por cantidad.
float ProductoDigital::calcularCostoTotal(int cantidad) const {
    if (cantidad <= 0) {
        throw MontoInvalidoException("La cantidad debe ser mayor que cero");
    }
    return this->precio * cantidad;
}

// Muestra los datos. Igual que en el fisico, usa el operator<< del padre.
void ProductoDigital::mostrarDetalles() const {
    cout << *this << endl;
    cout << "  Tipo: Digital | Tamano: " << this->tamanoArchivo << " MB | URL: "
         << this->urlDescarga << endl;
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
float ProductoDigital::getTamanoArchivo() const { return this->tamanoArchivo; }
string ProductoDigital::getUrlDescarga() const { return this->urlDescarga; }
