#include "PantallaPago.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QStringList>

PantallaPago::PantallaPago(std::function<void(const DatosPago&)> funcPagar,
                           std::function<void()> recargar, QWidget* parent)
    : QWidget(parent), alPagar(funcPagar), alRecargar(recargar) {
    auto* layout = new QVBoxLayout(this);

    etiquetaTotal = new QLabel("Total a pagar: $0");
    layout->addWidget(etiquetaTotal);

    layout->addWidget(new QLabel("Método de pago:"));
    metodo = new QComboBox();
    metodo->addItems(QStringList{"Tarjeta de crédito", "Transferencia bancaria"});
    layout->addWidget(metodo);

    dato1 = new QLineEdit();
    dato2 = new QLineEdit();
    dato3 = new QLineEdit();
    dato4 = new QLineEdit();
    layout->addWidget(dato1);
    layout->addWidget(dato2);
    layout->addWidget(dato3);
    layout->addWidget(dato4);

    auto* boton = new QPushButton("Pagar");
    layout->addWidget(boton);

    auto* recargarBtn = new QPushButton("Recargar saldo (+$100.000)");
    recargarBtn->setObjectName("botonLink");
    layout->addWidget(recargarBtn);
    layout->addStretch();

    connect(metodo, &QComboBox::currentIndexChanged, [this](int) { cambiarMetodo(); });
    connect(boton, &QPushButton::clicked, [this]() { pagar(); });
    connect(recargarBtn, &QPushButton::clicked, [this]() { alRecargar(); });

    cambiarMetodo();
}

void PantallaPago::cambiarMetodo() {
    if (metodo->currentIndex() == 0) {
        dato1->setPlaceholderText("Número de tarjeta (16 dígitos)");
        dato2->setPlaceholderText("Titular");
        dato3->setPlaceholderText("Vencimiento (MM/AA)");
        dato4->setPlaceholderText("CVV (3 dígitos)");
        dato3->setVisible(true);
        dato4->setVisible(true);
    } else {
        dato1->setPlaceholderText("Banco");
        dato2->setPlaceholderText("Número de cuenta");
        dato3->setVisible(false);
        dato4->setVisible(false);
    }
}

void PantallaPago::setTotal(double total) {
    etiquetaTotal->setText("Total a pagar: $" + QString::number(total, 'f', 0));
}

void PantallaPago::limpiar() {
    dato1->clear();
    dato2->clear();
    dato3->clear();
    dato4->clear();
}

void PantallaPago::pagar() {
    DatosPago d;
    d.metodo = metodo->currentIndex();
    d.dato1 = dato1->text().trimmed();
    d.dato2 = dato2->text().trimmed();
    d.dato3 = dato3->text().trimmed();
    d.dato4 = dato4->text().trimmed();
    alPagar(d);   // VentanaPrincipal arma el pedido, cobra y atrapa los errores
}
