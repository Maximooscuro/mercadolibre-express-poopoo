cmake_minimum_required(VERSION 4.2)
project(mercadolibrepoo)

set(CMAKE_CXX_STANDARD 20)

add_executable(mercadolibrepoo main.cpp)
file(GLOB CORE_SOURCES CONFIGURE_DEPENDS src/core/*.cpp)

add_executable(prueba_persistencia tests/main_persistencia.cpp ${CORE_SOURCES})
target_include_directories(prueba_persistencia PRIVATE include)