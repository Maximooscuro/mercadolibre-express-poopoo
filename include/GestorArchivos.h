#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Producto.h"
#include "Cliente.h"
#include "Pedido.h"

// Se encarga de guardar y cargar catalogo, clientes y pedidos en archivos
class GestorArchivos {
private:
    //auxiliares para leer y escribir en binario
    static void escribirInt(std::ofstream& out, int valor);
    static int leerInt(std::ifstream& in);

    static void escribirFloat(std::ofstream& out, float valor);
    static float leerFloat(std::ifstream& in);

    static void escribirString(std::ofstream& out, const std::string& texto);
    static std::string leerString(std::ifstream& in);

public:
    //catalogo (archivo de texto)
    void guardarCatalogoTexto(const std::string& ruta,
                              const std::vector<Producto*>& productos) const;
    std::vector<Producto*> cargarCatalogoTexto(const std::string& ruta) const;

    //clientes (binario)
    void guardarClientesBinario(const std::string& ruta,
                                const std::vector<Cliente>& clientes) const;
    std::vector<Cliente> cargarClientesBinario(const std::string& ruta) const;

    //pedidos (binario), cargarPedidos necesita el catalogo para enlazar los productos
    void guardarPedidosBinario(const std::string& ruta,
                               const std::vector<Pedido*>& pedidos) const;
    std::vector<Pedido*> cargarPedidosBinario(const std::string& ruta,
                                              const std::vector<Producto*>& catalogo) const;

    //reporte de pedidos en texto
    void guardarReporteTexto(const std::string& ruta,
                             const std::vector<Pedido*>& pedidos) const;

    //liberar memoria, se llaman sin crear un objeto: GestorArchivos::liberarProductos(v)
    static void liberarProductos(std::vector<Producto*>& productos);
    static void liberarPedidos(std::vector<Pedido*>& pedidos);
};
