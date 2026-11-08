#include "PantallaReportes.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QHeaderView>
#include <QLabel>
#include <QStringList>

PantallaReportes::PantallaReportes(QWidget* parent) : QWidget(parent) {
    auto* layout = new QVBoxLayout(this);

    auto* tabla = new QTableWidget(0, 4);
    tabla->setHorizontalHeaderLabels(QStringList{"Seguimiento", "Fecha", "Estado", "Total"});
    tabla->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    layout->addWidget(tabla);

    layout->addWidget(new QLabel("Historial de pedidos (pendiente de conectar con Persistencia)"));
}
