//
// Created by sapuy on 2/11/2026.
//

#include <QApplication>
#include <QPushButton>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QPushButton boton("Hola MercadoLibre Express");
    boton.resize(300, 80);
    boton.show();
    return app.exec();
}