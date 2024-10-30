cmake_minimum_required(VERSION 3.5)

# Establece el nombre del proyecto
project(SAPEI)

# Encuentra la biblioteca Qt
find_package(Qt5 REQUIRED COMPONENTS Widgets SerialPort)
find_package(SQLite3 REQUIRED)

# Agrega los archivos fuente y encabezados
set(SOURCES
    main.cpp
    mainwindow.cpp
    serialhandler.cpp
    addcarddialog.cpp
    addvehicledialog.cpp
    clientlistdialog.cpp
    vehiclelistdialog.cpp 
    lib/SAPEICore/Client.cpp
    lib/SAPEICore/Person.cpp
    lib/SAPEICore/Vehicle.cpp
    lib/SAPEICore/DataBase.cpp
)

set(HEADERS
    mainwindow.h
    serialhandler.h
    addcarddialog.h
    addvehicledialog.h
    clientlistdialog.h
    vehiclelistdialog.h 
    lib/SAPEICore/Client.h
    lib/SAPEICore/Person.h
    lib/SAPEICore/Vehicle.h
    lib/SAPEICore/DataBase.h
)

# Añade la ruta del archivo .qrc
set(RESOURCE_FILES
    resources.qrc
)

# Genera el archivo de encabezado de recursos
qt5_add_resources(RESOURCE_GENERATED_CPP ${RESOURCE_FILES})

# Define el ejecutable
add_executable(SAPEI ${SOURCES} ${HEADERS} ${RESOURCE_GENERATED_CPP} mainwindow.ui)

# Establece la configuración de la interfaz de usuario
set_target_properties(SAPEI PROPERTIES
    AUTOMOC TRUE
    AUTOUIC TRUE
    SKIP_AUTOMOC FALSE
)

# Añade las bibliotecas de Qt al ejecutable
target_link_libraries(SAPEI PRIVATE 
    Qt5::Widgets
    Qt5::SerialPort
)

# Establecer las rutas para la biblioteca SQLite3
target_include_directories(SAPEI PRIVATE ${SQLite3_INCLUDE_DIRS})
target_link_libraries(SAPEI PRIVATE ${SQLite3_LIBRARIES})

# Agregar flags para errores y advertencias
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(SAPEI PRIVATE 
        -Wall              # Activa todas las advertencias
        -Wextra           # Activa advertencias adicionales
        -Wpedantic        # Activa advertencias de estándares
        -Werror           # Tratar advertencias como errores
    )
else()
    target_compile_options(SAPEI PRIVATE 
        -Wall              # Activa todas las advertencias
        -Wextra           # Activa advertencias adicionales
        -Wconversion
        -pedantic-errors
    )
endif()

# Para habilitar soporte para C++11 o superior (ajusta según sea necesario)
set_target_properties(SAPEI PROPERTIES CXX_STANDARD 11 CXX_STANDARD_REQUIRED ON)

# Configuración de dependencias
if(UNIX)
 # Crear objetivo 'dependencies' solo para entornos Unix (Linux/macOS)
    add_custom_target(dependencies
        COMMAND bash ${CMAKE_SOURCE_DIR}/install_dependencies.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Instalando dependencias para Linux/macOS..."
    )
endif()
