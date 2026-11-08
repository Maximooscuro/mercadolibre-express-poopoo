#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLabel>
#include <functional>
#include <vector>
#include "ProductoDemo.h"

class PantallaCompra : public QWidget {
public:
    PantallaCompra(std::function<void(int)> alQuitar,
                   std::function<void()> alIrAPagar,
                   QWidget* parent = nullptr);
    void actualizar(const std::vector<ItemCarrito>& carrito);

private:
    QTableWidget* tabla;
    QLabel* total;
    std::function<void(int)> alQuitar;
    std::function<void()> alIrAPagar;
};
