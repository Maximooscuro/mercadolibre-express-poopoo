#pragma once
// Cuentas de usuario de la GUI: crear cuenta, validar usuario y contrasena.
// Se guardan en un archivo en la carpeta de datos de la aplicacion del usuario
// (no en data/, que es de Persistencia). La contrasena NO se guarda en claro:
// se guarda su hash SHA-256.
#include <QString>
#include <QMap>

class GestorUsuarios {
public:
    GestorUsuarios();

    // Crea una cuenta nueva. Si falla, devuelve false y explica en 'error'.
    bool registrar(const QString& usuario, const QString& clave, QString& error);

    // true si el usuario existe y la contrasena es correcta.
    bool validar(const QString& usuario, const QString& clave) const;

private:
    QString rutaArchivo() const;
    void cargar();
    bool guardar() const;
    static QString hash(const QString& usuario, const QString& clave);

    QMap<QString, QString> usuarios;   // usuario -> hash de la contrasena
};
