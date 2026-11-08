#include "PantallaPago.h"
#include "Mensajes.h"
#include "MetodoPago.h"      // de include/ (interfaz del lider)
#include "Excepciones.h"     // de include/ (header-only)
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <exception>
#include <string>

namespace {
// Pago de prueba: hereda de MetodoPago para demostrar polimorfismo sin esperar al Backend.
// Se reemplaza por TarjetaCredito / TransferenciaBancaria cuando existan sus .cpp.
class PagoFalso : public MetodoPago {
public:
    explicit PagoFalso(std::string nombre) : nombre(nombre) {}
    bool procesarPago(float monto) override {
        if (monto <= 0) throw MontoInvalidoException("Monto de pago invalido");
        return true;
    }
    std::string getNombreMetodo() const override { return nombre; }
private:
    std::string nombre;
};
}

PantallaPago::PantallaPago(std::function<void()> pagado, QWidget* parent)
    : QWidget(parent), alPagado(pagado) {
    auto* layout = new QVBoxLayout(this);

    etiquetaTotal = new QLabel("Total a pagar: $0");
    layout->addWidget(etiquetaTotal);

    layout->addWidget(new QLabel("Metodo de pago:"));
    metodo = new QComboBox();
    metodo->addItems(QStringList{"Tarjeta de credito", "Transferencia bancaria"});
    layout->addWidget(metodo);

    auto* boton = new QPushButton("Pagar");
    layout->addWidget(boton);
    layout->addStretch();

    connect(boton, &QPushButton::clicked, [this]() { pagar(); });
}

void PantallaPago::setTotal(double nuevoTotal) {
    total = nuevoTotal;
    etiquetaTotal->setText("Total a pagar: $" + QString::number(total, 'f', 0));
}

void PantallaPago::pagar() {
    PagoFalso pago(metodo->currentText().toStdString());
    MetodoPago& ref = pago;   // se usa a traves de la interfaz: polimorfismo
    try {
        ref.procesarPago(static_cast<float>(total));
        QMessageBox::information(this, "Pago",
            "Pago exitoso con " + QString::fromStdString(ref.getNombreMetodo()));
        alPagado();
    } catch (const std::exception& e) {
        mostrarError(this, QString::fromUtf8(e.what()));
    } catch (...) {
        mostrarError(this, "Ocurrio un error al procesar el pago.");
    }
}
