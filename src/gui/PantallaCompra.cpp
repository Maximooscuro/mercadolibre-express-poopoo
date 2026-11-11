#include "PantallaCompra.h"
#include "Mensajes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QStringList>

PantallaCompra::PantallaCompra(std::function<void(int)> quitar,
                               std::function<void()> irAPagar, QWidget* parent)
    : QWidget(parent), alQuitar(quitar), alIrAPagar(irAPagar) {
    auto* layout = new QVBoxLayout(this);

    tabla = new QTableWidget(0, 3);
    tabla->setHorizontalHeaderLabels(QStringList{"Producto", "Cantidad", "Subtotal (con envío)"});
    tabla->setSelectionBehavior(QAbstractItemView::SelectRows);
    tabla->setSelectionMode(QAbstractItemView::SingleSelection);
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tabla);

    total = new QLabel("Total: $0");
    layout->addWidget(total);

    auto* fila = new QHBoxLayout();
    auto* quitarBtn = new QPushButton("Quitar seleccionado");
    auto* pagarBtn = new QPushButton("Ir a pagar");
    fila->addWidget(quitarBtn);
    fila->addWidget(pagarBtn);
    fila->addStretch();
    layout->addLayout(fila);

    connect(quitarBtn, &QPushButton::clicked, [this]() {
        int f = tabla->currentRow();
        if (f < 0) {
            mostrarError(this, "Seleccione un producto del carrito.");
            return;
        }
        alQuitar(f);
    });
    connect(pagarBtn, &QPushButton::clicked, [this]() { alIrAPagar(); });
}

void PantallaCompra::actualizar(const std::vector<ItemCarrito>& carrito) {
    tabla->setRowCount(static_cast<int>(carrito.size()));
    double suma = 0;
    for (int i = 0; i < static_cast<int>(carrito.size()); ++i) {
        const ItemCarrito& it = carrito[i];
        // Polimorfismo: cada tipo de producto calcula su costo (el fisico suma envio)
        double subtotal = it.producto->calcularCostoTotal(it.cantidad);
        suma += subtotal;
        tabla->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(it.producto->getNombre())));
        tabla->setItem(i, 1, new QTableWidgetItem(QString::number(it.cantidad)));
        tabla->setItem(i, 2, new QTableWidgetItem("$" + QString::number(subtotal, 'f', 0)));
    }
    total->setText("Total (incluye envíos): $" + QString::number(suma, 'f', 0));
}
