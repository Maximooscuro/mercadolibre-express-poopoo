#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QLabel>
#include <functional>
#include <vector>
#include "Producto.h"

// Muestra el catalogo de productos reales. Esta pantalla es la duena de los
// productos: los crea al cargar el archivo y los destruye en su destructor.
class PantallaCatalogo : public QWidget {
public:
    using Callback = std::function<bool(Producto*, int)>;
    explicit PantallaCatalogo(Callback alAgregar, QWidget* parent = nullptr);
    ~PantallaCatalogo() override;

    void refrescar();   // vuelve a dibujar la tabla (por ejemplo, tras bajar el stock)

private:
    void agregar();

    std::vector<Producto*> productos;
    QTableWidget* tabla;
    QSpinBox* cantidad;
    QLabel* estado;
    Callback alAgregar;
};
