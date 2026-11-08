#pragma once
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <functional>

class PantallaPago : public QWidget {
public:
    explicit PantallaPago(std::function<void()> alPagado, QWidget* parent = nullptr);
    void setTotal(double nuevoTotal);

private:
    void pagar();

    QComboBox* metodo;
    QLabel* etiquetaTotal;
    double total = 0;
    std::function<void()> alPagado;
};
