#pragma once
#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QLineEdit>
#include <functional>
#include "ModeloGui.h"

class PantallaPago : public QWidget {
public:
    PantallaPago(std::function<void(const DatosPago&)> funcPagar,
             std::function<void()> recargar, QWidget* parent = nullptr);
    void setTotal(double total);
    void limpiar();   // borra los datos de pago escritos

private:
    void cambiarMetodo();
    void pagar();

    QComboBox* metodo;
    QLabel* etiquetaTotal;
    QLineEdit* dato1;
    QLineEdit* dato2;
    QLineEdit* dato3;
    QLineEdit* dato4;
    std::function<void(const DatosPago&)> alPagar;
    std::function<void()> alRecargar;
};
