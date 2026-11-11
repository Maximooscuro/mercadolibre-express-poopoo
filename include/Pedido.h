//
// Created by maxim on 30/10/2026.
//

#pragma once
#include <vector>
#include <string>
#include "DetallePedido.h"
#include "MetodoPago.h"
#include "ServicioNotificacion.h"

class Pedido {
private:
    std::string numeroSeguimiento;
    std::string fecha;
    std::string estado; // "Pendiente", "Pagado", "Cancelado"
    float montoTotal;

    // Pedido es dueño exclusivo de la memoria de sus detalles
    std::vector<DetallePedido*> detalles;

public:
    Pedido(std::string numSeg, std::string fch);
    ~Pedido(); // El destructor DEBE liberar cada DetallePedido* con delete

    void agregarDetalle(const Producto* prod, int cant);
    void calcularTotal();
    bool procesarOrden(MetodoPago& metodo, ServicioNotificacion& servicio, const std::string& correoCliente);

    std::string getNumeroSeguimiento() const;
    std::string getEstado() const;
    float getMontoTotal() const;
    const std::vector<DetallePedido*>& getDetalles() const;
    std::string getFecha() const;
    void setEstado(const std::string& nuevoEstado);
};
