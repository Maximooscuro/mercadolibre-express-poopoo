//
// Created by maxim on 30/10/2026.
//
#pragma once
#include <string>
#include <iostream>

class Producto {
protected:
    std::string codigo;
    std::string nombre;
    std::string descripcion;
    float precio;
    int stock;

public:
    Producto(std::string cod, std::string nom, std::string desc, float prec, int stk);
    virtual ~Producto() = default; // Destructor virtual para asegurar limpieza polimórfica

    // Métodos virtuales puros (polimorfismo estricto)
    virtual void mostrarDetalles() const = 0;
    virtual float calcularCostoTotal(int cantidad) const = 0;

    // Getters y administración de inventario
    std::string getCodigo() const;
    std::string getNombre() const;
    std::string getDescripcion() const;
    float getPrecio() const;
    int getStock() const;
    void reducirStock(int cantidad);
    void aumentarStock(int cantidad);

    // Sobrecarga de operador << para impresión en consola o reportes
    friend std::ostream& operator<<(std::ostream& os, const Producto& p);
};