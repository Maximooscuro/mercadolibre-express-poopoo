// Programa de prueba del backend (casos de prueba: polimorfismo y excepciones)
// NO agregar este archivo al CMakeLists principal: tiene su propio main().
#include <iostream>
#include <vector>
#include "ProductoFisico.h"
#include "ProductoDigital.h"
#include "TarjetaCredito.h"
#include "TransferenciaBancaria.h"
#include "Cliente.h"
#include "Categoria.h"
#include "Pedido.h"
#include "Excepciones.h"
using namespace std;

int main() {
    cout << "=== Prueba 1: polimorfismo de productos ===" << endl;
    vector<Producto*> catalogo;
    catalogo.push_back(new ProductoFisico("P001", "Audifonos", "Bluetooth", 80000, 10, 0.3f, "10x8x5"));
    catalogo.push_back(new ProductoDigital("D001", "Curso C++", "Curso en video", 50000, 100, 500, "http://descargas.com/curso"));

    for (size_t i = 0; i < catalogo.size(); i++) {
        catalogo[i]->mostrarDetalles();
        cout << "  Costo de 2 unidades: $" << catalogo[i]->calcularCostoTotal(2) << endl;
        ProductoFisico* f = dynamic_cast<ProductoFisico*>(catalogo[i]);
        if (f != nullptr) {
            cout << "  (es fisico, envio: $" << f->calcularCostoEnvio() << ")" << endl;
        }
    }

    cout << "\n=== Prueba 2: stock agotado ===" << endl;
    try {
        catalogo[0]->reducirStock(50);
    } catch (const StockAgotadoException& e) {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\n=== Prueba 3: saldo insuficiente ===" << endl;
    Cliente cli("Ana", "ana@correo.com", "Calle 1 # 2-3", 20000);
    try {
        cli.descontarSaldo(100000);
    } catch (const SaldoInsuficienteException& e) {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\n=== Prueba 4: pedido con pago polimorfico ===" << endl;
    ServicioNotificacion notificador;
    TarjetaCredito tarjeta("1234567812345678", "Ana Perez", "12/28", "123");
    TransferenciaBancaria transf("Bancolombia", "00012345");
    MetodoPago* metodos[2] = { &tarjeta, &transf };
    for (int i = 0; i < 2; i++) {
        cout << "Metodo disponible: " << metodos[i]->getNombreMetodo() << endl;
    }

    Pedido* p1 = new Pedido("PED-001", "02/11/2026");
    p1->agregarDetalle(catalogo[0], 2);
    p1->agregarDetalle(catalogo[1], 1);
    cout << "Total: $" << p1->getMontoTotal() << endl;
    p1->procesarOrden(*metodos[0], notificador, cli.getCorreo());
    cout << "Estado: " << p1->getEstado() << endl;

    cout << "\n=== Prueba 5: pagos invalidos ===" << endl;
    try {
        transf.procesarPago(-500);
    } catch (const MontoInvalidoException& e) {
        cout << "Error: " << e.what() << endl;
    }
    TarjetaCredito mala("123", "Luis", "01/27", "99");
    try {
        mala.procesarPago(1000);
    } catch (const MontoInvalidoException& e) {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\n=== Prueba 6: seguimiento duplicado ===" << endl;
    try {
        Pedido p2("PED-001", "02/11/2026");
    } catch (const SeguimientoDuplicadoException& e) {
        cout << "Error: " << e.what() << endl;
    }

    cout << "\n=== Prueba 7: agregacion (categoria no borra productos) ===" << endl;
    Categoria* cat = new Categoria("C01", "Tecnologia");
    cat->agregarProducto(catalogo[0]);
    cat->agregarProducto(catalogo[1]);
    cout << "Productos en la categoria: " << cat->getProductos().size() << endl;
    delete cat;
    cout << "Despues de borrar la categoria, el producto sigue vivo: " << *catalogo[0] << endl;

    delete p1;   // destruye tambien sus detalles
    for (size_t i = 0; i < catalogo.size(); i++) {
        delete catalogo[i];
    }
    catalogo.clear();
    return 0;
}
