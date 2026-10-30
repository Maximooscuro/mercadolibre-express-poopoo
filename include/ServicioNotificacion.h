//
// Created by maxim on 30/10/2026.
//

#pragma once
#include <string>
#include <iostream>

class ServicioNotificacion {
public:
    ServicioNotificacion() = default;
    ~ServicioNotificacion() = default;

    void enviarConfirmacion(const std::string& destinatario, const std::string& tracking, float total);
};