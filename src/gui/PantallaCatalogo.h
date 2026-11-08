#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <functional>
#include <vector>
#include "ProductoDemo.h"

class PantallaCatalogo : public QWidget {
public:
    using Callback = std::function<bool(const ProductoDemo&, int)>;
    explicit PantallaCatalogo(Callback alAgregar, QWidget* parent = nullptr);
    void descontarStock(const std::vector<ItemCarrito>& items);

private:
    void agregar();

    std::vector<ProductoDemo> datos;
    QTableWidget* tabla;
    QSpinBox* cantidad;
    QLabel* estado;
    Callback alAgregar;
};