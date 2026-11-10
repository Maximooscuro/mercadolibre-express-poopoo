#pragma once
#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QString>
#include <functional>
#include "GestorUsuarios.h"

class PantallaLogin : public QWidget {
public:
    explicit PantallaLogin(std::function<void(const QString&)> alIngresar,
                           QWidget* parent = nullptr);
    void limpiar();   // vuelve al modo "ingresar" con los campos vacios

private:
    void intentarIngreso();
    void alternarModo();
    void actualizarModo();

    GestorUsuarios gestor;
    bool modoRegistro = false;

    QLineEdit* usuario;
    QLineEdit* clave;
    QLineEdit* confirmar;
    QLabel* subtitulo;
    QPushButton* botonPrincipal;
    QPushButton* botonModo;
    std::function<void(const QString&)> alIngresar;
};