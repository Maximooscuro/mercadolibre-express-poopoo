#pragma once
// Lee el catalogo que guarda Persistencia (data/catalogo.txt) y lo convierte
// en ProductoDemo para la GUI. Es temporal: cuando Persistencia entregue su
// funcion de cargar, se reemplaza este archivo y las pantallas no cambian.
#include <QString>
#include <vector>
#include "ProductoDemo.h"

// Busca un archivo relativo (ej. "data/catalogo.txt") subiendo de carpeta
// desde donde esta el .exe (CLion lo ejecuta dentro de cmake-build-debug).
QString buscarArchivo(const QString& rutaRelativa);

// Formato por linea:
// TIPO|codigo|nombre|descripcion|precio|stock|peso_o_tamano|dimensiones_o_url
// Las lineas vacias o que empiezan con # se ignoran.
// 'error' queda vacio si todo salio bien; si no, explica que paso.
std::vector<ProductoDemo> cargarCatalogo(const QString& rutaRelativa, QString& error);
