#include "mainwindow.h"
#include "addcarddialog.h"
#include "addvehicledialog.h"
#include "lib/SAPEICore/Client.h"
#include "lib/SAPEICore/DataBase.h"
#include "serialhandler.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>    
#include <QLineEdit>        
#include <cstdint>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent), ui(new Ui::MainWindow), serialHandler(new SerialHandler(this)), isAddingCardMode(false),isChargingMode(false) {
    ui->setupUi(this);
   
    try {
        db = new DataBase("database.db"); // Cambia "database.db" por la ruta que desees
        ui->textBrowser->append("Base de datos inicializada correctamente.");
    } catch (const std::runtime_error& e) {
        ui->textBrowser->append("Error al inicializar la base de datos: " + QString::fromStdString(e.what()));
    }

    connect(ui->selectSerialPortButton, &QPushButton::clicked, this, &MainWindow::onSelectSerialPortClicked);
    connect(serialHandler, &SerialHandler::idReceived, this, &MainWindow::onIdReceived);
    connect(ui->addVehicleButton, &QPushButton::clicked, this, &MainWindow::onAddVehicleButtonClicked);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSelectSerialPortClicked() {
    serialHandler->selectSerialPort();
}

void MainWindow::on_addCardButton_clicked() {
    isAddingCardMode = true;
    ui->textBrowser->append("Modo de agregar tarjeta activado. Escanee una tarjeta...");

    if (serialHandler->isConnected()) {
        qDebug() << "Puerto serie conectado correctamente";
    } else {
        qDebug() << "No se pudo conectar al puerto serie";
    }
}

void MainWindow::onAddVehicleButtonClicked()
{
    addVehicleToClient();  // Llama a la función que muestra el diálogo de vehículo
}

bool MainWindow::validateId(const QString &id) {
    if (id.length() != 8) {
        qDebug() << "ID no válido";  
        return false;  
    }
    return true;  
}

void MainWindow::onIdReceived(const QString &id) {
  qDebug() << "ID recibido";  
  currentId = id;  

  if (validateId(currentId)) {
    ui->textBrowser->append("ID recibido: " + currentId);

    if (isAddingCardMode) {
      addCard(currentId);
      isAddingCardMode = false;
      serialHandler->sendToArduino("yellow p off");
    }
  } else {
    ui->textBrowser->append("ID no válido: " + id);
    isAddingCardMode = false;
  }
} 

void MainWindow::addCard(const QString &id){
  AddCardDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {           
    QString name = dialog.getName();
    QString lastName = dialog.getLastName();
    QString dniString = dialog.getDNI();
    QString email = dialog.getEmail();
    QString phone = dialog.getPhone();
    QString address = dialog.getAddress();
    int age = dialog.getAge();
    QString license = dialog.getLicense();
    QString type = dialog.getType();
    QString color = dialog.getColor();
    QString brand = dialog.getBrand();
    QString model = dialog.getModel();

    bool ok;
    unsigned long long idInt = id.toULongLong(&ok, 16); 
    if (!ok) {
        qDebug() << "Error al convertir el ID de hexadecimal a entero";
    }

    unsigned int dni = dniString.toUInt(&ok);
    if (!ok) {
        qDebug() << "Error al convertir el DNI a entero";
    }

    QString fullName = name + " " + lastName;

    Client newClient(idInt, fullName.toStdString(), age, dni, address.toStdString(),
                     email.toStdString(), phone.toStdString(),
                     license.toStdString(), type.toStdString(),
                     color.toStdString(), brand.toStdString(),
                     model.toStdString());

    newClient.setBalance(0);

    try {
        db->addClient(newClient);
        ui->textBrowser->append("Cliente agregado: " + name + " " + lastName + " con ID: " + id);
    } catch (const std::runtime_error& e) {
        ui->textBrowser->append("Error al agregar el cliente: " + QString::fromStdString(e.what()));
    }
  }
}

void MainWindow::addVehicleToClient(){
    bool ok;
    QString inputId = QInputDialog::getText(this, "Agregar Vehículo",
                                            "Ingrese el ID del cliente (en hexadecimal):",
                                            QLineEdit::Normal, "", &ok);
    if (ok && !inputId.isEmpty()) {
        // Convertir el ID hexadecimal a entero
        bool conversionOk;
        unsigned long long clientId = inputId.toULongLong(&conversionOk, 16); // Base 16 para hexadecimal

        if (!conversionOk) {
            QMessageBox::warning(this, "Error de conversión", "No se pudo convertir el ID de hexadecimal a entero.");
            return;
        }

        // Recupera el cliente utilizando clientId
        Client client;
        try {
            client = db->getClientById(clientId); // Usa getClientById para recuperar el cliente
        } catch (const std::runtime_error &e) {
            QMessageBox::warning(this, "Cliente no encontrado", "El cliente con el ID ingresado no existe en la base de datos.");
            return;
        }
        std::cout << client << std::endl;

    // Abrir un diálogo para ingresar los datos del vehículo
        AddVehicleDialog vehicleDialog(this);
        if (vehicleDialog.exec() == QDialog::Accepted) {
            QString license = vehicleDialog.getLicense();
            QString type = vehicleDialog.getType();
            QString color = vehicleDialog.getColor();
            QString brand = vehicleDialog.getBrand();
            QString model = vehicleDialog.getModel();

            // Crear el objeto Vehicle
            Vehicle newVehicle(license.toStdString(), type.toStdString(),
                               color.toStdString(), brand.toStdString(),
                               model.toStdString());

            // Agregar el vehículo al cliente en la base de datos
            try {
                db->addVehicle(clientId, newVehicle); // Usa addVehicle de DataBase
                ui->textBrowser->append("Vehículo agregado al cliente con ID: " + inputId);
            } catch (const std::runtime_error& e) {
                ui->textBrowser->append("Error al agregar el vehículo: " + QString::fromStdString(e.what()));
            }
        } else {
            ui->textBrowser->append("Operación de agregar vehículo cancelada.");
        }
  } 
}
