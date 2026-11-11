#include "CargadorCatalogo.h"
#include "GestorArchivos.h"      // modulo de Persistencia (include/)
#include "ProductoFisico.h"
#include "ProductoDigital.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringConverter>
#include <QStringList>
#include <exception>
#include <string>

QString buscarArchivo(const QString& rutaRelativa) {
    QDir dir(QCoreApplication::applicationDirPath());
    for (int i = 0; i < 6; ++i) {
        if (dir.exists(rutaRelativa)) return dir.filePath(rutaRelativa);
        if (!dir.cdUp()) break;
    }
    return QString();
}

namespace {
// Lector de respaldo: se salta las lineas invalidas en vez de abortar todo.
std::vector<Producto*> leerTolerante(const QString& ruta, int& invalidas) {
    std::vector<Producto*> productos;
    invalidas = 0;

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::ReadOnly | QIODevice::Text)) return productos;

    QTextStream in(&archivo);
    in.setEncoding(QStringConverter::Utf8);

    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        if (linea.isEmpty() || linea.startsWith('#')) continue;

        QStringList c = linea.split('|');
        if (c.size() < 8) { ++invalidas; continue; }

        bool okPrecio = false, okStock = false, okNum = false;
        float precio = c[4].trimmed().toFloat(&okPrecio);
        int stock = c[5].trimmed().toInt(&okStock);
        float numero = c[6].trimmed().toFloat(&okNum);
        if (!okPrecio || !okStock || !okNum) { ++invalidas; continue; }

        std::string cod = c[1].trimmed().toStdString();
        std::string nom = c[2].trimmed().toStdString();
        std::string desc = c[3].trimmed().toStdString();
        std::string extra = c[7].trimmed().toStdString();

        try {
            if (c[0].trimmed().toUpper() == "FISICO")
                productos.push_back(new ProductoFisico(cod, nom, desc, precio, stock, numero, extra));
            else
                productos.push_back(new ProductoDigital(cod, nom, desc, precio, stock, numero, extra));
        } catch (const std::exception&) {
            ++invalidas;
        }
    }
    return productos;
}
}

std::vector<Producto*> cargarCatalogo(const QString& rutaRelativa, QString& error) {
    std::vector<Producto*> productos;
    error.clear();

    QString ruta = buscarArchivo(rutaRelativa);
    if (ruta.isEmpty()) {
        error = "No se encontró " + rutaRelativa + ".";
        return productos;
    }

    // 1) Primero el modulo de Persistencia: es el mismo codigo que usa la consola
    QString motivo;
    try {
        GestorArchivos gestor;
        productos = gestor.cargarCatalogoTexto(ruta.toStdString());
        error = "Catálogo leído de " + ruta + " con Persistencia (" +
                QString::number(productos.size()) + " productos).";
        return productos;
    } catch (const std::exception& e) {
        motivo = QString::fromUtf8(e.what());
    }

    // 2) Si Persistencia rechaza el archivo (por ejemplo, una linea mal escrita),
    //    se usa el lector de respaldo para no dejar el catalogo vacio.
    int invalidas = 0;
    productos = leerTolerante(ruta, invalidas);
    error = "Persistencia no pudo leer " + ruta + " (" + motivo + "). Lector de respaldo: " +
            QString::number(productos.size()) + " productos";
    if (invalidas > 0)
        error += ", " + QString::number(invalidas) + " línea(s) inválidas omitidas";
    error += ".";
    return productos;
}