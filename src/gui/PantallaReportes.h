#pragma once
#include <QWidget>
#include <QTableWidget>
#include <vector>
#include "ModeloGui.h"

class PantallaReportes : public QWidget {
public:
    explicit PantallaReportes(QWidget* parent = nullptr);
    void actualizar(const std::vector<RegistroPedido>& historial);

private:
    QTableWidget* tabla;
};
