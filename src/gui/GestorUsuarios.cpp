#include "GestorUsuarios.h"
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <QStringConverter>

GestorUsuarios::GestorUsuarios() {
    cargar();
}

QString GestorUsuarios::rutaArchivo() const {
    QString carpeta = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(carpeta);
    return carpeta + "/usuarios.txt";
}

QString GestorUsuarios::hash(const QString& usuario, const QString& clave) {
    QByteArray h = QCryptographicHash::hash((usuario + ":" + clave).toUtf8(),
                                            QCryptographicHash::Sha256);
    return QString::fromLatin1(h.toHex());
}

void GestorUsuarios::cargar() {
    usuarios.clear();
    QFile f(rutaArchivo());
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) return;   // primera vez: no existe
    QTextStream in(&f);
    in.setEncoding(QStringConverter::Utf8);
    while (!in.atEnd()) {
        QString linea = in.readLine().trimmed();
        int p = linea.lastIndexOf('|');
        if (p <= 0) continue;
        usuarios[linea.left(p)] = linea.mid(p + 1);
    }
}

bool GestorUsuarios::guardar() const {
    QFile f(rutaArchivo());
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) return false;
    QTextStream out(&f);
    out.setEncoding(QStringConverter::Utf8);
    for (auto it = usuarios.constBegin(); it != usuarios.constEnd(); ++it)
        out << it.key() << '|' << it.value() << '\n';
    return true;
}

bool GestorUsuarios::registrar(const QString& usuario, const QString& clave, QString& error) {
    error.clear();
    QString u = usuario.trimmed();

    if (u.size() < 3 || u.size() > 20) {
        error = "El usuario debe tener entre 3 y 20 caracteres.";
        return false;
    }
    if (u.contains('|')) {
        error = "El usuario no puede contener el carácter |.";
        return false;
    }
    if (clave.size() < 4) {
        error = "La contraseña debe tener al menos 4 caracteres.";
        return false;
    }
    if (usuarios.contains(u)) {
        error = "Ese usuario ya existe. Elija otro nombre.";
        return false;
    }

    usuarios[u] = hash(u, clave);
    if (!guardar()) {
        usuarios.remove(u);
        error = "No se pudo guardar la cuenta en el disco.";
        return false;
    }
    return true;
}

bool GestorUsuarios::validar(const QString& usuario, const QString& clave) const {
    QString u = usuario.trimmed();
    auto it = usuarios.constFind(u);
    return it != usuarios.constEnd() && it.value() == hash(u, clave);
}
