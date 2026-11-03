#pragma once
#include <string>
#include <vector>
#include "Producto.h"
#include "Cliente.h"
#include "Pedido.h"

class GestorArchivos {
public:
    void guardarCatalogo(const std::vector<Producto*>& productos,
                         const std::string& ruta) const;
    std::vector<Producto*> cargarCatalogo(const std::string& ruta) const;

    void guardarClientes(const std::vector<Cliente>& clientes,
                         const std::string& ruta) const;
    std::vector<Cliente> cargarClientes(const std::string& ruta) const;

    void guardarPedidos(const std::vector<Pedido*>& pedidos,
                        const std::string& ruta) const;
    std::vector<Pedido*> cargarPedidos(const std::string& ruta,
                                       const std::vector<Producto*>& catalogo) const;

    void guardarReporte(const std::vector<Pedido*>& pedidos,
                        const std::string& ruta) const;

    static void liberarProductos(std::vector<Producto*>& productos);
    static void liberarPedidos(std::vector<Pedido*>& pedidos);
};
