#pragma once
// Lee data/catalogo.txt (formato de Persistencia) y crea productos REALES
// (ProductoFisico / ProductoDigital) con new. Quien los recibe es el dueno
// y debe hacer delete de cada uno.
#include <QString>
#include <vector>
#include "Producto.h"

// Busca un archivo relativo subiendo de carpeta desde donde esta el .exe.
QString buscarArchivo(const QString& rutaRelativa);

// Formato por linea:
// TIPO|codigo|nombre|descripcion|precio|stock|peso_o_tamano|dimensiones_o_url
// Lineas vacias o que empiezan con # se ignoran.
// 'error' devuelve un informe: de que archivo se leyo y cuantos productos salieron
// (o por que fallo). La pantalla lo muestra bajo la tabla.
std::vector<Producto*> cargarCatalogo(const QString& rutaRelativa, QString& error);