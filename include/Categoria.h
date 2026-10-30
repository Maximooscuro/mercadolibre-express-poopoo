//
// Created by maxim on 30/10/2026.
//

#pragma once
#include <string>
#include <vector>
#include "Producto.h"

class Categoria {
private:
    std::string idCategoria;
    std::string nombre;

    // Agregación: almacena punteros a productos que existen fuera de ella
    std::vector<Producto*> productos;

public:
    Categoria(std::string id, std::string nom);
    ~Categoria() = default; // No borra los Producto* porque solo los referencia

    void agregarProducto(Producto* prod);
    void removerProducto(const std::string& codigo);

    std::string getIdCategoria() const;
    std::string getNombre() const;
    const std::vector<Producto*>& getProductos() const;
};
