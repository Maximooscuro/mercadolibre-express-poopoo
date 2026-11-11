#pragma once
#include <QWidget>
#include <QStackedWidget>
#include <QString>
#include <map>
#include <string>
#include <vector>
#include "Cliente.h"
#include "ModeloGui.h"

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
    void ingresar(const QString& usuario);
    void cerrarSesion();
    void irA(int indice);
    bool agregarAlCarrito(Producto* p, int cantidad);
    void quitarDelCarrito(int fila);
    void irAPago();
    void procesarPago(const DatosPago& d);
    void recargarSaldo();
    void actualizarSaludo();
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

    std::map<std::string, Cliente> clientes;   // un Cliente real por usuario
    Cliente* clienteActual = nullptr;
    std::vector<ItemCarrito> carrito;
    std::vector<RegistroPedido> historial;
    int proximoPedido = 1000;
};
