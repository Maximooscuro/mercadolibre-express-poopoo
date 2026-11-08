#include <QApplication>
#include "VentanaPrincipal.h"
#include "Estilos.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");               // estilo base uniforme en todos los equipos
    app.setStyleSheet(estilosApp());      // colores y bordes (QSS)
    VentanaPrincipal ventana;
    ventana.show();
    return app.exec();
}
