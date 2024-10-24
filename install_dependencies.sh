#!/bin/bash

# Detectar distribución y ejecutar el instalador correspondiente

# Ubuntu/Debian
if [ -x "$(command -v apt)" ]; then
    echo "Detectado sistema Ubuntu/Debian, instalando dependencias..."
    sudo apt update
    sudo apt install -y cmake make libsqlite3-dev qtbase5-dev qtchooser libqt5serialport5-dev

# Arch Linux
elif [ -x "$(command -v pacman)" ]; then
    echo "Detectado sistema Arch Linux, instalando dependencias..."
    sudo pacman -S --noconfirm cmake make sqlite qt5-base qt5-serialport

# Fedora
elif [ -x "$(command -v dnf)" ]; then
    echo "Detectado sistema Fedora, instalando dependencias..."
    sudo dnf install -y cmake make sqlite sqlite-devel qt5-qtbase qt5-qtbase-devel qt5-qserialport
else
    echo "Distribución no soportada para la instalación automática."
    exit 1
fi

echo "Instalación de dependencias completada."
