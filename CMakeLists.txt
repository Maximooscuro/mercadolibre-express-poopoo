cmake_minimum_required(VERSION 4.2)
project(mercadolibrepoo)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(mercadolibrepoo
        main.cpp
        src/core/Cliente.cpp
        src/core/Categoria.cpp
        src/core/DetallePedido.cpp
        src/core/Pedido.cpp
        src/core/Producto.cpp
        src/core/ProductoDigital.cpp
        src/core/ProductoFisico.cpp
        src/core/ServicioNotificacion.cpp
        src/core/TarjetaCredito.cpp
        src/core/TransferenciaBancaria.cpp
        include/MenuController.h
        src/core/MenuController.cpp
)

#QA/UI
find_package(Qt6 COMPONENTS Widgets)

if(Qt6_FOUND)
    add_executable(gui_prueba
            src/gui/prueba_qt.cpp
            src/gui/VentanaPrincipal.cpp
            src/gui/PantallaLogin.cpp
            src/gui/GestorUsuarios.cpp
            src/gui/PantallaCatalogo.cpp
            src/gui/PantallaCompra.cpp
            src/gui/PantallaPago.cpp
            src/gui/PantallaReportes.cpp
            src/gui/CargadorCatalogo.cpp
            src/core/Cliente.cpp
            src/core/Categoria.cpp
            src/core/DetallePedido.cpp
            src/core/Pedido.cpp
            src/core/Producto.cpp
            src/core/ProductoDigital.cpp
            src/core/ProductoFisico.cpp
            src/core/ServicioNotificacion.cpp
            src/core/TarjetaCredito.cpp
            src/core/TransferenciaBancaria.cpp)
    target_include_directories(gui_prueba PRIVATE include src/gui)
    target_link_libraries(gui_prueba PRIVATE Qt6::Widgets)
endif()