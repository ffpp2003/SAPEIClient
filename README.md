# SAPEI Client

Este proyecto es**interafaz de usuario del proyecto SAPEI**.

## Requisitos

Asegúrate de tener instalados los siguientes paquetes antes de compilar:

- CMake (versión mínima: 3.5)
- Make
- Qt5
- Qt5 3D
- SQLite3


para instalar las dependencias:
```bash

#crear un directorio build (si no exsite)
mkdir build

# Moverse al directorio build
cd build

# Configurar el proyecto con CMake
cmake ..

#instalar las dependencias:
make dependencies
```
O para instalarlas manualmente:

```bash

sudo apt update

sudo apt install -y cmake make libsqlite3-dev qtbase5-dev qtchooser libqt5serialport5-dev qt3d5-dev-tools 


```



## Compilación

Sigue estos pasos para compilar el proyecto:

```bash
# Crear un directorio build
mkdir build

# Moverse al directorio build
cd build

# Configurar el proyecto con CMake
cmake ..

# Compilar el proyecto
make

