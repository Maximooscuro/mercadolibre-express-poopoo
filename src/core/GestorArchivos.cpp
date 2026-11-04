include "GestorArchivos.h"
#include "ProductoFisico.h"
#include "ProductoDigital.h"
#include <sstream>
#include <stdexcept>

void GestorArchivos::escribirFloat(std::ofstream& out, float valor) {
    out.write(reinterpret_cast<const char*>(&valor), sizeof(float));
}

float GestorArchivos::leerFloat(std::ifstream& in) {
    float valor = 0.0f;
    in.read(reinterpret_cast<char*>(&valor), sizeof(float));
    return valor;
}

void GestorArchivos::escribirInt(std::ofstream& out, int valor) {
    out.write(reinterpret_cast<const char*>(&valor), sizeof(int));
}

int GestorArchivos::leerInt(std::ifstream& in) {
    int valor = 0;
    in.read(reinterpret_cast<char*>(&valor), sizeof(int));
    return valor;
}

void GestorArchivos::escribirString(std::ofstream& out, const std::string& texto) {
    int tamano = static_cast<int>(texto.size());
    escribirInt(out, tamano);
    out.write(texto.data(), tamano);
}

std::string GestorArchivos::leerString(std::ifstream& in) {
    int tamano = leerInt(in);
    std::string texto(tamano, '\0');
    in.read(&texto[0], tamano);
    return texto;
}

void GestorArchivos::guardarCatalogoTexto(const std::string& ruta, Producto* productos[], int cantidad) const {
    std::ofstream out(ruta);
    if (!out.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo del catalogo.");
    }

    for (int i = 0; i < cantidad; ++i) {
        Producto* p = productos[i];
        if (!p) continue;

        if (const auto* f = dynamic_cast<const ProductoFisico*>(p)) {
            out << "FISICO|" << f->getCodigo() << "|" << f->getNombre() << "|"
                << f->getPrecio() << "|" << f->getPeso() << "\n";
        } else if (const auto* d = dynamic_cast<const ProductoDigital*>(p)) {
            out << "DIGITAL|" << d->getCodigo() << "|" << d->getNombre() << "|"
                << d->getPrecio() << "|" << d->getUrlDescarga() << "\n";
        }
    }
}

int GestorArchivos::cargarCatalogoTexto(const std::string& ruta, Producto* productos[], int maxCapacidad) const {
    std::ifstream in(ruta);
    if (!in.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo para lectura.");
    }

    std::string linea;
    int contador = 0;

    while (std::getline(in, linea) && contador < maxCapacidad) {
        if (linea.empty() || linea[0] == '#') continue;

        std::stringstream ss(linea);
        std::string tipo, codigo, nombre, precioStr, extra;

        std::getline(ss, tipo, '|');
        std::getline(ss, codigo, '|');
        std::getline(ss, nombre, '|');
        std::getline(ss, precioStr, '|');
        std::getline(ss, extra, '|');

        float precio = std::stof(precioStr);

        if (tipo == "FISICO") {
            float peso = std::stof(extra);
            productos[contador++] = new ProductoFisico(codigo, nombre, precio, peso);
        } else if (tipo == "DIGITAL") {
            productos[contador++] = new ProductoDigital(codigo, nombre, precio, extra);
        }
    }

    return contador;
}

void GestorArchivos::liberarProductos(Producto* productos[], int cantidad) {
    for (int i = 0; i < cantidad; ++i) {
        delete productos[i];
        productos[i] = nullptr;
    }
}