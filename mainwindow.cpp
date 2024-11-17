#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent), 
     serialHandler(new SerialHandler(this)),  
     db(new DataBase("database.db")), 
     balanceHandler(new BalanceHandler(db, this)), 
     isAddingCardMode(false),
     isChargingMode(false),
     ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QMovie *movie = new QMovie(":/utnlogo.gif"); // Reemplaza con la ruta de tu GIF
    movie->setScaledSize(QSize(200, 200));
    ui->utnLogo->setMovie(movie);
    movie->start(); // Iniciar la animación
    ui->utnLogo->setFixedSize(200, 200);
    ui->utnLogo->setScaledContents(false); // Permitir que el QLabel ajuste su contenido

    updateAddCardState();

    try {
        ui->textBrowser->append("Base de datos inicializada correctamente.");
    } catch (const std::runtime_error& e) {
        ui->textBrowser->append("Error al inicializar la base de datos: " + QString::fromStdString(e.what()));
    }

    connect(ui->selectSerialPortButton, &QPushButton::clicked, this, &MainWindow::onSelectSerialPortClicked);
    connect(serialHandler, &SerialHandler::idReceived, this, &MainWindow::onIdReceived);
    connect(ui->addVehicleButton, &QPushButton::clicked, this, &MainWindow::onAddVehicleButtonClicked);
    connect(ui->clientListButton, &QPushButton::clicked, this, &MainWindow::onClientListButtonClicked);
    connect(ui->vehicleListButton, &QPushButton::clicked, this, &MainWindow::onVehicleListButtonClicked);
    connect(ui->confirmPriceButton, &QPushButton::clicked, this, &MainWindow::onConfirmPriceChangeClicked);
    connect(ui->chargeBalanceButton, &QPushButton::clicked, this, &MainWindow::openBalanceDialog);
    connect(balanceHandler, &BalanceHandler::balanceUpdated, this, &MainWindow::onBalanceUpdated);
    connect(balanceHandler, &BalanceHandler::balanceUpdateFailed, this, &MainWindow::onBalanceUpdateFailed);
    connect(balanceHandler, &BalanceHandler::windowClosed, this, &MainWindow::onClosedChargeWindow);
    connect(balanceHandler->ui.acceptButton, &QPushButton::clicked, this, &MainWindow::onClosedChargeWindow);
    connect(balanceHandler->ui.cancelButton, &QPushButton::clicked, this, &MainWindow::onClosedChargeWindow);


    QTimer *connectionStatusTimer = new QTimer(this);
    connect(connectionStatusTimer, &QTimer::timeout, this, &MainWindow::updateConnectionStatus);
    connectionStatusTimer->start(1000);
    QString configFileName = "configfile.conf";
    QFile configFile(configFileName);

    if (!QFileInfo::exists(configFileName)) {
        if (configFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&configFile);
            out << 600; // Precio por defecto
            configFile.close();
        }
    }

    // Leer el precio desde el archivo y establecerlo
    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&configFile);
        double fileprice;
        in >> fileprice; // Leer el precio del archivo
        configFile.close();

        // Establecer el precio en la variable price y en el display
        updatePriceDisplay(); // Llamar a tu función de actu                     //
                                   
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSelectSerialPortClicked() {
    serialHandler->selectSerialPort();
}


void MainWindow::updateConnectionStatus() {
    // Verifica el estado de conexión a través de SerialHandler
    QGraphicsScene* scene = new QGraphicsScene(this);
    QGraphicsTextItem* textItem = new QGraphicsTextItem;

    if (serialHandler->isConnected()) {
        textItem->setPlainText("Conectado");
        textItem->setDefaultTextColor(Qt::green);
    } else {
        textItem->setPlainText("Desconectado");
        textItem->setDefaultTextColor(Qt::red);
    }

    QFont font = textItem->font();
    font.setPointSize(24);  // Tamaño de la fuente
    font.setBold(true);     // Texto en negrita
    textItem->setFont(font);

    scene->addItem(textItem);
    ui->connectionStatusView->setScene(scene);
    updateAddCardState();
}

void MainWindow::openBalanceDialog(){
  isChargingMode = true;
  QString msg = QString::number(balanceHandler->openDialog());
  serialHandler->sendToArduino(msg);
}

void MainWindow::updatePriceDisplay()
{
    double price = 0;
    price = balanceHandler->loadPrice();
    ui->priceDisplay->display(price);
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

void MainWindow::onClientListButtonClicked()
{
    ClientListDialog dialog(db, this);
    dialog.exec();
}

void MainWindow::onVehicleListButtonClicked()
{
    VehicleListDialog dialog(db, this);
    dialog.exec();
}

void MainWindow::onConfirmPriceChangeClicked() {
  changePrice();
  updatePriceDisplay();
}

void MainWindow::onBalanceUpdated(const QString &message) {
    ui->textBrowser->append("Éxito: " + message);  // Actualizar la interfaz con el mensaje
}

void MainWindow::onBalanceUpdateFailed(const QString &message) {
    ui->textBrowser->append("Error: " + message);  // Mostrar el error en la interfaz
}

void MainWindow::changePrice(){

    bool ok;
    double newPrice = ui->priceInput->text().toDouble(&ok);  
    if (newPrice >= 0) {
        ui->textBrowser->append("Precio de estacionamiento actualizado a: $" + QString::number(newPrice, 'f', 2));
        balanceHandler->savePrice(newPrice);
    } else {
        QMessageBox::warning(this, "Error de entrada", "El precio debe ser un valor positivo.");
    }
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
    bool ok;

    if (validateId(currentId)) {
        unsigned long long idInt = id.toULongLong(&ok, 16); 
        qDebug() << "ID recibido: " << currentId;
        Client client = db->getClientById(idInt);
        double currentBalance = db->getBalance(idInt);
        double chargeAmount = balanceHandler->loadPrice();

      if(!(isAddingCardMode || isChargingMode)){
        QString msg = QString::number(balanceHandler->debit(idInt, chargeAmount));
        serialHandler->sendToArduino(msg);
        // Verificación de saldo
      }
      if(isChargingMode){
        QString nombre = QString::fromStdString(client.getName());
        balanceHandler->openDialog(isChargingMode, nombre);
      }
      else if(isAddingCardMode){
        if(!client.isNull()){
          ui->textBrowser->append("ID ya existe en la base de datos. No se añade la tarjeta. El cliente es " + QString::fromStdString(client.getName()));
        }
        else if(!addCard(currentId)){
          ui->textBrowser->append("No se pudo agregar la tarjeta.");
        }
        else {
        ui->textBrowser->append("Tarjeta añadida con éxito.");
        }
        isAddingCardMode = false;
      }
    }
    else {
     ui->textBrowser->append("ID no válido: " + id);
    }
}

bool MainWindow::addCard(const QString &id){
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
        return 1;
    } catch (const std::runtime_error& e) {
        ui->textBrowser->append("Error al agregar el cliente: " + QString::fromStdString(e.what()));
        return 0;
    }
  }
}

void MainWindow::addVehicleToClient() {
    bool ok;
    QString inputName = QInputDialog::getText(this, "Agregar Vehículo",
                                              "Ingrese el nombre del cliente:",
                                              QLineEdit::Normal, "", &ok);
    
    // Verificar si se presionó "Cancelar" en el diálogo
    if (!ok) {
        ui->textBrowser->append("Operación de agregar vehículo cancelada.");
        return;
    }

    // Recupera el cliente utilizando clientId
    Client client;

    if ((client = db->getClientByName(inputName.toStdString())).isNull()) {
        QMessageBox::warning(this, "Cliente no encontrado", 
                             "El cliente con el nombre ingresado no existe en la base de datos.");
        return;
    } else {
        std::cout << client << std::endl;
    }

    unsigned long long clientId = client.getId();

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
            ui->textBrowser->append("Vehículo agregado al cliente " + inputName);
        } catch (const std::runtime_error& e) {
            ui->textBrowser->append("Error al agregar el vehículo: " + QString::fromStdString(e.what()));
        }
    } else {
        ui->textBrowser->append("Operación de agregar vehículo cancelada.");
    }
}

void MainWindow::onClosedChargeWindow(){
  isChargingMode = false;
}

void MainWindow::updateAddCardState(){
  if(serialHandler->isConnected()){
    ui->addCardButton->setEnabled(true);
    ui->addCardButton->setStyleSheet(
    "QPushButton {"
        "background-color: #2d2d2d;"
        "color: #ffffff;"
        "border: 1px solid #3c3c3c;"
        "padding: 5px;"
        "border-radius: 4px;"
    "}"
    "QPushButton:hover {"
        "background-color: #3c3c3c;"
    "}"
    );
  }
  else{
    ui->addCardButton->setEnabled(false);
    ui->addCardButton->setStyleSheet("background-color: #1c1c1c; color: #eeeeee;");
  }
}
