#include "PantallaCatalogo.h"
#include "CargadorCatalogo.h"
#include "Mensajes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QStringList>

PantallaCatalogo::PantallaCatalogo(Callback cb, QWidget* parent)
    : QWidget(parent), alAgregar(cb) {

    // 1. Intenta leer el catalogo real que guarda Persistencia
    QString aviso;
    datos = cargarCatalogo("data/catalogo.txt", aviso);

    // 2. Si no se pudo leer nada, usa productos de ejemplo para no dejar la tabla vacia
    if (datos.empty()) {
        aviso = (aviso.isEmpty() ? QString() : aviso + " ") + "Se usan productos de ejemplo.";
        datos = {
            {"P001", "Audífonos Bluetooth", "Físico", 85000, 2},
            {"P002", "Teclado mecánico", "Físico", 210000, 15},
            {"P003", "Curso de C++ (PDF)", "Digital", 49000, 100}
        };
    }

    auto* layout = new QVBoxLayout(this);

    tabla = new QTableWidget(static_cast<int>(datos.size()), 5);
    tabla->setHorizontalHeaderLabels(QStringList{"Código", "Nombre", "Tipo", "Precio", "Stock"});
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (int i = 0; i < static_cast<int>(datos.size()); ++i) {
        const ProductoDemo& p = datos[i];
        tabla->setItem(i, 0, new QTableWidgetItem(p.codigo));
        tabla->setItem(i, 1, new QTableWidgetItem(p.nombre));
        tabla->setItem(i, 2, new QTableWidgetItem(p.tipo));
        tabla->setItem(i, 3, new QTableWidgetItem("$" + QString::number(p.precio, 'f', 0)));
        tabla->setItem(i, 4, new QTableWidgetItem(QString::number(p.stock)));
    }
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
}

void PantallaCatalogo::agregar() {
    int fila = tabla->currentRow();
    if (fila < 0) {
        mostrarError(this, "Seleccione un producto de la tabla.");
        return;
    }
    int cant = cantidad->value();
    if (cant <= 0) {
        mostrarError(this, "La cantidad debe ser mayor que 0.");
        return;
    }
    // Simula StockAgotadoException (caso de prueba CP-01)
    if (cant > datos[fila].stock) {
        mostrarError(this, "Stock agotado: solo hay " + QString::number(datos[fila].stock) +
                           " unidades de " + datos[fila].nombre + ".");
        return;
    }
    // alAgregar devuelve false si la ventana principal rechazo el producto
    if (alAgregar(datos[fila], cant))
        estado->setText("Agregado: " + QString::number(cant) + " x " + datos[fila].nombre);
}

// Se llama cuando el pago fue exitoso: resta lo comprado y actualiza la tabla
void PantallaCatalogo::descontarStock(const std::vector<ItemCarrito>& items) {
    for (const auto& it : items) {
        for (int i = 0; i < static_cast<int>(datos.size()); ++i) {
            if (datos[i].codigo == it.producto.codigo) {
                datos[i].stock -= it.cantidad;
                if (datos[i].stock < 0) datos[i].stock = 0;
                tabla->item(i, 4)->setText(QString::number(datos[i].stock));
            }
        }
    }
}