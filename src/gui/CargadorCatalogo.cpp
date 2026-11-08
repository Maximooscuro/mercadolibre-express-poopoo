#include "CargadorCatalogo.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QStringList>

QString buscarArchivo(const QString& rutaRelativa) {
    QDir dir(QCoreApplication::applicationDirPath());
    for (int i = 0; i < 6; ++i) {
        if (dir.exists(rutaRelativa)) return dir.filePath(rutaRelativa);
        if (!dir.cdUp()) break;
    }
    return QString();
}

std::vector<ProductoDemo> cargarCatalogo(const QString& rutaRelativa, QString& error) {
    std::vector<ProductoDemo> productos;
    error.clear();

    QString ruta = buscarArchivo(rutaRelativa);
    if (ruta.isEmpty()) {
        error = "No se encontró " + rutaRelativa + ".";
        return productos;
    }

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) {
        error = "No se pudo abrir " + ruta + ".";
        return productos;
    }

    QTextStream in(&archivo);
    in.setEncoding(QStringConverter::Utf8);

    int invalidas = 0;
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (linea.isEmpty() || linea.startsWith('#')) continue;

        QStringList c = linea.split('|');
        if (c.size() < 6) { ++invalidas; continue; }

        bool okPrecio = false, okStock = false;
        double precio = c[4].trimmed().toDouble(&okPrecio);
        int stock = c[5].trimmed().toInt(&okStock);
        if (!okPrecio || !okStock) { ++invalidas; continue; }

        QString tipo = (c[0].trimmed().toUpper() == "FISICO") ? "Físico" : "Digital";
        productos.push_back({c[1].trimmed(), c[2].trimmed(), tipo, precio, stock});
    }

    if (invalidas > 0)
        error = QString::number(invalidas) + " línea(s) del catálogo tenían formato inválido y se omitieron.";
    return productos;
}
