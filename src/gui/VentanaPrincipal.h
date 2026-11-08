#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QString>
#include <vector>
#include "ProductoDemo.h"

class QLabel;
class QPushButton;
class PantallaLogin;
class PantallaCatalogo;
class PantallaCompra;
class PantallaPago;
class PantallaReportes;

class VentanaPrincipal : public QWidget {
public:
    explicit VentanaPrincipal(QWidget* parent = nullptr);

private:
    void ingresar(const QString& nombre);
    void cerrarSesion();
    void irA(int indice);
    bool agregarAlCarrito(const ProductoDemo& p, int cantidad);
    void quitarDelCarrito(int fila);
    void irAPago();
    void pagoCompletado();
    double totalCarrito() const;

    QStackedWidget* raiz;      // 0 = login, 1 = aplicacion
    QStackedWidget* pila;      // las 4 pantallas
    PantallaLogin* login;
    PantallaCatalogo* catalogo;
    PantallaCompra* compra;
    PantallaPago* pago;
    PantallaReportes* reportes;
    QLabel* saludo;
    std::vector<QPushButton*> botonesNav;
    std::vector<ItemCarrito> carrito;
};