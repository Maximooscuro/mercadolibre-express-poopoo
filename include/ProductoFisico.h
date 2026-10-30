//
// Created by maxim on 30/10/2026.
//

#pragma once
#include "Producto.h"

class ProductoFisico : public Producto {
private:
    float peso;            // En kilogramos
    std::string dimensiones; // Formato "LxAxP"

public:
    ProductoFisico(std::string cod, std::string nom, std::string desc, float prec, int stk,
                   float peso, std::string dim);
    ~ProductoFisico() override = default;

    float calcularCostoEnvio() const;
    float calcularCostoTotal(int cantidad) const override;
    void mostrarDetalles() const override;

    float getPeso() const;
    std::string getDimensiones() const;
};