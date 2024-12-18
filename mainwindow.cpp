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

    setupFloatingGif();

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
    connect(balanceHandler, &BalanceHandler::clientNotFound, this, &MainWindow::onClientNotFound);
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
            out << 600;
            configFile.close();
        }
    }

    if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&configFile);
        double fileprice;
        in >> fileprice;
        configFile.close();

        updatePriceDisplay();
    }

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSelectSerialPortClicked() {
    serialHandler->selectSerialPort();
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);

    QGraphicsView *connectionStatusView = findChild<QGraphicsView *>("connectionStatusView");
    if (!connectionStatusView) return;

    QLabel *floatingGif = connectionStatusView->findChild<QLabel *>("floatingGif");
    if (!floatingGif) return;

    int gifWidth = 100;
    int gifHeight = 100;
    floatingGif->setGeometry(connectionStatusView->width() - gifWidth,
                             0, 
                             gifWidth, 
                             gifHeight);
}

void MainWindow::setupFloatingGif() {
    QGraphicsView *connectionStatusView = findChild<QGraphicsView *>("connectionStatusView");

    QLabel *floatingGif = new QLabel(connectionStatusView);
    floatingGif->setAttribute(Qt::WA_TranslucentBackground); // Fondo transparente
    floatingGif->setAttribute(Qt::WA_NoSystemBackground);
    floatingGif->setObjectName("floatingGif");

    QMovie *gifMovie = new QMovie(":/utnlogo.gif");

    int gifWidth = 100;
    int gifHeight = 100;
    gifMovie->setScaledSize(QSize(gifWidth, gifHeight));

    floatingGif->setMovie(gifMovie);
    gifMovie->start();

    floatingGif->setGeometry(connectionStatusView->width() - gifWidth, 
                             0, 
                             gifWidth, 
                             gifHeight);

    floatingGif->raise();
}

void MainWindow::updateConnectionStatus() {
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
    font.setPointSize(24);
    font.setBold(true);
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
    addVehicleToClient();
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
    ui->textBrowser->append("Éxito: " + message);
}

void MainWindow::onBalanceUpdateFailed(const QString &message) {
    ui->textBrowser->append("Error: " + message);
}

void MainWindow::onClientNotFound(){
    ui->textBrowser->append("Cliente no registrado");
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

    if (!ok) {
        ui->textBrowser->append("Operación de agregar vehículo cancelada.");
        return;
    }

    Client client;

    if ((client = db->getClientByName(inputName.toStdString())).isNull()) {
        QMessageBox::warning(this, "Cliente no encontrado", 
                             "El cliente con el nombre ingresado no existe en la base de datos.");
        return;
    } else {
        std::cout << client << std::endl;
    }

    unsigned long long clientId = client.getId();

    AddVehicleDialog vehicleDialog(this);
    if (vehicleDialog.exec() == QDialog::Accepted) {
        QString license = vehicleDialog.getLicense();
        QString type = vehicleDialog.getType();
        QString color = vehicleDialog.getColor();
        QString brand = vehicleDialog.getBrand();
        QString model = vehicleDialog.getModel();

        Vehicle newVehicle(license.toStdString(), type.toStdString(),
                           color.toStdString(), brand.toStdString(),
                           model.toStdString());

        try {
            db->addVehicle(clientId, newVehicle);
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
