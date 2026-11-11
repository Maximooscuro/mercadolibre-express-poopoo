#pragma once
// Estructuras pequenas que la GUI usa para pasar datos entre pantallas.
// Las clases del negocio (Producto, Pedido, Cliente...) vienen de include/.
#include <QString>
#include "Producto.h"

struct ItemCarrito {
    Producto* producto;   // apunta a un producto del catalogo (no es dueno)
    int cantidad;
};

struct RegistroPedido {
    QString seguimiento;
    QString fecha;
    QString cliente;
    QString estado;
    double total;
};

// Datos que escribe el usuario en la pantalla de pago.
// metodo 0 = tarjeta: dato1 numero, dato2 titular, dato3 vencimiento, dato4 cvv
// metodo 1 = transferencia: dato1 banco, dato2 numero de cuenta
struct DatosPago {
    int metodo;
    QString dato1, dato2, dato3, dato4;
};
