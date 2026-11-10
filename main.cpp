#include <iostream>
#include <vector>
#include "include/ProductoFisico.h"
#include "include/Cliente.h"
#include "include/Categoria.h"
#include "include/Pedido.h"
#include "include/DetallePedido.h"
#include "include/TarjetaCredito.h"
#include "include/ServicioNotificacion.h"
#include "include/Excepciones.h"

using namespace std;

int main() {
    cout << "=========================================\n";
    cout << "  PRUEBA RAPIDA - MERCADOLIBRE EXPRESS   \n";
    cout << "=========================================\n\n";

    try {
        // 1. Instanciar un cliente de prueba
        Cliente cliente("Samuel Sapuy", "samuel@unillanos.edu.co", "Villavicencio", 500000.0f);
        cout << "[OK] Cliente creado: " << cliente.getNombre()
             << " | Saldo disponible: $" << cliente.getSaldoDisponible() << endl;

        // 2. Instanciar un producto físico (Herencia y polimorfismo)
        Producto* audifonos = new ProductoFisico("P001", "Audifonos Gamer RGB", "Inalambricos de alta fidelidad", 120000.0f, 10, 0.4f, "15x10x5");
        cout << "[OK] Producto físico instanciado correctamente.\n";

        // 3. Crear una categoría y asociar el producto (Agregación)
        Categoria catTecnologia("C01", "Tecnologia y Accesorios");
        catTecnologia.agregarProducto(audifonos);
        cout << "[OK] Producto agregado a la categoría: " << catTecnologia.getNombre() << endl;

        // 4. Crear un pedido (Composición)
        Pedido* miPedido = new Pedido("PED-2026", "10/11/2026");

        // 5. Agregar detalles al pedido (Valida stock internamente)
        miPedido->agregarDetalle(audifonos, 2); // 2 unidades de audífonos
        cout << "[OK] Detalle agregado con éxito. Costo total del pedido (con envío): $"
             << miPedido->getMontoTotal() << endl;

        // 6. Configurar la pasarela de pago polimórfica y el notificador
        TarjetaCredito pasarelaPago("1234567812345678", cliente.getNombre(), "12/28", "456");
        ServicioNotificacion notificador;

        // 7. Procesar la orden completa
        cout << "\n--- Procesando transacción ---" << endl;
        bool resultado = miPedido->procesarOrden(pasarelaPago, notificador, cliente.getCorreo());

        if (resultado) {
            cout << "[EXITO] ¡La orden se procesó y el estado actual es: " << miPedido->getEstado() << "!" << endl;
        }

        // 8. Liberación de memoria dinámica propia del main
        delete miPedido;
        delete audifonos;
        cout << "\n[OK] Memoria liberada correctamente sin fugas." << endl;

    } catch (const exception& e) {
        // Manejo controlado de cualquier error o regla de negocio rota
        cerr << "\n[ERROR CAPTURADO]: " << e.what() << endl;
    }

    cout << "\n=========================================\n";
    cout << "      FIN DE LA PRUEBA EN CONSOLA        \n";
    cout << "=========================================\n";

    return 0;
}