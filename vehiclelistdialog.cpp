#include "vehiclelistdialog.h"
#include "ui_vehiclelistdialog.h"

VehicleListDialog::VehicleListDialog(DataBase* db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VehicleListDialog),
    database(db)
{
    ui->setupUi(this);

    updateVehicleList();

    // Conectar el cambio de texto del campo de búsqueda al slot de actualización de la lista
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &VehicleListDialog::onSearchTextChanged);
    connect(ui->editVehicleButton, &QPushButton::clicked, this, &VehicleListDialog::onEditVehicleButtonClicked);
    connect(ui->deleteVehicleButton, &QPushButton::clicked, this, &VehicleListDialog::onDeleteVehicleButtonClicked);
    connect(ui->VehicleListWidget, &QListWidget::itemDoubleClicked, this, &VehicleListDialog::onVehicleDoubleClicked);
}

VehicleListDialog::~VehicleListDialog()
{
    delete ui;
}

void VehicleListDialog::updateVehicleList()
{
    ui->VehicleListWidget->clear();

    std::vector<Vehicle> vehicles = database->getAllVehicles();
    for (const Vehicle &vehicle : vehicles) {
        QString displayText = QString::fromStdString(vehicle.getLicensePlate() + " - " +
                                                     vehicle.getBrand() + " - " +
                                                     vehicle.getModel());
        QListWidgetItem *item = new QListWidgetItem(displayText);
        ui->VehicleListWidget->addItem(item);
    }
}

void VehicleListDialog::onSearchTextChanged(const QString &text)
{
    ui->VehicleListWidget->clear();

    std::vector<Vehicle> vehicles = database->getAllVehicles();
    for (const Vehicle &vehicle : vehicles) {
        QString displayText = QString::fromStdString(vehicle.getLicensePlate() + " - " +
                                                     vehicle.getBrand() + " - " +
                                                     vehicle.getModel());
        if (displayText.contains(text, Qt::CaseInsensitive)) {
            QListWidgetItem *item = new QListWidgetItem(displayText);
            ui->VehicleListWidget->addItem(item);
        }
    }
}


void VehicleListDialog::onEditVehicleButtonClicked()
{
    QListWidgetItem *selectedItem = ui->VehicleListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un vehículo para editar.");
        return;
    }

    QString itemText = selectedItem->text();
    QStringList parts = itemText.split(" - ");
    if (parts.isEmpty()) {
        QMessageBox::warning(this, "Error", "Formato de texto inválido en la lista de vehículos.");
        return;
    }
    std::string licensePlate = parts[0].toStdString();

    Vehicle vehicle = database->getVehicleByPlate(licensePlate);
    if (vehicle.isNull()) {
        QMessageBox::warning(this, "Error", "El vehículo no existe en la base de datos.");
        return;
    }

    unsigned long long clientId = vehicle.getClientId();
 
    EditVehicleDialog editDialog(this);
    editDialog.setLicensePlate(QString::fromStdString(vehicle.getLicensePlate()));
    editDialog.setType(QString::fromStdString(vehicle.getType()));
    editDialog.setColor(QString::fromStdString(vehicle.getColor()));
    editDialog.setBrand(QString::fromStdString(vehicle.getBrand()));
    editDialog.setModel(QString::fromStdString(vehicle.getModel()));

    // Si se confirma la edición, actualizar los datos del vehículo
    if (editDialog.exec() == QDialog::Accepted) {
        Vehicle updatedVehicle(
            editDialog.getLicensePlate().toStdString(),
            editDialog.getType().toStdString(),
            editDialog.getColor().toStdString(),
            editDialog.getBrand().toStdString(),
            editDialog.getModel().toStdString()
        );

        database->rmVehicle(licensePlate);
        database->addVehicle(clientId, updatedVehicle);

        updateVehicleList();
    }
}


void VehicleListDialog::onDeleteVehicleButtonClicked(){
    QListWidgetItem *selectedItem = ui->VehicleListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un vehículo para eliminar.");
        return;
    }

 QString itemText = selectedItem->text();
    QStringList parts = itemText.split(" - ");
    if (parts.isEmpty()) {
        QMessageBox::warning(this, "Error", "Formato de texto inválido en la lista de vehículos.");
        return;
    }

    QString licensePlateQString = parts[0];

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar eliminación",
                                  QString("¿Estás seguro de que deseas eliminar el vehículo con placa '%1'?").arg(licensePlateQString),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        database->rmVehicle(licensePlateQString.toStdString());
        updateVehicleList();
    }
}

void VehicleListDialog::onVehicleDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;

    QString itemText = item->text();
    QStringList parts = itemText.split(" - ");
    if (parts.isEmpty()) return;

    std::string licensePlate = parts[0].toStdString();
    Vehicle vehicle = database->getVehicleByPlate(licensePlate);
    if (vehicle.isNull()) return;

    Client client = database->getClientById(vehicle.getClientId());

    ui->typeLabel->setText("Tipo: " + QString::fromStdString(vehicle.getType()));
    ui->colorLabel->setText("Color: " + QString::fromStdString(vehicle.getColor()));
    ui->brandLabel->setText("Marca: " + QString::fromStdString(vehicle.getBrand()));
    ui->modelLabel->setText("Modelo: " + QString::fromStdString(vehicle.getModel()));
    ui->clientInfoLabel->setText("Cliente: " + QString::fromStdString(client.getName()));
    ui->licensePlateLabel->setText("Placa: " + QString::fromStdString(vehicle.getLicensePlate()));
    ui->idLabel->setText("ID: " + QString::number(client.getId()));  
    ui->hexIdLabel->setText("ID: " + QString::number(client.getId(), 16).toUpper());
}
