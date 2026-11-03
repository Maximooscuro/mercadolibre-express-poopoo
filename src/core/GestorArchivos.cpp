#include "GestorArchivos.h"
#include "ProductoFisico.h"
#include "ProductoDigital.h"

#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>

namespace {

void escribirString(std::ofstream& out, const std::string& s) {
    size_t n = s.size();
    out.write(reinterpret_cast<const char*>(&n), sizeof(n));
    out.write(s.data(), static_cast<std::streamsize>(n));
}

std::string leerString(std::ifstream& in) {
    size_t n = 0;
    in.read(reinterpret_cast<char*>(&n), sizeof(n));
    if (!in || n > 1000000) {
        throw std::runtime_error("Archivo binario corrupto o incompleto (string)");
    }
    std::string s(n, '\0');
    if (n > 0) in.read(&s[0], static_cast<std::streamsize>(n));
    if (!in) throw std::runtime_error("Archivo binario corrupto o incompleto (string)");
    return s;
}

template <typename T>
void escribirValor(std::ofstream& out, const T& v) {
    out.write(reinterpret_cast<const char*>(&v), sizeof(T));
}

template <typename T>
T leerValor(std::ifstream& in) {
    T v{};
    in.read(reinterpret_cast<char*>(&v), sizeof(T));
    if (!in) throw std::runtime_error("Archivo binario corrupto o incompleto");
    return v;
}

std::vector<std::string> separar(const std::string& linea, char delim) {
    std::vector<std::string> campos;
    std::stringstream ss(linea);
    std::string campo;
    while (std::getline(ss, campo, delim)) campos.push_back(campo);
    return campos;
}

std::ofstream abrirSalida(const std::string& ruta, bool binario) {
    std::ofstream out(ruta, binario ? std::ios::binary : std::ios::out);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);
    return out;
}

}

void GestorArchivos::guardarCatalogo(const std::vector<Producto*>& productos,
                                     const std::string& ruta) const {
    std::ofstream out = abrirSalida(ruta, false);
    out << "# TIPO|codigo|nombre|descripcion|precio|stock|peso_o_tamano|dimensiones_o_url\n";

    for (const Producto* p : productos) {
        if (const auto* f = dynamic_cast<const ProductoFisico*>(p)) {
            out << "FISICO|" << f->getCodigo() << '|' << f->getNombre() << '|'
                << f->getDescripcion() << '|' << f->getPrecio() << '|'
                << f->getStock() << '|' << f->getPeso() << '|'
                << f->getDimensiones() << '\n';
        } else if (const auto* d = dynamic_cast<const ProductoDigital*>(p)) {
            out << "DIGITAL|" << d->getCodigo() << '|' << d->getNombre() << '|'
                << d->getDescripcion() << '|' << d->getPrecio() << '|'
                << d->getStock() << '|' << d->getTamanoArchivo() << '|'
                << d->getUrlDescarga() << '\n';
        }
    }
    if (!out) throw std::runtime_error("Error al escribir el catalogo: " + ruta);
}

std::vector<Producto*> GestorArchivos::cargarCatalogo(const std::string& ruta) const {
    std::ifstream in(ruta);
    if (!in.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    std::vector<Producto*> productos;
    try {
        std::string linea;
        int numLinea = 0;
        while (std::getline(in, linea)) {
            ++numLinea;
            if (!linea.empty() && linea.back() == '\r') linea.pop_back();
            if (linea.empty() || linea[0] == '#') continue;

            std::vector<std::string> c = separar(linea, '|');
            if (c.size() != 8) {
                throw std::runtime_error("Linea " + std::to_string(numLinea) +
                                         " del catalogo con formato invalido");
            }
            float precio = std::stof(c[4]);
            int stock = std::stoi(c[5]);

            if (c[0] == "FISICO") {
                productos.push_back(new ProductoFisico(c[1], c[2], c[3], precio, stock,
                                                       std::stof(c[6]), c[7]));
            } else if (c[0] == "DIGITAL") {
                productos.push_back(new ProductoDigital(c[1], c[2], c[3], precio, stock,
                                                        std::stof(c[6]), c[7]));
            } else {
                throw std::runtime_error("Tipo de producto desconocido en linea " +
                                         std::to_string(numLinea) + ": " + c[0]);
            }
        }
    } catch (...) {
        liberarProductos(productos);
        throw;
    }
    return productos;
}

void GestorArchivos::guardarClientes(const std::vector<Cliente>& clientes,
                                     const std::string& ruta) const {
    std::ofstream out = abrirSalida(ruta, true);
    escribirValor<size_t>(out, clientes.size());
    for (const Cliente& c : clientes) {
        escribirString(out, c.getNombre());
        escribirString(out, c.getCorreo());
        escribirString(out, c.getDireccionEnvio());
        escribirValor<float>(out, c.getSaldoDisponible());
    }
    if (!out) throw std::runtime_error("Error al escribir clientes: " + ruta);
}

std::vector<Cliente> GestorArchivos::cargarClientes(const std::string& ruta) const {
    std::ifstream in(ruta, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    std::vector<Cliente> clientes;
    size_t n = leerValor<size_t>(in);
    for (size_t i = 0; i < n; ++i) {
        std::string nombre = leerString(in);
        std::string correo = leerString(in);
        std::string direccion = leerString(in);
        float saldo = leerValor<float>(in);
        clientes.emplace_back(nombre, correo, direccion, saldo);
    }
    return clientes;
}

void GestorArchivos::guardarPedidos(const std::vector<Pedido*>& pedidos,
                                    const std::string& ruta) const {
    std::ofstream out = abrirSalida(ruta, true);
    escribirValor<size_t>(out, pedidos.size());
    for (const Pedido* p : pedidos) {
        escribirString(out, p->getNumeroSeguimiento());
        escribirString(out, p->getFecha());
        escribirString(out, p->getEstado());

        const auto& detalles = p->getDetalles();
        escribirValor<size_t>(out, detalles.size());
        for (const DetallePedido* d : detalles) {
            escribirString(out, d->getProducto()->getCodigo());
            escribirValor<int>(out, d->getCantidad());
        }
    }
    if (!out) throw std::runtime_error("Error al escribir pedidos: " + ruta);
}

std::vector<Pedido*> GestorArchivos::cargarPedidos(
        const std::string& ruta, const std::vector<Producto*>& catalogo) const {
    std::ifstream in(ruta, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    std::vector<Pedido*> pedidos;
    Pedido* actual = nullptr;
    try {
        size_t n = leerValor<size_t>(in);
        for (size_t i = 0; i < n; ++i) {
            std::string seguimiento = leerString(in);
            std::string fecha = leerString(in);
            std::string estado = leerString(in);

            actual = new Pedido(seguimiento, fecha);

            size_t nDetalles = leerValor<size_t>(in);
            for (size_t j = 0; j < nDetalles; ++j) {
                std::string codigo = leerString(in);
                int cantidad = leerValor<int>(in);

                const Producto* encontrado = nullptr;
                for (const Producto* prod : catalogo) {
                    if (prod->getCodigo() == codigo) { encontrado = prod; break; }
                }
                if (!encontrado) {
                    throw std::runtime_error("Producto " + codigo +
                                             " del pedido " + seguimiento +
                                             " no existe en el catalogo");
                }
                actual->agregarDetalle(encontrado, cantidad);
            }
            actual->calcularTotal();
            actual->setEstado(estado);

            pedidos.push_back(actual);
            actual = nullptr;
        }
    } catch (...) {
        delete actual;
        liberarPedidos(pedidos);
        throw;
    }
    return pedidos;
}

void GestorArchivos::guardarReporte(const std::vector<Pedido*>& pedidos,
                                    const std::string& ruta) const {
    std::ofstream out = abrirSalida(ruta, false);
    out << "=========== REPORTE DE PEDIDOS - MERCADOLIBRE EXPRESS ===========\n\n";

    float granTotal = 0.0f;
    for (const Pedido* p : pedidos) {
        out << "Pedido:  " << p->getNumeroSeguimiento() << '\n'
            << "Fecha:   " << p->getFecha() << '\n'
            << "Estado:  " << p->getEstado() << '\n'
            << std::left << std::setw(28) << "  Producto"
            << std::setw(10) << "Cant."
            << "Subtotal\n";

        for (const DetallePedido* d : p->getDetalles()) {
            out << "  " << std::left << std::setw(26) << d->getProducto()->getNombre()
                << std::setw(10) << d->getCantidad()
                << std::fixed << std::setprecision(2) << d->getSubtotal() << '\n';
        }
        out << "  TOTAL: " << std::fixed << std::setprecision(2)
            << p->getMontoTotal() << "\n"
            << "-----------------------------------------------------------\n";
        granTotal += p->getMontoTotal();
    }
    out << "\nPedidos: " << pedidos.size()
        << " | Total general: " << std::fixed << std::setprecision(2) << granTotal << '\n';
    if (!out) throw std::runtime_error("Error al escribir el reporte: " + ruta);
}

void GestorArchivos::liberarProductos(std::vector<Producto*>& productos) {
    for (Producto* p : productos) delete p;
    productos.clear();
}

void GestorArchivos::liberarPedidos(std::vector<Pedido*>& pedidos) {
    for (Pedido* p : pedidos) delete p;
    pedidos.clear();
}
