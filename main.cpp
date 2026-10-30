#include <iostream>

// Inclusión de todas las cabeceras de arquitectura
#include "include/Excepciones.h"
#include "include/Producto.h"
#include "include/ProductoFisico.h"
#include "include/ProductoDigital.h"
#include "include/MetodoPago.h"
#include "include/TarjetaCredito.h"
#include "include/TransferenciaBancaria.h"
#include "include/DetallePedido.h"
#include "include/Pedido.h"
#include "include/Cliente.h"
#include "include/Categoria.h"
#include "include/ServicioNotificacion.h"

int main() {
    // Comprobación de que el compilador reconoce los tipos y punteros
    Producto* pPtr = nullptr;
    MetodoPago* mPtr = nullptr;
    Pedido* pedPtr = nullptr;
    Cliente* cPtr = nullptr;
    Categoria* catPtr = nullptr;

    // Comprobación de que las excepciones se pueden instanciar
    try {
        throw SaldoInsuficienteException("Prueba exitosa de excepciones.");
    } catch (const std::exception& e) {
        std::cout << "[OK] Excepciones operativas: " << e.what() << "\n";
    }

    std::cout << "[OK] Todas las cabeceras (.h) compilaron sin errores de sintaxis ni dependencias circulares.\n";
    return 0;
}