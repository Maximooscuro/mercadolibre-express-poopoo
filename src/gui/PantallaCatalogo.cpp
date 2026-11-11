#include "PantallaCatalogo.h"
#include "CargadorCatalogo.h"
#include "ProductoFisico.h"
#include "ProductoDigital.h"
#include "Mensajes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QStringList>

PantallaCatalogo::PantallaCatalogo(Callback cb, QWidget* parent)
    : QWidget(parent), alAgregar(cb) {

    // 1. Catalogo real que guarda Persistencia
    QString aviso;
    productos = cargarCatalogo("data/catalogo.txt", aviso);

    // 2. Si no se pudo leer nada, productos de respaldo (tambien reales)
    if (productos.empty()) {
        aviso = (aviso.isEmpty() ? QString() : aviso + " ") + "Se usan productos de ejemplo.";
        productos.push_back(new ProductoFisico("P001", "Mouse", "Mouse inalambrico", 45000.0f, 20, 0.15f, "10x6x4"));
        productos.push_back(new ProductoFisico("P002", "Teclado", "Teclado mecanico", 120000.0f, 10, 0.9f, "44x14x4"));
        productos.push_back(new ProductoDigital("P003", "Ebook C++", "Guia de POO", 30000.0f, 999, 5.2f, "http://descarga.com/ebook"));
    }

    auto* layout = new QVBoxLayout(this);

    tabla = new QTableWidget(0, 5);
    tabla->setHorizontalHeaderLabels(QStringList{"Código", "Nombre", "Tipo", "Precio", "Stock"});
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tabla);

    auto* fila = new QHBoxLayout();
    fila->addWidget(new QLabel("Cantidad:"));
    cantidad = new QSpinBox();
    cantidad->setRange(0, 999);
    cantidad->setValue(1);
    fila->addWidget(cantidad);
    auto* boton = new QPushButton("Agregar al carrito");
    fila->addWidget(boton);
    fila->addStretch();
    layout->addLayout(fila);

    estado = new QLabel(aviso);
    layout->addWidget(estado);

    connect(boton, &QPushButton::clicked, [this]() { agregar(); });

    refrescar();
}

PantallaCatalogo::~PantallaCatalogo() {
    for (Producto* p : productos) delete p;
    productos.clear();
}

void PantallaCatalogo::refrescar() {
    tabla->setRowCount(static_cast<int>(productos.size()));
    for (int i = 0; i < static_cast<int>(productos.size()); ++i) {
        Producto* p = productos[i];
        // dynamic_cast: pregunta si el producto es fisico o digital
        QString tipo = dynamic_cast<ProductoFisico*>(p) ? "Físico" : "Digital";
        tabla->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(p->getCodigo())));
        tabla->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(p->getNombre())));
        tabla->setItem(i, 2, new QTableWidgetItem(tipo));
        tabla->setItem(i, 3, new QTableWidgetItem("$" + QString::number(p->getPrecio(), 'f', 0)));
        tabla->setItem(i, 4, new QTableWidgetItem(QString::number(p->getStock())));
    }
}

void PantallaCatalogo::agregar() {
    int fila = tabla->currentRow();
    if (fila < 0) {
        mostrarError(this, "Seleccione un producto de la tabla.");
        return;
    }
    int cant = cantidad->value();
    Producto* p = productos[fila];

    // La validacion de stock y cantidad la hace el Backend (DetallePedido),
    // y VentanaPrincipal muestra el error si algo falla.
    if (alAgregar(p, cant))
        estado->setText("Agregado: " + QString::number(cant) + " x " +
                        QString::fromStdString(p->getNombre()));
}
