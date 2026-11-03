#include "Categoria.h"
#include "Excepciones.h"
using namespace std;

Categoria::Categoria(string id, string nom) {
    this->idCategoria = id;
    this->nombre = nom;
}

// Agregacion: la categoria solo guarda punteros, nunca borra los productos
void Categoria::agregarProducto(Producto* prod) {
    if (prod == nullptr) {
        throw MontoInvalidoException("No se puede agregar un producto nulo");
    }
    this->productos.push_back(prod);
}

void Categoria::removerProducto(const string& codigo) {
    for (size_t i = 0; i < this->productos.size(); i++) {
        if (this->productos[i]->getCodigo() == codigo) {
            this->productos.erase(this->productos.begin() + i);   // quita el puntero, no el producto
            return;
        }
    }
}

// ---- GETTERS (borrar esta seccion si el .h ya los trae con cuerpo) ----
string Categoria::getIdCategoria() const { return this->idCategoria; }
string Categoria::getNombre() const { return this->nombre; }
const vector<Producto*>& Categoria::getProductos() const { return this->productos; }
