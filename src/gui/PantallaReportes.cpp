#include "PantallaReportes.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include <QLabel>
#include <QStringList>

PantallaReportes::PantallaReportes(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);

    tabla = new QTableWidget(0, 5);
    tabla->setHorizontalHeaderLabels(QStringList{"Seguimiento", "Fecha", "Cliente", "Estado", "Total"});
    tabla->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tabla);

    layout->addWidget(new QLabel("Pedidos de esta sesión (aún no se guardan en archivo)"));
}

void PantallaReportes::actualizar(const std::vector<RegistroPedido>& historial) {
    tabla->setRowCount(static_cast<int>(historial.size()));
    for (int i = 0; i < static_cast<int>(historial.size()); ++i) {
        const RegistroPedido& r = historial[i];
        tabla->setItem(i, 0, new QTableWidgetItem(r.seguimiento));
        tabla->setItem(i, 1, new QTableWidgetItem(r.fecha));
        tabla->setItem(i, 2, new QTableWidgetItem(r.cliente));
        tabla->setItem(i, 3, new QTableWidgetItem(r.estado));
        tabla->setItem(i, 4, new QTableWidgetItem("$" + QString::number(r.total, 'f', 0)));
    }
}
