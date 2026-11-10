#include <iostream>
#include <vector>
#include <memory>
#include "include/Producto.h"
#include "include/ProductoFisico.h"
#include "include/ProductoDigital.h"
#include "include/Categoria.h"
#include "include/Pedido.h"
#include "include/DetallePedido.h"
#include "include/TarjetaCredito.h"

using namespace std;

void mostrarMenu() {
    cout << "\n=========================================\n";
    cout << "   MERCADOLIBRE EXPRESS - INTEGRACION     \n";
    cout << "=========================================\n";
    cout << "1. Ver Catalogo de Productos\n";
    cout << "2. Simular Compra / Crear Pedido\n";
    cout << "3. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main() {
    // 1. Inicializacion del Catálogo en Memoria (Base para Santiago / Persistencia)
    vector<Producto*> catalogo;
    catalogo.push_back(new ProductoFisico("P001", "Audifonos Gamer", "Inalambricos RGB", 120000, 15, 0.4f, "15x10x8"));
    catalogo.push_back(new ProductoDigital("D001", "Curso C++ Avanzado", "Plataforma online", 75000, 500, 150, "https://link.com/curso"));

    Categoria catTecnologia("C01", "Tecnologia");
    catTecnologia.agregarProducto(catalogo[0]);
    catTecnologia.agregarProducto(catalogo[1]);

    int opcion = 0;

    // 2. Bucle Interactivo Principal (Base para la futura GUI de Paopi)
    do {
        mostrarMenu();
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "\n--- CATALOGO DISPONIBLE ---\n";
                for (size_t i = 0; i < catalogo.size(); i++) {
                    cout << "[" << i << "] ";
                    catalogo[i]->mostrarDetalles();
                    cout << "---------------------------\n";
                }
                break;
            }
            case 2: {
                cout << "\n--- SIMULACION DE COMPRA ---\n";
                string codigoPedido = "PED-" + to_string(rand() % 900 + 100);
                
                // Aplicando Composición
                Pedido* nuevoPedido = new Pedido(codigoPedido, "09/11/2026");
                nuevoPedido->agregarDetalle(new DetallePedido(catalogo[0], 2));

                double total = nuevoPedido->calcularTotal();
                cout << "Pedido " << codigoPedido << " creado. Total a pagar: $" << total << endl;

                // Aplicando Polimorfismo en Pagos
                TarjetaCredito pago("1234567812345678", "Lider Tecnico", "12/28", "123");
                if (pago.procesarPago(total)) {
                    cout << "¡Pago procesado con exito!\n";
                } else {
                    cout << "Pago rechazado.\n";
                }

                delete nuevoPedido; // Limpieza estricta de composición
                break;
            }
            case 3:
                cout << "\nCerrando sistema base...\n";
                break;
            default:
                cout << "\nOpcion no valida.\n";
        }
    } while (opcion != 3);

    // 3. Limpieza global del heap
    for (size_t i = 0; i < catalogo.size(); i++) {
        delete catalogo[i];
    }
    catalogo.clear();

    return 0;
}