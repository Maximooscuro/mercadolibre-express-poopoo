//
// Created by maxim on 10/11/2026.
//
#include "MenuController.h"
#include <iostream>
#include "ProductoFisico.h"

using namespace std;

// Pide un número entero y se queda atrapado aquí hasta que el usuario meta uno válido (evita que la consola explote con letras)
int MenuController::leerEnteroSeguro(int minVal, int maxVal) {
        int valor;
        while (true) {
            if (cin >> valor) {
                if (valor >= minVal && valor <= maxVal) {
                    return valor; // Si está en el rango, lo retorna y salimos
                }
                cout << "Error: El valor debe estar entre " << minVal << " y " << maxVal << ". Intente de nuevo: ";
            } else {
                cout << "Entrada invalida. Por favor ingrese un numero entero valido: ";
                cin.clear(); // Limpia el estado de error de cin
                cin.ignore(10000, '\n'); // Limpia la basura que haya quedado en el buffer
            }
        }
}

// Lo mismo que el anterior pero asegurándose de que el número con decimales sea mayor a cero
float MenuController::leerFlotantePositivo() {
    float valor;
    while (true) {
        if (cin >> valor) {
            if (valor > 0) {
                return valor;
            }
            cout << "Error: El valor debe ser mayor a cero. Intente de nuevo: ";
        } else {
            cout << "Entrada invalida. Por favor ingrese un numero valido: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }
    }
}

// Constructor: Precarga los datos iniciales para que la app no abra vacía
MenuController::MenuController(){
    // Mete un cliente por defecto y pone el puntero de sesión apuntando a él
    listaClientes.push_back(Cliente("Samuel Falsuy", "samuel@unillanos.edu.co", "Villavicencio", 500000.0f));
    clienteSesionActual = &listaClientes[0];
    
    // Crea un producto físico de prueba y una categoría inicial en memoria dinámica
    catalogo.push_back(new ProductoFisico("P001", "Audifonos Gamer RGB", "Inalambricos", 120000.0f, 10, 0.4f, "15x10x5"));
    Categoria* catTecnologia = new Categoria("C01", "Tecnologia");
    catTecnologia->agregarProducto(catalogo[0]);
    categorias.push_back(catTecnologia);
}

// Destructor: Borra toda la memoria dinámica (los 'new') para evitar fugas de RAM al cerrar
MenuController::~MenuController() {
    for (auto p : catalogo) delete p;
    for (auto c : categorias) delete c;
    catalogo.clear();
    categorias.clear();
}

// El bucle principal que mantiene la app viva mostrando el menú una y otra vez
void MenuController::ejecutar() {
    int opcion = 0;
    do {
        mostrarMenuPrincipal();
        cout << "Seleccione una opcion: ";
        opcion = leerEnteroSeguro(1, 5);
        switch (opcion) {
            case 1: gestionarProductos(); break;
            case 2: gestionarCategorias(); break;
            case 3: gestionarClientes(); break;
            case 4: realizarPedido(); break;
            case 5: cout << "\nSaliendo del sistema. ¡Hasta pronto!\n"; break;
            default: cout << "\nOpcion no valida.\n";
        }
        cout << "\n";
    } while (opcion != 5);
}

// Dibuja el panel principal en la consola
void MenuController::mostrarMenuPrincipal() const {
    cout << "=========================================\n";
    cout << "    MERCADOLIBRE EXPRESS - PANEL PRINCIPAL\n";
    cout << "=========================================\n";
    cout << "1. Gestion de Productos y Catalogo\n";
    cout << "2. Gestion de Categorias\n";
    cout << "3. Gestion de Clientes y Sesiones\n";
    cout << "4. Realizar Pedido (Carrito y Pago)\n";
    cout << "5. Salir\n";
    cout << "=========================================\n";
}

// Submenú para ver, crear o actualizar stock de los productos
void MenuController::gestionarProductos() {
    int op;
    cout << "\n--- GESTION DE PRODUCTOS ---\n";
    cout << "1. Ver catalogo completo\n";
    cout << "2. Anadir nuevo producto fisico\n";
    cout << "3. Modificar stock de un producto\n";
    cout << "Seleccione una opcion: ";
    op = leerEnteroSeguro(1, 3);
    
    switch (op) {
        case 1: {
            cout << "\n--- CATALOGO GENERAL ---\n";
            if (catalogo.empty()) {
                cout << "El catalogo esta vacio.\n";
                return;
            }
            // Recorre el vector imprimiendo los detalles de cada producto
            for (size_t i = 0; i < catalogo.size(); ++i) {
                cout << i + 1 << ". ";
                catalogo[i]->mostrarDetalles();
                cout << "----------------------------------\n";
            }
            break;
        }
        case 2: {
            try {
                string cod, nom, desc, dim;
                float prec, peso;
                int stk;

                cout << "Codigo: "; cin >> cod;
                cout << "Nombre: "; cin >> ws; getline(cin, nom);
                cout << "Descripcion: "; getline(cin, desc);
                cout << "Precio: "; prec = leerFlotantePositivo();
                cout << "Stock inicial: "; stk = leerEnteroSeguro(1, 1000);
                cout << "Peso (kg): "; peso = leerFlotantePositivo();
                cout << "Dimensiones (Ej: 10x10): "; cin >> dim;

                // Crea el producto con new y lo manda al vector del catálogo
                Producto* nuevo = new ProductoFisico(cod, nom, desc, prec, stk, peso, dim);
                catalogo.push_back(nuevo);
                cout << "[EXITO] Producto fisico anadido al catalogo.\n";
            } catch (const exception& e) {
                cout << "[ERROR]: " << e.what() << "\n";
            } 
            break;
        }
        case 3: {
            string cod; 
            int cantidad;
            cout << "Ingrese el codigo del producto: "; cin >> cod;
            cout << "Cantidad a sumar al stock: "; cin >> cantidad;
            bool encontrado = false;
            try {
                // Busca el producto por su código y le sube el stock si lo encuentra
                for (auto &punteroBusqueda : catalogo) {
                    if (punteroBusqueda->getCodigo() == cod) {
                        punteroBusqueda->aumentarStock(cantidad);
                        cout << "[EXITO] Stock actualizado. Nuevo stock: " << punteroBusqueda->getStock() << "\n";
                        encontrado = true;
                        break;
                    }
                }
                if (!encontrado) cout << "Producto no encontrado.\n";
            } catch (const exception& e) {
                cout << "[ERROR]: " << e.what() << "\n";
            } 
            break;
        }
    }
}

// Submenú para administrar categorías y asociarles productos
void MenuController::gestionarCategorias() {
    int op = 0;
    cout << "\n--- GESTION DE CATEGORIAS ---\n";
    cout << "1. Ver categorias y sus productos\n";
    cout << "2. Crear nueva categoria\n";
    cout << "3. Asociar producto a categoria\n";
    cout << "Seleccione una opcion: ";
    op = leerEnteroSeguro(1, 3);
    
    switch (op) {
        case 1: {
            if (categorias.empty()) {
                cout << "No hay categorias registradas.\n";
                return;
            }
            for (auto c : categorias) {
                cout << "\nCategoria: " << c->getNombre() << " (ID: " << c->getIdCategoria() << ")\n";
                cout << "Productos asociados:\n";
                const auto& prods = c->getProductos();
                if (prods.empty()) {
                    cout << "  (Sin productos en esta categoria)\n";
                } else {
                    for (auto p : prods) {
                        cout << "  - " << p->getNombre() << " ($" << p->getPrecio() << ")\n";
                    }
                }
            }
            break;
        }
        case 2: {
            string id, nombre;
            cout << "ID de la categoria: "; cin >> id;
            cout << "Nombre de la categoria: "; cin >> ws; getline(cin, nombre);
            categorias.push_back(new Categoria(id, nombre));
            cout << "[EXITO] Categoria creada.\n";
            break;
        }
        case 3: {
            if (categorias.empty() || catalogo.empty()) {
                cout << "Se necesitan categorias y productos existentes.\n";
                break;
            }
            string idCat, codProd;
            cout << "Ingrese ID de la categoria: "; cin >> idCat;
            cout << "Ingrese Codigo del producto: "; cin >> codProd;

            Categoria* catEncontrada = nullptr;
            Producto* prodEncontrado = nullptr;

            // Busca ambos elementos en sus respectivos vectores
            for (auto c : categorias) if (c->getIdCategoria() == idCat) catEncontrada = c;
            for (auto p : catalogo) if (p->getCodigo() == codProd) prodEncontrado = p;

            if (catEncontrada && prodEncontrado) {
                try {
                    catEncontrada->agregarProducto(prodEncontrado);
                    cout << "[EXITO] Producto asociado a la categoria correctamente.\n";
                } catch (const exception& e) {
                    cout << "[ERROR]: " << e.what() << "\n";
                }
            } else {
                cout << "[ERROR] Categoria o producto no encontrado.\n";
            }
        }
    }
}

// Submenú para ver perfiles, recargar saldo, registrar usuarios o cambiar de sesión
void MenuController::gestionarClientes() {
    cout << "\n--- GESTION DE CLIENTES Y SESION ---\n";
    if (clienteSesionActual != nullptr) {
        cout << ">>> Cliente Actual en Sesion: " << clienteSesionActual->getNombre() << " (" << clienteSesionActual->getCorreo() << ")\n";
    }
    cout << "1. Ver perfil y saldo del cliente actual\n";
    cout << "2. Recargar saldo al cliente actual\n";
    cout << "3. Registrar un nuevo cliente\n";
    cout << "4. Cambiar de cliente (Iniciar sesion con otro)\n";
    cout << "5. Eliminar un cliente del sistema\n";
    cout << "Seleccione una opcion: ";

    int op = leerEnteroSeguro(1, 5);
    switch (op) {
        case 1: {
            if (!clienteSesionActual) { cout << "No hay ningun cliente en sesion.\n"; break; }
            cout << "\nNombre: " << clienteSesionActual->getNombre() << "\n";
            cout << "Correo: " << clienteSesionActual->getCorreo() << "\n";
            cout << "Direccion: " << clienteSesionActual->getDireccionEnvio() << "\n";
            cout << "Saldo disponible: $" << clienteSesionActual->getSaldoDisponible() << "\n";
            break;
        }
        case 2: {
            if (!clienteSesionActual) { cout << "No hay ningun cliente en sesion.\n"; break; }
            float monto;
            cout << "Monto a recargar: $";
            monto = leerFlotantePositivo();
            try {
                clienteSesionActual->recargarSaldo(monto);
                cout << "[EXITO] Recarga aplicada. Nuevo saldo: $" << clienteSesionActual->getSaldoDisponible() << "\n";
            } catch (const exception& e) {
                cout << "[ERROR]: " << e.what() << "\n";
            }
            break;
        }
        case 3: registrarNuevoCliente(); break;
        case 4: cambiarClienteSesion(); break;
        case 5: eliminarCliente(); break;
    }
}

// Proceso de compra: arma el carrito, calcula totales y procesa el pago con tarjeta o transferencia
void MenuController::realizarPedido() {
    if (catalogo.empty()) {
        cout << "No hay productos disponibles para comprar.\n";
        return;
    }
    try {
        string tracking, fecha;
        cout << "Ingrese codigo de seguimiento unico (Ej: PED-100): "; cin >> tracking;
        cout << "Ingrese fecha (Ej: 10/11/2026): "; cin >> fecha;

        // Crea el pedido en memoria dinámica
        Pedido* nuevoPedido = new Pedido(tracking, fecha);

        char continuar = 's';
        while (continuar == 's' || continuar == 'S') {
            cout << "\nCatalogo disponible:\n";
            for (size_t i = 0; i < catalogo.size(); ++i) {
                cout << i + 1 << ". " << catalogo[i]->getNombre() << " (Stock: " << catalogo[i]->getStock() << ") - $" << catalogo[i]->getPrecio() << "\n";
            }

            int index, cantidad;
            cout << "Seleccione el numero del producto a agregar: "; cin >> index;
            if (index < 1 || index > (int)catalogo.size()) {
                cout << "Indice invalido.\n";
                continue;
            }
            cout << "Ingrese cantidad: "; cantidad = leerEnteroSeguro(1, 1000);

            // Añade el producto al carrito validando el stock de una vez
            nuevoPedido->agregarDetalle(catalogo[index - 1], cantidad);
            cout << "[OK] Producto agregado al carrito. Subtotal acumulado: $" << nuevoPedido->getMontoTotal() << "\n";

            cout << "Desea agregar otro producto? (s/n): "; cin >> continuar;
        }

        cout << "\nTotal a pagar (incluye envios): $" << nuevoPedido->getMontoTotal() << "\n";
        cout << "Seleccione metodo de pago:\n1. Tarjeta de Credito\n2. Transferencia Bancaria\nOpcion: ";
        int tipoPago = leerEnteroSeguro(1, 2);

        bool pagoExitoso = false;
        ServicioNotificacion notificador;

        if (tipoPago == 1) {
            string num, tit, venc, cvv;
            cout << "Numero de tarjeta (16 digitos): "; cin >> num;
            cout << "Titular: "; cin >> ws; getline(cin, tit);
            cout << "Vencimiento (MM/AA): "; cin >> venc;
            cout << "CVV (3 digitos): "; cin >> cvv;

            TarjetaCredito tarjeta(num, tit, venc, cvv);
            pagoExitoso = nuevoPedido->procesarOrden(tarjeta, notificador, clienteSesionActual->getCorreo());
        } else {
            string banco, cuenta;
            cout << "Banco: "; cin >> ws; getline(cin, banco);
            cout << "Numero de Cuenta: "; cin >> cuenta;

            TransferenciaBancaria transferencia(banco, cuenta);
            pagoExitoso = nuevoPedido->procesarOrden(transferencia, notificador, clienteSesionActual->getCorreo());
        }

        if (pagoExitoso) {
            cout << "[EXITO] Orden procesada correctamente. Estado del pedido: " << nuevoPedido->getEstado() << "\n";
        }

        delete nuevoPedido; // Limpieza del pedido temporal creado con new

    } catch (const exception& e) {
        cout << "[ERROR EN PROCESO DE PEDIDO]: " << e.what() << "\n";
    }
}

// Pide los datos y añade un nuevo cliente a la lista general
void MenuController::registrarNuevoCliente() {
    cout << "\n--- REGISTRAR NUEVO CLIENTE ---\n";
    try {
        string nombre, correo, direccion;
        float saldoInicial;

        cout << "Nombre completo: "; cin >> ws; getline(cin, nombre);
        cout << "Correo electronico: "; cin >> correo;
        cout << "Direccion de envio: "; cin >> ws; getline(cin, direccion);
        cout << "Saldo inicial de apertura: $"; saldoInicial = leerFlotantePositivo();

        listaClientes.push_back(Cliente(nombre, correo, direccion, saldoInicial));
        
        // Auto-selecciona al recién creado como el usuario activo
        clienteSesionActual = &listaClientes.back();

        cout << "[EXITO] Cliente registrado y seleccionado como usuario actual.\n";
    } catch (const exception& e) {
        cout << "[ERROR AL REGISTRAR]: " << e.what() << "\n";
    }
}

// Muestra la lista de usuarios y permite cambiar el puntero de sesión a otro cliente
void MenuController::cambiarClienteSesion() {
    cout << "\n--- SELECCIONAR CLIENTE (INICIAR SESION) ---\n";
    if (listaClientes.empty()) {
        cout << "No hay clientes registrados en el sistema.\n";
        return;
    }

    cout << "Clientes disponibles:\n";
    for (size_t i = 0; i < listaClientes.size(); ++i) {
        cout << i + 1 << ". " << listaClientes[i].getNombre() << " (" << listaClientes[i].getCorreo() << ")\n";
    }

    cout << "Seleccione el numero del cliente: ";
    int seleccion = leerEnteroSeguro(1, (int)listaClientes.size());

    clienteSesionActual = &listaClientes[seleccion - 1];
    cout << "[EXITO] Sesion cambiada a: " << clienteSesionActual->getNombre() << "\n";
}

// Elimina un cliente de la lista validando que no sea el que está activo en la sesión
void MenuController::eliminarCliente() {
    cout << "\n--- ELIMINAR CLIENTE ---\n";
    if (listaClientes.empty()) {
        cout << "No hay clientes registrados.\n";
        return;
    }

    cout << "Clientes registrados:\n";
    for (size_t i = 0; i < listaClientes.size(); ++i) {
        cout << i + 1 << ". " << listaClientes[i].getNombre() << " (" << listaClientes[i].getCorreo() << ")\n";
    }

    cout << "Seleccione el numero del cliente a eliminar: ";
    int index = leerEnteroSeguro(1, (size_t)listaClientes.size());

    Cliente* clienteAEliminar = &listaClientes[index - 1];
    if (clienteAEliminar == clienteSesionActual) {
        cout << "[ADVERTENCIA] No puedes eliminar al cliente que esta activo en la sesion actual.\n";
        return;
    }

    // Remueve el elemento del vector de forma segura
    listaClientes.erase(listaClientes.begin() + (index - 1));
    cout << "[EXITO] Cliente eliminado correctamente del sistema.\n";
}