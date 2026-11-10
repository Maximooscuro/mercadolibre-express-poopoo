#include "VentanaPrincipal.h"
#include "PantallaLogin.h"
#include "PantallaCatalogo.h"
#include "PantallaCompra.h"
#include "PantallaPago.h"
#include "PantallaReportes.h"
#include "Mensajes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>

VentanaPrincipal::VentanaPrincipal(QWidget* parent) : QWidget(parent) {
    setObjectName("ventana");
    setAttribute(Qt::WA_StyledBackground, true);
    setWindowTitle("MercadoLibre Express");
    resize(900, 560);

    login = new PantallaLogin([this](const QString& n) { ingresar(n); });
    catalogo = new PantallaCatalogo([this](const ProductoDemo& p, int c) { return agregarAlCarrito(p, c); });
    compra = new PantallaCompra([this](int f) { quitarDelCarrito(f); }, [this]() { irAPago(); });
    pago = new PantallaPago([this]() { pagoCompletado(); });
    reportes = new PantallaReportes();

    pila = new QStackedWidget();
    pila->setContentsMargins(16, 16, 16, 16);
    pila->addWidget(catalogo);   // indice 0
    pila->addWidget(compra);     // indice 1
    pila->addWidget(pago);       // indice 2
    pila->addWidget(reportes);   // indice 3

    // Barra superior: logo, navegacion, saludo y cerrar sesion
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

void VentanaPrincipal::ingresar(const QString& nombre) {
    saludo->setText("Hola, " + nombre);
    carrito.clear();
    compra->actualizar(carrito);
    irA(0);
    raiz->setCurrentIndex(1);
}

void VentanaPrincipal::cerrarSesion() {
    carrito.clear();
    compra->actualizar(carrito);
    login->limpiar();
    raiz->setCurrentIndex(0);
}

void VentanaPrincipal::irA(int indice) {
    if (indice == 2) pago->setTotal(totalCarrito());
    pila->setCurrentIndex(indice);
    for (int i = 0; i < static_cast<int>(botonesNav.size()); ++i)
        botonesNav[i]->setChecked(i == indice);
}

bool VentanaPrincipal::agregarAlCarrito(const ProductoDemo& p, int cantidad) {
    for (auto& it : carrito) {
        if (it.producto.codigo == p.codigo) {
            // p.stock es el stock actual del catalogo: no se puede pasar de ahi
            if (it.cantidad + cantidad > p.stock) {
                mostrarError(this, "Stock agotado: ya tiene " + QString::number(it.cantidad) +
                                   " en el carrito y solo hay " + QString::number(p.stock) +
                                   " unidades de " + p.nombre + ".");
                return false;
            }
            it.cantidad += cantidad;
            compra->actualizar(carrito);
            return true;
        }
    }
    carrito.push_back({p, cantidad});
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

void VentanaPrincipal::pagoCompletado() {
    catalogo->descontarStock(carrito);   // primero resta el stock, luego limpia el carrito
    carrito.clear();
    compra->actualizar(carrito);
    irA(0);
}

double VentanaPrincipal::totalCarrito() const {
    double suma = 0;
    for (const auto& it : carrito) suma += it.producto.precio * it.cantidad;
    return suma;
}