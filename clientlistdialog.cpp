#include "clientlistdialog.h"
#include "ui_clientlistdialog.h"

ClientListDialog::ClientListDialog(DataBase* db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientListDialog),
    database(db)
{
    ui->setupUi(this);

    updateClientList();

    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &ClientListDialog::onSearchTextChanged);
    connect(ui->clientListWidget, &QListWidget::itemDoubleClicked, this, &ClientListDialog::onClientDoubleClicked);

    connect(ui->editClientButton, &QPushButton::clicked, this, &ClientListDialog::onEditClientButtonClicked);
    connect(ui->deleteClientButton, &QPushButton::clicked, this, &ClientListDialog::onDeleteClientButtonClicked);
}

ClientListDialog::~ClientListDialog()
{
    delete ui;
}

void ClientListDialog::updateClientList()
{
    ui->clientListWidget->clear(); 

    std::vector<Client> clients = database->getAllClients();
    for (const Client &client : clients) {
    QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(client.getName()) + " - DNI: " + QString::number(client.getDni()));
        ui->clientListWidget->addItem(item);  
    }
}

void ClientListDialog::onSearchTextChanged(const QString &text)
{
    ui->clientListWidget->clear();

    std::vector<Client> clients = database->getAllClients();
    for (const Client &client : clients) {
        QString name = QString::fromStdString(client.getName());
        QString dni = QString::number(client.getDni());
        
        if (name.contains(text, Qt::CaseInsensitive) || dni.contains(text, Qt::CaseInsensitive)) {
            QString displayText = name + " - DNI: " + dni;
            QListWidgetItem *item = new QListWidgetItem(displayText);
            ui->clientListWidget->addItem(item);
        }
    }
}

void ClientListDialog::onClientDoubleClicked(QListWidgetItem *item) {
    QString clientText = item->text();
    QStringList parts = clientText.split(" - DNI: "); 

    std::string clientName = parts[0].toStdString();

    Client client = database->getClientByName(clientName);

    ui->nameLabel->setText("Nombre: " + QString::fromStdString(client.getName()));
    ui->dniLabel->setText("DNI: " + QString::number(client.getDni()));
    ui->emailLabel->setText("Email: " + QString::fromStdString(client.getEmail()));
    ui->phoneLabel->setText("Teléfono: " + QString::fromStdString(client.getPhone()));
    ui->addressLabel->setText("Dirección: " + QString::fromStdString(client.getAddress()));
    ui->ageLabel->setText("Edad: " + QString::number(client.getAge()));
    ui->idLabel->setText("ID: " + QString::number(client.getId()));
    ui->hexIdLabel->setText("ID Hexadecimal: " + QString::number(client.getId(), 16).toUpper());
    ui->balanceLabel->setText("Saldo: " + QString::number(client.getBalance()));

    std::vector<Vehicle> vehicles = database->getVehiclesByClientId(client.getId());

 ui->vehicleTableWidget->setRowCount(0);  
    int rowCount = static_cast<int>(vehicles.size());
    ui->vehicleTableWidget->setRowCount(rowCount);  
    for (size_t row = 0; row < vehicles.size(); ++row) {
        const Vehicle &vehicle = vehicles[row];

 ui->vehicleTableWidget->setItem(static_cast<int>(row), 0, new QTableWidgetItem(QString::fromStdString(vehicle.getLicensePlate())));
        ui->vehicleTableWidget->setItem(static_cast<int>(row), 1, new QTableWidgetItem(QString::fromStdString(vehicle.getBrand())));
        ui->vehicleTableWidget->setItem(static_cast<int>(row), 2, new QTableWidgetItem(QString::fromStdString(vehicle.getModel())));
        ui->vehicleTableWidget->setItem(static_cast<int>(row), 3, new QTableWidgetItem(QString::fromStdString(vehicle.getColor())));
        ui->vehicleTableWidget->setItem(static_cast<int>(row), 4, new QTableWidgetItem(QString::fromStdString(vehicle.getType())));
    }
}

void ClientListDialog::onEditClientButtonClicked(){

  updateClient();

}


void ClientListDialog::onDeleteClientButtonClicked(){
  deleteClient();
}

void ClientListDialog::deleteClient(){
    QListWidgetItem *selectedItem = ui->clientListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un cliente para eliminar.");
        return;
    }
    
    std::string clientName = selectedItem->text().toStdString();
    Client client = database->getClientByName(clientName);
    unsigned long long clientId = client.getId();

    QString clientNameQString = QString::fromStdString(clientName);
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar eliminación",
                                  QString("¿Estás seguro de que deseas eliminar al cliente '%1'?").arg(clientNameQString),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        database->rmClient(clientId);
        updateClientList();
    }
}

void ClientListDialog::updateClient(){
    QListWidgetItem *selectedItem = ui->clientListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un cliente para editar.");
        return;
    }
    
    std::string clientName = selectedItem->text().toStdString();
    Client client = database->getClientByName(clientName);
    
    EditClientDialog editDialog(this);
    editDialog.setName(QString::fromStdString(client.getName()));
    editDialog.setDNI(QString::number(client.getDni()));  
    editDialog.setEmail(QString::fromStdString(client.getEmail()));
    editDialog.setPhone(QString::fromStdString(client.getPhone()));
    editDialog.setAddress(QString::fromStdString(client.getAddress()));
    editDialog.setAge(client.getAge());
    
    if (editDialog.exec() == QDialog::Accepted) {
        
            client.setName(editDialog.getName().toStdString());
            client.setAge(editDialog.getAge());
            client.setDni(editDialog.getDNI().toUInt());
            client.setAddress(editDialog.getAddress().toStdString());
            client.setEmail(editDialog.getEmail().toStdString());
            client.setPhone(editDialog.getPhone().toStdString());
        
        database->updateClient(client);
        updateClientList();  
    }
}
