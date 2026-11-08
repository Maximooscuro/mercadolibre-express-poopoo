#include "PantallaLogin.h"
#include "Mensajes.h"
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>

PantallaLogin::PantallaLogin(std::function<void(const QString&)> ingresar, QWidget* parent)
    : QWidget(parent), alIngresar(ingresar) {
    auto* externo = new QVBoxLayout(this);

    auto* tarjeta = new QFrame();
    tarjeta->setObjectName("tarjeta");
    tarjeta->setFixedWidth(380);

    auto* l = new QVBoxLayout(tarjeta);
    l->setContentsMargins(32, 32, 32, 32);
    l->setSpacing(12);

    auto* titulo = new QLabel("MercadoLibre Express");
    titulo->setObjectName("titulo");
    titulo->setAlignment(Qt::AlignCenter);

    auto* sub = new QLabel("Bienvenido. Ingrese usuario y contraseña");
    sub->setObjectName("subtitulo");
    sub->setAlignment(Qt::AlignCenter);
    sub->setWordWrap(true);

    usuario = new QLineEdit();
    usuario->setPlaceholderText("Usuario");
    clave = new QLineEdit();
    clave->setPlaceholderText("Contraseña");
    clave->setEchoMode(QLineEdit::Password);

    auto* boton = new QPushButton("Ingresar");
    boton->setCursor(Qt::PointingHandCursor);

    // Ayuda solo para la demostracion; se quita cuando haya usuarios reales
    auto* ayuda = new QLabel("Demo: admin / admin123   o   cliente / 1234");
    ayuda->setObjectName("subtitulo");
    ayuda->setAlignment(Qt::AlignCenter);

    l->addWidget(titulo);
    l->addWidget(sub);
    l->addSpacing(8);
    l->addWidget(usuario);
    l->addWidget(clave);
    l->addWidget(boton);
    l->addWidget(ayuda);

    externo->addStretch();
    externo->addWidget(tarjeta, 0, Qt::AlignHCenter);
    externo->addStretch();

    connect(boton, &QPushButton::clicked, [this]() { intentarIngreso(); });
    connect(usuario, &QLineEdit::returnPressed, [this]() { intentarIngreso(); });
    connect(clave, &QLineEdit::returnPressed, [this]() { intentarIngreso(); });
}

void PantallaLogin::limpiar() {
    usuario->clear();
    clave->clear();
    usuario->setFocus();
}

void PantallaLogin::intentarIngreso() {
    // Usuarios de ejemplo. Luego se conectara con Cliente / Persistencia.
    struct UsuarioDemo { QString usuario; QString clave; QString nombre; };
    const UsuarioDemo usuarios[] = {
        {"admin", "admin123", "Administrador"},
        {"cliente", "1234", "Cliente"}
    };

    QString u = usuario->text().trimmed();
    QString c = clave->text();

    if (u.isEmpty() || c.isEmpty()) {
        mostrarError(this, "Ingrese usuario y contraseña.");
        return;
    }
    for (const auto& x : usuarios) {
        if (x.usuario == u && x.clave == c) {
            QString nombre = x.nombre;
            limpiar();
            alIngresar(nombre);
            return;
        }
    }
    mostrarError(this, "Usuario o contraseña incorrectos.");
}
