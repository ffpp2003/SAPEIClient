cmake_minimum_required(VERSION 3.5)

# Establece el estándar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Establece el nombre del proyecto
project(SAPEI)

# Encuentra las bibliotecas necesarias de Qt y SQLite
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
    editclientdialog.cpp
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
    editclientdialog.h
    lib/SAPEICore/Client.h
    lib/SAPEICore/Person.h
    lib/SAPEICore/Vehicle.h
    lib/SAPEICore/DataBase.h
)

# Añade el archivo de recursos
set(RESOURCE_FILES resources.qrc)
qt5_add_resources(RESOURCE_GENERATED_CPP ${RESOURCE_FILES})

# Define el ejecutable
add_executable(SAPEI ${SOURCES} ${HEADERS} ${RESOURCE_GENERATED_CPP} mainwindow.ui)

# Establece propiedades de Qt para AUTOUIC y AUTOMOC
set_target_properties(SAPEI PROPERTIES
    AUTOMOC TRUE
    AUTOUIC TRUE
)

# Añade las bibliotecas de Qt y SQLite3 al ejecutable
target_link_libraries(SAPEI PRIVATE Qt5::Widgets Qt5::SerialPort ${SQLite3_LIBRARIES})
target_include_directories(SAPEI PRIVATE ${SQLite3_INCLUDE_DIRS})

# Configura las opciones de compilación para advertencias y errores
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(SAPEI PRIVATE -Wall -Wextra -Wpedantic -Werror)
else()
    target_compile_options(SAPEI PRIVATE -Wall -Wextra -Wconversion -pedantic-errors)
endif()

# Configuración de dependencias solo para entornos Unix (Linux/macOS)
if(UNIX)
    add_custom_target(dependencies
        COMMAND bash ${CMAKE_SOURCE_DIR}/install_dependencies.sh
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Instalando dependencias para Linux/macOS..."
    )
endif()
