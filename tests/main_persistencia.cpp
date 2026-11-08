#include <iostream>
#include "GestorArchivos.h"
#include "ProductoFisico.h"
#include "ProductoDigital.h"

int main() {
    GestorArchivos gestor;

    try {
        std::vector<Producto*> catalogo;
        catalogo.push_back(new ProductoFisico("P001", "Mouse", "Mouse inalambrico",
                                              45000.0f, 20, 0.15f, "10x6x4"));
        catalogo.push_back(new ProductoDigital("P002", "Ebook C++", "Guia de POO",
                                               30000.0f, 999, 5.2f, "http://descarga.com/ebook"));

        gestor.guardarCatalogoTexto("data/catalogo.txt", catalogo);
        GestorArchivos::liberarProductos(catalogo);

        std::vector<Producto*> cargado = gestor.cargarCatalogoTexto("data/catalogo.txt");
        std::cout << "Productos cargados: " << cargado.size() << "\n";
        for (const Producto* p : cargado) std::cout << *p << "\n";

        std::vector<Cliente> clientes;
        clientes.push_back(Cliente("Ana Perez", "ana@correo.com", "Calle 1 #2-3", 100000.0f));
        gestor.guardarClientesBinario("data/clientes.dat", clientes);

        std::vector<Cliente> clientesLeidos = gestor.cargarClientesBinario("data/clientes.dat");
        std::cout << "Cliente cargado: " << clientesLeidos[0].getNombre()
                  << " saldo " << clientesLeidos[0].getSaldoDisponible() << "\n";

        std::vector<Pedido*> pedidos;
        Pedido* ped = new Pedido("TRK-001", "2026-11-08");
        ped->agregarDetalle(cargado[0], 2);
        ped->agregarDetalle(cargado[1], 1);
        ped->calcularTotal();
        ped->setEstado("Pagado");
        pedidos.push_back(ped);

        gestor.guardarPedidosBinario("data/pedidos.dat", pedidos);
        gestor.guardarReporteTexto("data/reporte.txt", pedidos);
        GestorArchivos::liberarPedidos(pedidos);

        std::vector<Pedido*> pedidosLeidos = gestor.cargarPedidosBinario("data/pedidos.dat", cargado);
        std::cout << "Pedido cargado: " << pedidosLeidos[0]->getNumeroSeguimiento()
                  << " estado " << pedidosLeidos[0]->getEstado()
                  << " total " << pedidosLeidos[0]->getMontoTotal() << "\n";

        GestorArchivos::liberarPedidos(pedidosLeidos);
        GestorArchivos::liberarProductos(cargado);

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << "\n";
        return 1;
    }
    std::cout << "Prueba terminada.\n";
    return 0;
}
