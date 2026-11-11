cmake_minimum_required(VERSION 4.2)
project(mercadolibrepoo)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(mercadolibrepoo
        main.cpp
        src/core/Cliente.cpp
        src/core/Categoria.cpp
        src/core/DetallePedido.cpp
        src/core/GestorArchivos.cpp
        src/core/MenuController.cpp
        src/core/Pedido.cpp
        src/core/Producto.cpp
        src/core/ProductoDigital.cpp
        src/core/ProductoFisico.cpp
        src/core/ServicioNotificacion.cpp
        src/core/TarjetaCredito.cpp
        src/core/TransferenciaBancaria.cpp
)
