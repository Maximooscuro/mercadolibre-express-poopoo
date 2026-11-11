#include "GestorArchivos.h"
#include "ProductoFisico.h"
#include "ProductoDigital.h"

#include <sstream>
#include <stdexcept>

void GestorArchivos::escribirInt(std::ofstream& out, int valor) {
    out.write(reinterpret_cast<const char*>(&valor), sizeof(int));
}

int GestorArchivos::leerInt(std::ifstream& in) {
    int valor = 0;
    in.read(reinterpret_cast<char*>(&valor), sizeof(int));
    if (!in) throw std::runtime_error("Archivo binario corrupto o incompleto");
    return valor;
}

void GestorArchivos::escribirFloat(std::ofstream& out, float valor) {
    out.write(reinterpret_cast<const char*>(&valor), sizeof(float));
}

float GestorArchivos::leerFloat(std::ifstream& in) {
    float valor = 0;
    in.read(reinterpret_cast<char*>(&valor), sizeof(float));
    if (!in) throw std::runtime_error("Archivo binario corrupto o incompleto");
    return valor;
}

void GestorArchivos::escribirString(std::ofstream& out, const std::string& texto) {
    int largo = texto.size();
    escribirInt(out, largo);
    out.write(texto.c_str(), largo);
}

std::string GestorArchivos::leerString(std::ifstream& in) {
    int largo = leerInt(in);
    if (largo < 0 || largo > 1000000) {
        throw std::runtime_error("Archivo binario corrupto (largo de texto invalido)");
    }
    std::string texto(largo, ' ');
    in.read(&texto[0], largo);
    if (!in) throw std::runtime_error("Archivo binario corrupto o incompleto");
    return texto;
}

void GestorArchivos::guardarCatalogoTexto(const std::string& ruta,
                                          const std::vector<Producto*>& productos) const {
    std::ofstream out(ruta);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);

    for (const Producto* p : productos) {
        const ProductoFisico* fisico = dynamic_cast<const ProductoFisico*>(p);
        const ProductoDigital* digital = dynamic_cast<const ProductoDigital*>(p);

        if (fisico != nullptr) {
            out << "FISICO|" << fisico->getCodigo() << '|' << fisico->getNombre() << '|'
                << fisico->getDescripcion() << '|' << fisico->getPrecio() << '|'
                << fisico->getStock() << '|' << fisico->getPeso() << '|'
                << fisico->getDimensiones() << '\n';
        } else if (digital != nullptr) {
            out << "DIGITAL|" << digital->getCodigo() << '|' << digital->getNombre() << '|'
                << digital->getDescripcion() << '|' << digital->getPrecio() << '|'
                << digital->getStock() << '|' << digital->getTamanoArchivo() << '|'
                << digital->getUrlDescarga() << '\n';
        }
    }
    if (!out) throw std::runtime_error("Error al escribir el catalogo: " + ruta);
}

std::vector<Producto*> GestorArchivos::cargarCatalogoTexto(const std::string& ruta) const {
    std::ifstream in(ruta);
    if (!in.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    std::vector<Producto*> productos;
    try {
        std::string linea;
        int numLinea = 0;
        while (std::getline(in, linea)) {
            numLinea++;
            if (!linea.empty() && linea.back() == '\r') linea.pop_back();
            if (linea.empty() || linea[0] == '#') continue;

            std::stringstream ss(linea);
            std::string tipo, codigo, nombre, descripcion, sPrecio, sStock, sDato1, dato2;
            std::getline(ss, tipo, '|');
            std::getline(ss, codigo, '|');
            std::getline(ss, nombre, '|');
            std::getline(ss, descripcion, '|');
            std::getline(ss, sPrecio, '|');
            std::getline(ss, sStock, '|');
            std::getline(ss, sDato1, '|');
            std::getline(ss, dato2);

            if (ss.fail()) {
                throw std::runtime_error("Linea " + std::to_string(numLinea) +
                                         " del catalogo con formato invalido");
            }

            float precio = std::stof(sPrecio);
            int stock = std::stoi(sStock);
            float dato1 = std::stof(sDato1);

            if (tipo == "FISICO") {
                productos.push_back(new ProductoFisico(codigo, nombre, descripcion,
                                                       precio, stock, dato1, dato2));
            } else if (tipo == "DIGITAL") {
                productos.push_back(new ProductoDigital(codigo, nombre, descripcion,
                                                        precio, stock, dato1, dato2));
            } else {
                throw std::runtime_error("Tipo de producto desconocido en la linea " +
                                         std::to_string(numLinea));
            }
        }
    } catch (...) {
        liberarProductos(productos);
        throw;
    }
    return productos;
}

void GestorArchivos::guardarClientesBinario(const std::string& ruta,
                                            const std::vector<Cliente>& clientes) const {
    std::ofstream out(ruta, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);

    escribirInt(out, clientes.size());
    for (const Cliente& c : clientes) {
        escribirString(out, c.getNombre());
        escribirString(out, c.getCorreo());
        escribirString(out, c.getDireccionEnvio());
        escribirFloat(out, c.getSaldoDisponible());
    }
    if (!out) throw std::runtime_error("Error al escribir clientes: " + ruta);
}

std::vector<Cliente> GestorArchivos::cargarClientesBinario(const std::string& ruta) const {
    std::ifstream in(ruta, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    std::vector<Cliente> clientes;
    int cantidad = leerInt(in);
    for (int i = 0; i < cantidad; i++) {
        std::string nombre = leerString(in);
        std::string correo = leerString(in);
        std::string direccion = leerString(in);
        float saldo = leerFloat(in);
        clientes.push_back(Cliente(nombre, correo, direccion, saldo));
    }
    return clientes;
}

void GestorArchivos::guardarPedidosBinario(const std::string& ruta,
                                           const std::vector<Pedido*>& pedidos) const {
    std::ofstream out(ruta, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);

    escribirInt(out, pedidos.size());
    for (const Pedido* p : pedidos) {
        escribirString(out, p->getNumeroSeguimiento());
        escribirString(out, p->getFecha());
        escribirString(out, p->getEstado());

        const std::vector<DetallePedido*>& detalles = p->getDetalles();
        escribirInt(out, detalles.size());
        for (const DetallePedido* d : detalles) {
            escribirString(out, d->getProducto()->getCodigo());
            escribirInt(out, d->getCantidad());
        }
    }
    if (!out) throw std::runtime_error("Error al escribir pedidos: " + ruta);
}

std::vector<Pedido*> GestorArchivos::cargarPedidosBinario(
        const std::string& ruta, const std::vector<Producto*>& catalogo) const {
    std::ifstream in(ruta, std::ios::binary);
    if (!in.is_open()) throw std::runtime_error("No se pudo abrir: " + ruta);

    std::vector<Pedido*> pedidos;
    Pedido* actual = nullptr;
    try {
        int cantidadPedidos = leerInt(in);
        for (int i = 0; i < cantidadPedidos; i++) {
            std::string seguimiento = leerString(in);
            std::string fecha = leerString(in);
            std::string estado = leerString(in);

            actual = new Pedido(seguimiento, fecha);

            int cantidadDetalles = leerInt(in);
            for (int j = 0; j < cantidadDetalles; j++) {
                std::string codigo = leerString(in);
                int cantidad = leerInt(in);

                const Producto* encontrado = nullptr;
                for (const Producto* prod : catalogo) {
                    if (prod->getCodigo() == codigo) encontrado = prod;
                }
                if (encontrado == nullptr) {
                    throw std::runtime_error("Producto " + codigo + " del pedido " +
                                             seguimiento + " no existe en el catalogo");
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

void GestorArchivos::guardarReporteTexto(const std::string& ruta,
                                         const std::vector<Pedido*>& pedidos) const {
    std::ofstream out(ruta);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);

    out << "REPORTE DE PEDIDOS - MERCADOLIBRE EXPRESS\n\n";
    float granTotal = 0;
    for (const Pedido* p : pedidos) {
        out << "Pedido: " << p->getNumeroSeguimiento() << '\n';
        out << "Fecha: " << p->getFecha() << '\n';
        out << "Estado: " << p->getEstado() << '\n';
        for (const DetallePedido* d : p->getDetalles()) {
            out << "  " << d->getProducto()->getNombre() << " x" << d->getCantidad()
                << " = " << d->getSubtotal() << '\n';
        }
        out << "Total: " << p->getMontoTotal() << "\n\n";
        granTotal += p->getMontoTotal();
    }
    out << "Total general: " << granTotal << '\n';
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
