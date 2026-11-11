#include "VentanaPrincipal.h"
#include "PantallaLogin.h"
#include "PantallaCatalogo.h"
#include "PantallaCompra.h"
#include "PantallaPago.h"
#include "PantallaReportes.h"
#include "Mensajes.h"
#include "Pedido.h"
#include "DetallePedido.h"
#include "TarjetaCredito.h"
#include "TransferenciaBancaria.h"
#include "ServicioNotificacion.h"
#include "Excepciones.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QMessageBox>
#include <QDate>
#include <exception>

VentanaPrincipal::VentanaPrincipal(QWidget* parent) : QWidget(parent) {
    setObjectName("ventana");
    setAttribute(Qt::WA_StyledBackground, true);
    setWindowTitle("MercadoLibre Express");
    resize(900, 560);

    login = new PantallaLogin([this](const QString& n) { ingresar(n); });
    catalogo = new PantallaCatalogo([this](Producto* p, int c) { return agregarAlCarrito(p, c); });
    compra = new PantallaCompra([this](int f) { quitarDelCarrito(f); }, [this]() { irAPago(); });
    pago = new PantallaPago([this](const DatosPago& d) { procesarPago(d); },
                            [this]() { recargarSaldo(); });
    reportes = new PantallaReportes();

    pila = new QStackedWidget();
    pila->setContentsMargins(16, 16, 16, 16);
    pila->addWidget(catalogo);   // indice 0
    pila->addWidget(compra);     // indice 1
    pila->addWidget(pago);       // indice 2
    pila->addWidget(reportes);   // indice 3

    // Barra superior: logo, navegacion, saludo y cambio de usuario
    auto* encabezado = new QFrame();
    encabezado->setObjectName("encabezado");
    auto* barra = new QHBoxLayout(encabezado);
    barra->setContentsMargins(16, 8, 16, 8);

    auto* logo = new QLabel("MercadoLibre Express");
    logo->setObjectName("logo");
    barra->addWidget(logo);
    barra->addSpacing(24);

    const char* nombres[] = {"Catálogo", "Compra", "Pago", "Reportes"};
    for (int i = 0; i < 4; ++i) {
        auto* b = new QPushButton(nombres[i]);
        b->setObjectName("botonNav");
        b->setCheckable(true);
        b->setCursor(Qt::PointingHandCursor);
        barra->addWidget(b);
        botonesNav.push_back(b);
        connect(b, &QPushButton::clicked, [this, i]() { irA(i); });
    }
    barra->addStretch();

    saludo = new QLabel("Hola");
    saludo->setObjectName("saludo");
    barra->addWidget(saludo);

    auto* salir = new QPushButton("Cambiar de usuario");
    salir->setObjectName("botonSalir");
    salir->setCursor(Qt::PointingHandCursor);
    barra->addWidget(salir);
    connect(salir, &QPushButton::clicked, [this]() { cerrarSesion(); });

    auto* contenido = new QWidget();
    auto* vContenido = new QVBoxLayout(contenido);
    vContenido->setContentsMargins(0, 0, 0, 0);
    vContenido->setSpacing(0);
    vContenido->addWidget(encabezado);
    vContenido->addWidget(pila);

    raiz = new QStackedWidget();
    raiz->addWidget(login);       // indice 0
    raiz->addWidget(contenido);   // indice 1

    auto* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(raiz);

    irA(0);
    raiz->setCurrentIndex(0);     // arranca en el login
}

void VentanaPrincipal::ingresar(const QString& usuario) {
    // Cada usuario tiene su Cliente real, con el saldo inicial que usa el menu de consola
    std::string clave = usuario.toStdString();
    auto it = clientes.find(clave);
    if (it == clientes.end()) {
        it = clientes.emplace(clave, Cliente(clave, clave + "@correo.com", "Villavicencio", 500000.0f)).first;
    }
    clienteActual = &it->second;

    carrito.clear();
    compra->actualizar(carrito);
    pago->limpiar();
    actualizarSaludo();
    irA(0);
    raiz->setCurrentIndex(1);
}

void VentanaPrincipal::cerrarSesion() {
    carrito.clear();
    compra->actualizar(carrito);
    pago->limpiar();
    login->limpiar();
    clienteActual = nullptr;
    raiz->setCurrentIndex(0);
}

void VentanaPrincipal::actualizarSaludo() {
    if (!clienteActual) return;
    saludo->setText("Hola, " + QString::fromStdString(clienteActual->getNombre()) +
                    "   |   Saldo: $" + QString::number(clienteActual->getSaldoDisponible(), 'f', 0));
}

void VentanaPrincipal::irA(int indice) {
    if (indice == 2) pago->setTotal(totalCarrito());
    pila->setCurrentIndex(indice);
    for (int i = 0; i < static_cast<int>(botonesNav.size()); ++i)
        botonesNav[i]->setChecked(i == indice);
}

bool VentanaPrincipal::agregarAlCarrito(Producto* p, int cantidad) {
    ItemCarrito* existente = nullptr;
    for (auto& it : carrito) {
        if (it.producto == p) { existente = &it; break; }
    }
    int nuevaCantidad = (existente ? existente->cantidad : 0) + cantidad;

    // Se valida con la logica real del Backend: DetallePedido lanza
    // StockAgotadoException o MontoInvalidoException si algo esta mal.
    try {
        DetallePedido prueba(p, nuevaCantidad);
        (void)prueba;
    } catch (const std::exception& e) {
        mostrarError(this, QString::fromUtf8(e.what()));
        return false;
    }

    if (existente) existente->cantidad = nuevaCantidad;
    else carrito.push_back({p, cantidad});
    compra->actualizar(carrito);
    return true;
}

void VentanaPrincipal::quitarDelCarrito(int fila) {
    if (fila >= 0 && fila < static_cast<int>(carrito.size())) {
        carrito.erase(carrito.begin() + fila);
        compra->actualizar(carrito);
    }
}

void VentanaPrincipal::irAPago() {
    if (carrito.empty()) {
        mostrarError(this, "El carrito está vacío.");
        return;
    }
    irA(2);
}

void VentanaPrincipal::procesarPago(const DatosPago& d) {
    if (!clienteActual) return;

    try {
        // El Pedido vive en la pila (sin new): su destructor libera los detalles
        // siempre, incluso si algo lanza una excepcion (no hay fugas de memoria).
        std::string seguimiento = "PED-" + std::to_string(proximoPedido++);
        std::string fecha = QDate::currentDate().toString("dd/MM/yyyy").toStdString();
        Pedido pedido(seguimiento, fecha);

        for (const auto& it : carrito)
            pedido.agregarDetalle(it.producto, it.cantidad);

        float total = pedido.getMontoTotal();
        if (clienteActual->getSaldoDisponible() < total)
            throw SaldoInsuficienteException();

        ServicioNotificacion notificador;
        bool aprobado = false;

        // Polimorfismo: las dos formas de pago se usan como MetodoPago
        if (d.metodo == 0) {
            TarjetaCredito tarjeta(d.dato1.toStdString(), d.dato2.toStdString(),
                                   d.dato3.toStdString(), d.dato4.toStdString());
            aprobado = pedido.procesarOrden(tarjeta, notificador, clienteActual->getCorreo());
        } else {
            TransferenciaBancaria transferencia(d.dato1.toStdString(), d.dato2.toStdString());
            aprobado = pedido.procesarOrden(transferencia, notificador, clienteActual->getCorreo());
        }

        QString cliente = QString::fromStdString(clienteActual->getNombre());
        QString numero = QString::fromStdString(pedido.getNumeroSeguimiento());

        if (!aprobado) {
            historial.push_back({numero, QString::fromStdString(fecha), cliente,
                                 QString::fromStdString(pedido.getEstado()), total});
            reportes->actualizar(historial);
            mostrarError(this, "Pago rechazado. Pedido " + numero + ".");
            return;
        }

        // Pago aprobado: ahora si se cobra el saldo y se baja el stock
        clienteActual->descontarSaldo(total);
        for (const auto& it : carrito)
            it.producto->reducirStock(it.cantidad);

        historial.push_back({numero, QString::fromStdString(fecha), cliente,
                             QString::fromStdString(pedido.getEstado()), total});
        reportes->actualizar(historial);

        carrito.clear();
        compra->actualizar(carrito);
        pago->limpiar();
        catalogo->refrescar();
        actualizarSaludo();

        QMessageBox::information(this, "Pago",
            "Pago exitoso. Pedido " + numero + " (" +
            QString::fromStdString(pedido.getEstado()) + ") por $" +
            QString::number(total, 'f', 0) + ".");
        irA(0);

    } catch (const std::exception& e) {
        // Aqui caen las excepciones reales del Backend (saldo, stock, monto, seguimiento)
        mostrarError(this, QString::fromUtf8(e.what()));
    }
}

void VentanaPrincipal::recargarSaldo() {
    if (!clienteActual) return;
    try {
        clienteActual->recargarSaldo(100000.0f);
        actualizarSaludo();
    } catch (const std::exception& e) {
        mostrarError(this, QString::fromUtf8(e.what()));
    }
}

double VentanaPrincipal::totalCarrito() const {
    double suma = 0;
    for (const auto& it : carrito)
        suma += it.producto->calcularCostoTotal(it.cantidad);
    return suma;
}
