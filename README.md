# SAPEI Client

Este proyecto es **interafaz de usuario del proyecto SAPEI**.

## Requisitos

Asegúrate de tener instalados los siguientes paquetes antes de compilar:

- CMake (versión mínima: 3.5)
- Make
- Qt5
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
sudo apt install -y cmake make libsqlite3-dev qtbase5-dev qtchooser libqt5serialport5-dev
```

## Submodulos

El repositorio utiliza submodulos para importar el SAPEICore y poder utilizarlo.

Apenas clonado el repositorio, el submodulo necesita ser inicializado para poder tener las bibliotecas disponibles. Para eso, en la carpeta raiz del repositorio, ejecutar:
```bash
git submodule init
git submodule update
```

Con eso, el repositorio SAPEICore sera clonado bajo lib/SAPEICore. Dentro de esa carpeta, todos los comandos `git`, van a afectar al repositorio SAPEICore. Esto permite cambiar ramas, hacer commits.

Cuando haya un nuevo commit en el submodulo, para actualizarlo, ejecuter:
```bash
git submodule update
```

ó, establecer un remoto y una rama en el submodulo y hacer `git pull`.


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
```
