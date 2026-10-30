//
// Created by maxim on 30/10/2026.
//

#pragma once
#include "Producto.h"

class ProductoDigital : public Producto {
private:
    float tamanoArchivo;   // En Megabytes (MB)
    std::string urlDescarga;

public:
    ProductoDigital(std::string cod, std::string nom, std::string desc, float prec, int stk,
                    float tam, std::string url);
    ~ProductoDigital() override = default;

    float calcularCostoTotal(int cantidad) const override;
    void mostrarDetalles() const override;

    float getTamanoArchivo() const;
    std::string getUrlDescarga() const;
};
