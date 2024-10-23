#include "mainwindow.h"
#include "addcarddialog.h"
#include "lib/SAPEICore/Client.h"
#include "lib/SAPEICore/DataBase.h"
#include "serialhandler.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
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
}



MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSelectSerialPortClicked()
{
    // Llamar al método para seleccionar el puerto serie
    serialHandler->selectSerialPort();
}


void MainWindow::on_addCardButton_clicked() {
    // Cambia a modo de agregar tarjeta
    isAddingCardMode = true;
    ui->textBrowser->append("Modo de agregar tarjeta activado. Escanee una tarjeta...");

    // Asegúrate de que el puerto serie esté abierto después de la selección
    if (serialHandler->isConnected()) {
        qDebug() << "Puerto serie conectado correctamente";
    } else {
        qDebug() << "No se pudo conectar al puerto serie";
    }
}


bool MainWindow::validateId(const QString &id) {
    if (id.length() != 8) {
        qDebug() << "ID no válido";  // Mensaje de depuración
        return false;  // ID no válido
    }
    return true;  // ID válido
}

void MainWindow::onIdReceived(const QString &id) {
  qDebug() << "ID recibido";  // Mensaje de depuración
  currentId = id;  // Almacena el ID recibido

  // Llama al método de validación
  if (validateId(currentId)) {
    ui->textBrowser->append("ID recibido: " + currentId);

    // Si estás en modo de agregar tarjeta, muestra el diálogo
    if (isAddingCardMode) {
      addCard(currentId);
      isAddingCardMode = false;
      serialHandler->sendToArduino("yellow p off");
    }
  }else {
    ui->textBrowser->append("ID no válido: " + id);
    isAddingCardMode = false;
  }
} 


void MainWindow::addCard(const QString &id){
  AddCardDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {           // Obtener los datos ingresados desde el diálogo
    QString name = dialog.getName();
    QString lastName = dialog.getLastName();
    QString dni = dialog.getDNI();
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
    unsigned long long idInt = id.toULongLong(&ok, 16); // Convertir el QString 'id' desde hexadecimal a entero
    if (!ok) {
    // Manejar error si la conversión falla
    qDebug() << "Error al convertir el ID de hexadecimal a entero";
    }

    QString fullName = name + " " + lastName;

    // Crear el objeto Client
    Client newClient(idInt, fullName.toStdString(), age, address.toStdString(),
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
