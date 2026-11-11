//
// Created by maxim on 10/11/2026.
//
#pragma once
#include <vector>
#include <string>
#include "Cliente.h"
#include "Producto.h"
#include "Categoria.h"
#include "Pedido.h"
#include "MetodoPago.h"
#include "TarjetaCredito.h"
#include "TransferenciaBancaria.h"
#include "ServicioNotificacion.h"
#include "Excepciones.h"
#include "GestorArchivos.h"

class MenuController {
    private:
    //vectores para las listas
    std::vector<Cliente> listaClientes;
    Cliente* clienteSesionActual = nullptr;
    std::vector<Producto*> catalogo;
    std::vector<Categoria*> categorias;
    //persistencia
    GestorArchivos gestor;
    std::vector<Pedido*> historialPedidos;
    std::string dirDatos = "data/";
    bool cargaFallida = false;
    void cargarDatos();
    void guardarDatos();
    //funciones de la categoria
    void mostrarMenuPrincipal() const;
    void gestionarProductos();
    void gestionarCategorias();
    void gestionarClientes();
    void realizarPedido();
    void registrarNuevoCliente();
    void cambiarClienteSesion();
    void eliminarCliente();
    //funciones auxiliares
    int leerEnteroSeguro(int minVal, int maxVal);
    float leerFlotantePositivo();
public:
    MenuController();
    ~MenuController();
//funcion goat :)
    void ejecutar();
};