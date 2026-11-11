#pragma once
// Hoja de estilos (QSS) de toda la aplicacion. Colores tipo MercadoLibre
inline const char* estilosApp() {
    return R"QSS(
QWidget { font-family: "Segoe UI"; font-size: 14px; color: #333333; }
QWidget#ventana { background: #EBEBEB; }
QStackedWidget { background: #EBEBEB; }

QFrame#encabezado { background: #FFE600; }
QLabel#logo { font-size: 18px; font-weight: 700; color: #2D3277; }
QLabel#saludo { font-weight: 700; color: #2D3277; padding-right: 8px; }

QFrame#tarjeta { background: white; border: 1px solid #DDDDDD; border-radius: 12px; }
QLabel#titulo { font-size: 24px; font-weight: 700; color: #2D3277; }
QLabel#subtitulo { color: #777777; }

QPushButton {
    background: #2D3277; color: white; border: none;
    border-radius: 6px; padding: 8px 16px; font-weight: 600;
}
QPushButton:hover { background: #3F47A8; }
QPushButton:pressed { background: #232860; }

QPushButton#botonNav { background: transparent; color: #2D3277; font-weight: 700; padding: 8px 14px; }
QPushButton#botonNav:hover { background: rgba(45, 50, 119, 0.12); }
QPushButton#botonNav:checked { background: #2D3277; color: white; }

QPushButton#botonSalir { background: transparent; color: #2D3277; border: 1px solid #2D3277; }
QPushButton#botonSalir:hover { background: rgba(45, 50, 119, 0.12); }

QPushButton#botonLink { background: transparent; color: #2D3277; font-weight: 600; padding: 4px; }
QPushButton#botonLink:hover { text-decoration: underline; background: transparent; }

QLineEdit, QSpinBox, QComboBox {
    background: white; border: 1px solid #BBBBBB; border-radius: 6px; padding: 6px 10px;
}
QLineEdit:focus, QSpinBox:focus, QComboBox:focus { border: 1px solid #2D3277; }

QTableWidget {
    background: white; border: 1px solid #DDDDDD; border-radius: 6px;
    gridline-color: #E6E6E6;
}
QTableWidget::item:selected { background: #2D3277; color: white; }
QHeaderView::section {
    background: #FFE600; color: #2D3277; font-weight: 700;
    border: none; padding: 6px;
}
)QSS";
}