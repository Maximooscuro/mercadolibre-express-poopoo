#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QString>
#include <functional>

class PantallaLogin : public QWidget {
public:
    explicit PantallaLogin(std::function<void(const QString&)> alIngresar,
                           QWidget* parent = nullptr);
    void limpiar();

private:
    void intentarIngreso();

    QLineEdit* usuario;
    QLineEdit* clave;
    std::function<void(const QString&)> alIngresar;
};
