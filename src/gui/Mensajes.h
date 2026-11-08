#pragma once
// Ventana de error reutilizable. Se llama desde los catch de las excepciones.
#include <QMessageBox>
#include <QString>
#include <QWidget>

inline void mostrarError(QWidget* padre, const QString& mensaje) {
    QMessageBox::warning(padre, "Error", mensaje);
}
