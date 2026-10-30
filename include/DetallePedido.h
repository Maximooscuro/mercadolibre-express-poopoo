//
// Created by maxim on 30/10/2026.
//

#pragma once
#include "Producto.h"

class DetallePedido {
private:
    const Producto* producto; // Referencia al producto comprado
    int cantidad;
    float subtotal;

public:
    DetallePedido(const Producto* prod, int cant);
    ~DetallePedido() = default;

    void calcularSubtotal();
    float getSubtotal() const;
    int getCantidad() const;
    const Producto* getProducto() const;
};
