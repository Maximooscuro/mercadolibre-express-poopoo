// ============================================================================
// main_prueba.cpp
// Programa de PRUEBA del backend. Sirve como casos de prueba para demostrar
// polimorfismo y manejo de excepciones.
// NO agregar este archivo al CMakeLists principal: tiene su propio main() y
// chocaria con el main.cpp real del proyecto.
// ============================================================================
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
    // ---- Prueba 1: polimorfismo ----
    // Un vector de Producto* puede guardar fisicos y digitales mezclados.
    cout << "=== Prueba 1: polimorfismo de productos ===" << endl;
    vector<Producto*> catalogo;
    catalogo.push_back(new ProductoFisico("P001", "Audifonos", "Bluetooth", 80000, 10, 0.3f, "10x8x5"));
    catalogo.push_back(new ProductoDigital("D001", "Curso C++", "Curso en video", 50000, 100, 500, "http://descargas.com/curso"));

    for (size_t i = 0; i < catalogo.size(); i++) {
        // Misma llamada, comportamiento distinto segun el tipo real del objeto.
        catalogo[i]->mostrarDetalles();
        cout << "  Costo de 2 unidades: $" << catalogo[i]->calcularCostoTotal(2) << endl;
        // dynamic_cast: intenta convertir a ProductoFisico. Si el objeto no es
        // fisico (es digital), devuelve nullptr y no entra al if.
        ProductoFisico* f = dynamic_cast<ProductoFisico*>(catalogo[i]);
        if (f != nullptr) {
            cout << "  (es fisico, envio: $" << f->calcularCostoEnvio() << ")" << endl;
        }
    }

    // ---- Prueba 2: stock agotado ----
    // Pedimos 50 unidades cuando solo hay 10. try/catch atrapa el error.
    cout << "\n=== Prueba 2: stock agotado ===" << endl;
    try {
        catalogo[0]->reducirStock(50);
    } catch (const StockAgotadoException& e) {
        cout << "Error: " << e.what() << endl;   // what() trae el mensaje
    }

    // ---- Prueba 3: saldo insuficiente ----
    cout << "\n=== Prueba 3: saldo insuficiente ===" << endl;
    Cliente cli("Ana", "ana@correo.com", "Calle 1 # 2-3", 20000);
    try {
        cli.descontarSaldo(100000);   // tiene 20000, no alcanza
    } catch (const SaldoInsuficienteException& e) {
        cout << "Error: " << e.what() << endl;
    }

    // ---- Prueba 4: pedido con pago polimorfico ----
    cout << "\n=== Prueba 4: pedido con pago polimorfico ===" << endl;
    ServicioNotificacion notificador;
    TarjetaCredito tarjeta("1234567812345678", "Ana Perez", "12/28", "123");
    TransferenciaBancaria transf("Bancolombia", "00012345");
    // Un arreglo de MetodoPago*: guarda tarjeta y transferencia juntos.
    MetodoPago* metodos[2] = { &tarjeta, &transf };
    for (int i = 0; i < 2; i++) {
        cout << "Metodo disponible: " << metodos[i]->getNombreMetodo() << endl;
    }

    Pedido* p1 = new Pedido("PED-001", "02/11/2026");
    p1->agregarDetalle(catalogo[0], 2);   // 2 audifonos (fisico)
    p1->agregarDetalle(catalogo[1], 1);   // 1 curso (digital)
    cout << "Total: $" << p1->getMontoTotal() << endl;
    p1->procesarOrden(*metodos[0], notificador, cli.getCorreo());   // paga con tarjeta
    cout << "Estado: " << p1->getEstado() << endl;

    // ---- Prueba 5: pagos invalidos ----
    cout << "\n=== Prueba 5: pagos invalidos ===" << endl;
    try {
        transf.procesarPago(-500);   // monto negativo
    } catch (const MontoInvalidoException& e) {
        cout << "Error: " << e.what() << endl;
    }
    TarjetaCredito mala("123", "Luis", "01/27", "99");   // numero y cvv incorrectos
    try {
        mala.procesarPago(1000);
    } catch (const MontoInvalidoException& e) {
        cout << "Error: " << e.what() << endl;
    }

    // ---- Prueba 6: seguimiento duplicado ----
    cout << "\n=== Prueba 6: seguimiento duplicado ===" << endl;
    try {
        Pedido p2("PED-001", "02/11/2026");   // ese numero ya se uso
    } catch (const SeguimientoDuplicadoException& e) {
        cout << "Error: " << e.what() << endl;
    }

    // ---- Prueba 7: agregacion ----
    // Al borrar la categoria, los productos deben seguir existiendo.
    cout << "\n=== Prueba 7: agregacion (categoria no borra productos) ===" << endl;
    Categoria* cat = new Categoria("C01", "Tecnologia");
    cat->agregarProducto(catalogo[0]);
    cat->agregarProducto(catalogo[1]);
    cout << "Productos en la categoria: " << cat->getProductos().size() << endl;
    delete cat;
    cout << "Despues de borrar la categoria, el producto sigue vivo: " << *catalogo[0] << endl;

    // ---- Liberar memoria ----
    delete p1;   // el destructor de Pedido tambien borra sus detalles (composicion)
    for (size_t i = 0; i < catalogo.size(); i++) {
        delete catalogo[i];   // destructor virtual: borra el objeto completo
    }
    catalogo.clear();
    return 0;
}
