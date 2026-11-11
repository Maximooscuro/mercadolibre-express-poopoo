#include "PantallaLogin.h"
#include "Mensajes.h"
#include <QVBoxLayout>
#include <QFrame>

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

    subtitulo = new QLabel();
    subtitulo->setObjectName("subtitulo");
    subtitulo->setAlignment(Qt::AlignCenter);
    subtitulo->setWordWrap(true);

    usuario = new QLineEdit();
    usuario->setPlaceholderText("Usuario");
    clave = new QLineEdit();
    clave->setPlaceholderText("Contraseña");
    clave->setEchoMode(QLineEdit::Password);
    confirmar = new QLineEdit();
    confirmar->setPlaceholderText("Confirmar contraseña");
    confirmar->setEchoMode(QLineEdit::Password);

    botonPrincipal = new QPushButton();
    botonPrincipal->setCursor(Qt::PointingHandCursor);

    botonModo = new QPushButton();
    botonModo->setObjectName("botonLink");
    botonModo->setCursor(Qt::PointingHandCursor);
    botonModo->setFlat(true);

    l->addWidget(titulo);
    l->addWidget(subtitulo);
    l->addSpacing(8);
    l->addWidget(usuario);
    l->addWidget(clave);
    l->addWidget(confirmar);
    l->addWidget(botonPrincipal);
    l->addWidget(botonModo);

    externo->addStretch();
    externo->addWidget(tarjeta, 0, Qt::AlignHCenter);
    externo->addStretch();

    connect(botonPrincipal, &QPushButton::clicked, [this]() { intentarIngreso(); });
    connect(botonModo, &QPushButton::clicked, [this]() { alternarModo(); });
    connect(usuario, &QLineEdit::returnPressed, [this]() { clave->setFocus(); });
    connect(clave, &QLineEdit::returnPressed, [this]() {
        if (modoRegistro) confirmar->setFocus(); else intentarIngreso();
    });
    connect(confirmar, &QLineEdit::returnPressed, [this]() { intentarIngreso(); });

    actualizarModo();
}

void PantallaLogin::actualizarModo() {
    confirmar->setVisible(modoRegistro);
    if (modoRegistro) {
        subtitulo->setText("Crear cuenta: elija su usuario y contraseña");
        botonPrincipal->setText("Crear cuenta");
        botonModo->setText("¿Ya tiene cuenta? Ingresar");
    } else {
        subtitulo->setText("Bienvenido. Ingrese usuario y contraseña");
        botonPrincipal->setText("Ingresar");
        botonModo->setText("¿No tiene cuenta? Crear una");
    }
}

void PantallaLogin::alternarModo() {
    modoRegistro = !modoRegistro;
    clave->clear();
    confirmar->clear();
    actualizarModo();
}

void PantallaLogin::limpiar() {
    modoRegistro = false;
    usuario->clear();
    clave->clear();
    confirmar->clear();
    actualizarModo();
    usuario->setFocus();
}

void PantallaLogin::intentarIngreso() {
    QString u = usuario->text().trimmed();
    QString c = clave->text();

    if (u.isEmpty() || c.isEmpty()) {
        mostrarError(this, "Ingrese usuario y contraseña.");
        return;
    }

    if (modoRegistro) {
        if (c != confirmar->text()) {
            mostrarError(this, "Las contraseñas no coinciden.");
            return;
        }
        QString error;
        if (!gestor.registrar(u, c, error)) {
            mostrarError(this, error);
            return;
        }
    } else {
        if (!gestor.validar(u, c)) {
            mostrarError(this, "Usuario o contraseña incorrectos.");
            return;
        }
    }

    limpiar();
    alIngresar(u);
}