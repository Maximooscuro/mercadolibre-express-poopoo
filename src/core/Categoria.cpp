// ============================================================================
// Categoria.cpp
// Una categoria agrupa productos (por ejemplo "Tecnologia").
//
// AGREGACION: la categoria solo guarda PUNTEROS a productos que existen por
// aparte en el catalogo. Si la categoria se borra, los productos NO se borran
// (por eso el destructor esta como "= default" en el .h y no hace delete).
// ============================================================================
#include "Categoria.h"
#include "Excepciones.h"
using namespace std;

// Constructor: guarda el id y el nombre de la categoria.
Categoria::Categoria(string id, string nom) {
    this->idCategoria = id;
    this->nombre = nom;
}

// Agrega un producto a la categoria (solo guarda el puntero).
void Categoria::agregarProducto(Producto* prod) {
    // Un puntero nulo no apunta a ningun producto, no tiene sentido guardarlo.
    if (prod == nullptr) {
        throw MontoInvalidoException("No se puede agregar un producto nulo");
    }
    this->productos.push_back(prod);
}

// Quita un producto de la categoria buscandolo por su codigo.
void Categoria::removerProducto(const string& codigo) {
    for (size_t i = 0; i < this->productos.size(); i++) {
        if (this->productos[i]->getCodigo() == codigo) {
            // erase quita el puntero de la lista. El producto NO se borra
            // (no hacemos delete) porque la categoria no es su dueña.
            this->productos.erase(this->productos.begin() + i);
            return;   // ya lo encontramos, salimos
        }
    }
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
string Categoria::getIdCategoria() const { return this->idCategoria; }
string Categoria::getNombre() const { return this->nombre; }
const vector<Producto*>& Categoria::getProductos() const { return this->productos; }
