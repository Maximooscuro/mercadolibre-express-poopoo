cmake_minimum_required(VERSION 4.2)
project(mercadolibrepoo)

set(CMAKE_CXX_STANDARD 20)

add_executable(mercadolibrepoo main.cpp
        include/Excepciones.h
        include/ProductoFisico.h
        include/Producto.h
        include/ProductoDigital.h
        include/MetodoPago.h
        include/TarjetaCredito.h
        include/TransferenciaBancaria.h
        include/DetallePedido.h
        include/Pedido.h
        include/Cliente.h
        include/Categoria.h
        include/ServicioNotificacion.h
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
