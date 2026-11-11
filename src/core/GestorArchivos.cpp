#include "GestorArchivos.h"
#include "ProductoFisico.h"
#include "ProductoDigital.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>

//cosas de uso interno, solo se ven dentro de este archivo
namespace {

// Pedido no trae setEstado(), asi que para dejarlo en Pagado o Rechazado
// toca pasar por procesarOrden() con un pago que ya sabe si aprueba o no
class PagoRestaurado : public MetodoPago {
    bool aprobar;
public:
    explicit PagoRestaurado(bool a) : aprobar(a) {}
    bool procesarPago(float) override { return aprobar; }
    std::string getNombreMetodo() const override { return "Restaurado"; }
};

void restaurarEstado(Pedido& pedido, const std::string& estado) {
    // "Pendiente" es con lo que nace el pedido, ahi no hay nada que hacer
    if (estado != "Pagado" && estado != "Rechazado") return;
    if (pedido.getDetalles().empty()) return;

    PagoRestaurado pago(estado == "Pagado");
    ServicioNotificacion servicio;

    // procesarOrden imprime el correo de confirmacion, asi que mando el cout
    // a un stream falso para que no salga en pantalla cada vez que se carga
    std::ostringstream silencio;
    std::streambuf* original = std::cout.rdbuf(silencio.rdbuf());
    try {
        pedido.procesarOrden(pago, servicio, "");
    } catch (...) {
        std::cout.rdbuf(original);
        throw;
    }
    std::cout.rdbuf(original); // el cout vuelve a la normalidad
}

}

//funciones auxiliares para los archivos binarios:
// escribe los bytes del int tal cual estan en memoria
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

// el string se guarda con su largo primero, asi al leer se sabe cuantos caracteres son
void GestorArchivos::escribirString(std::ofstream& out, const std::string& texto) {
    int largo = texto.size();
    escribirInt(out, largo);
    out.write(texto.c_str(), largo);
}

std::string GestorArchivos::leerString(std::ifstream& in) {
    int largo = leerInt(in);
    // un largo absurdo significa que el archivo esta danado, mejor no reservar esa memoria
    if (largo < 0 || largo > 1000000) {
        throw std::runtime_error("Archivo binario corrupto (largo de texto invalido)");
    }
    std::string texto(largo, ' ');
    in.read(&texto[0], largo);
    if (!in) throw std::runtime_error("Archivo binario corrupto o incompleto");
    return texto;
}

//catalogo en archivo de texto, una linea por producto:
//TIPO|codigo|nombre|descripcion|precio|stock|peso_o_tamano|dimensiones_o_url
void GestorArchivos::guardarCatalogoTexto(const std::string& ruta,
                                          const std::vector<Producto*>& productos) const {
    std::ofstream out(ruta);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);

    // 7 cifras para que un precio como 1234567 no pierda digitos
    out << std::setprecision(7);

    for (const Producto* p : productos) {
        // dynamic_cast para saber si el producto es fisico o digital
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
            // el \r aparece si el archivo se edito en windows
            if (!linea.empty() && linea.back() == '\r') linea.pop_back();
            // lineas vacias y comentarios (#) se saltan
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

            // si algun numero viene mal escrito stof/stoi lanzan excepcion y cae al catch
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
        // si falla a la mitad se borra lo que ya se creo con new y se relanza el error
        liberarProductos(productos);
        throw;
    }
    return productos;
}

//clientes en binario: primero la cantidad y luego los datos de cada uno
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

    // se lee en el mismo orden en que se guardo
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

//pedidos en binario. Del producto solo se guarda el codigo porque un puntero
//no sirve de un dia para otro (la direccion cambia en cada ejecucion).
//La fecha no se guarda porque Pedido no tiene getter para ella
void GestorArchivos::guardarPedidosBinario(const std::string& ruta,
                                           const std::vector<Pedido*>& pedidos) const {
    std::ofstream out(ruta, std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);

    escribirInt(out, pedidos.size());
    for (const Pedido* p : pedidos) {
        escribirString(out, p->getNumeroSeguimiento());
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
    Pedido* actual = nullptr; // el pedido que se esta armando, todavia no esta en el vector
    try {
        int cantidadPedidos = leerInt(in);
        for (int i = 0; i < cantidadPedidos; i++) {
            std::string seguimiento = leerString(in);
            std::string estado = leerString(in);

            // como no se guardo la fecha se pone un texto fijo
            actual = new Pedido(seguimiento, "sin fecha");

            int cantidadDetalles = leerInt(in);
            for (int j = 0; j < cantidadDetalles; j++) {
                std::string codigo = leerString(in);
                int cantidad = leerInt(in);

                // con el codigo se busca el producto en el catalogo para volver a enlazarlo
                Producto* encontrado = nullptr;
                for (Producto* prod : catalogo) {
                    if (prod->getCodigo() == codigo) encontrado = prod;
                }
                if (encontrado == nullptr) {
                    throw std::runtime_error("Producto " + codigo + " del pedido " +
                                             seguimiento + " no existe en el catalogo");
                }

                // un pedido viejo pudo comprar mas unidades de las que hay hoy en stock
                // y DetallePedido lo rechazaria, asi que se sube el stock solo mientras
                // se agrega el detalle y despues queda como estaba
                int faltante = cantidad - encontrado->getStock();
                if (faltante > 0) encontrado->aumentarStock(faltante);
                try {
                    actual->agregarDetalle(encontrado, cantidad);
                } catch (...) {
                    if (faltante > 0) encontrado->reducirStock(faltante);
                    throw;
                }
                if (faltante > 0) encontrado->reducirStock(faltante);
            }
            actual->calcularTotal();
            restaurarEstado(*actual, estado);

            pedidos.push_back(actual);
            actual = nullptr; // ya esta completo, ahora lo maneja el vector
        }
    } catch (...) {
        // se borra el pedido que quedo a medias y los que ya estaban completos
        delete actual;
        liberarPedidos(pedidos);
        throw;
    }
    return pedidos;
}

//reporte legible de todos los pedidos con el total general
void GestorArchivos::guardarReporteTexto(const std::string& ruta,
                                         const std::vector<Pedido*>& pedidos) const {
    std::ofstream out(ruta);
    if (!out.is_open()) throw std::runtime_error("No se pudo abrir para escribir: " + ruta);

    out << std::fixed << std::setprecision(2);  // montos legibles: 1239670.00
    out << "REPORTE DE PEDIDOS - MERCADOLIBRE EXPRESS\n\n";
    float granTotal = 0;
    for (const Pedido* p : pedidos) {
        out << "Pedido: " << p->getNumeroSeguimiento() << '\n';
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

//limpieza de memoria: delete de cada puntero y se vacia el vector
void GestorArchivos::liberarProductos(std::vector<Producto*>& productos) {
    for (Producto* p : productos) delete p;
    productos.clear();
}

void GestorArchivos::liberarPedidos(std::vector<Pedido*>& pedidos) {
    for (Pedido* p : pedidos) delete p;
    pedidos.clear();
}
