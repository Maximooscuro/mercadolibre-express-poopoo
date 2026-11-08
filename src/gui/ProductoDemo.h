#pragma once
// Datos de ejemplo para la GUI. Se reemplazan por Producto*/Pedido cuando el Backend termine.
#include <QString>

struct ProductoDemo {
    QString codigo;
    QString nombre;
    QString tipo;
    double precio;
    int stock;
};

struct ItemCarrito {
    ProductoDemo producto;
    int cantidad;
};
