#pragma once
#include <string>
#include <fstream>
#include "Producto.h"

class GestorArchivos {
private:
    static void escribirFloat(std::ofstream& out, float valor);
    static float leerFloat(std::ifstream& in);

    static void escribirInt(std::ofstream& out, int valor);
    static int leerInt(std::ifstream& in);

    static void escribirString(std::ofstream& out, const std::string& texto);
    static std::string leerString(std::ifstream& in);

public:
    void guardarCatalogoTexto(const std::string& ruta, Producto* productos[], int cantidad) const;
    int cargarCatalogoTexto(const std::string& ruta, Producto* productos[], int maxCapacidad) const;

    static void liberarProductos(Producto* productos[], int cantidad);
};