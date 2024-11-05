#include "vehiclelistdialog.h"
#include "ui_vehiclelistdialog.h"

VehicleListDialog::VehicleListDialog(DataBase* db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VehicleListDialog),
    database(db)
{
    ui->setupUi(this);

    // Cargar lista inicial
    updateVehicleList();

    // Conectar el cambio de texto del campo de búsqueda al slot de actualización de la lista
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &VehicleListDialog::onSearchTextChanged);

    connect(ui->editVehicleButton, &QPushButton::clicked, this, &VehicleListDialog::onEditVehicleButtonClicked);
    connect(ui->deleteVehicleButton, &QPushButton::clicked, this, &VehicleListDialog::onDeleteVehicleButtonClicked);
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
        // Crear la cadena con la patente, marca y modelo
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
        // Verificar si la patente, marca o modelo contiene el texto de búsqueda
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
    // Obtener el vehículo seleccionado
    QListWidgetItem *selectedItem = ui->VehicleListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un vehículo para editar.");
        return;
    }

    // Obtener la placa del vehículo seleccionado
    QString itemText = selectedItem->text();
    QStringList parts = itemText.split(" - ");  // Dividir el texto del elemento por " - "
    if (parts.isEmpty()) {
        QMessageBox::warning(this, "Error", "Formato de texto inválido en la lista de vehículos.");
        return;
    }
    std::string licensePlate = parts[0].toStdString();  // La placa es la primera parte

    // Buscar el vehículo en la base de datos usando la placa
    Vehicle vehicle = database->getVehicleByPlate(licensePlate);
    if (vehicle.isNull()) {
        QMessageBox::warning(this, "Error", "El vehículo no existe en la base de datos.");
        return;
    }

    unsigned long long clientId = vehicle.getClientId();
 
    // Abrir EditVehicleDialog con los datos del vehículo
    EditVehicleDialog editDialog(this);
    editDialog.setLicensePlate(QString::fromStdString(vehicle.getLicensePlate()));
    editDialog.setType(QString::fromStdString(vehicle.getType()));
    editDialog.setColor(QString::fromStdString(vehicle.getColor()));
    editDialog.setBrand(QString::fromStdString(vehicle.getBrand()));
    editDialog.setModel(QString::fromStdString(vehicle.getModel()));

    // Si se confirma la edición, actualizar los datos del vehículo
    if (editDialog.exec() == QDialog::Accepted) {
        // Crear un nuevo objeto Vehicle con los datos editados
        Vehicle updatedVehicle(
            editDialog.getLicensePlate().toStdString(),
            editDialog.getType().toStdString(),
            editDialog.getColor().toStdString(),
            editDialog.getBrand().toStdString(),
            editDialog.getModel().toStdString()
        );

        // Eliminar el vehículo existente
        database->rmVehicle(licensePlate);  // Aquí se usa rmVehicle en lugar de rmClient

        // Agregar el vehículo actualizado a la base de datos
        database->addVehicle(clientId, updatedVehicle); // Asumiendo que existe un método para agregar vehículos

        updateVehicleList();  // Refrescar la lista de vehículos
    }
}


void VehicleListDialog::onDeleteVehicleButtonClicked(){
    // Obtener el vehículo seleccionado
    QListWidgetItem *selectedItem = ui->VehicleListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un vehículo para eliminar.");
        return;
    }

    // Obtener la placa del vehículo seleccionado desde el texto del elemento
 QString itemText = selectedItem->text();
    QStringList parts = itemText.split(" - ");  // Divide el texto por " - "
    if (parts.isEmpty()) {
        QMessageBox::warning(this, "Error", "Formato de texto inválido en la lista de vehículos.");
        return;
    }
    
    QString licensePlateQString = parts[0];  // La placa es la primera parte
                                             //
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar eliminación",
                                  QString("¿Estás seguro de que deseas eliminar el vehículo con placa '%1'?").arg(licensePlateQString),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Eliminar el vehículo de la base de datos utilizando la placa
        database->rmVehicle(licensePlateQString.toStdString());

        // Refrescar la lista de vehículos
        updateVehicleList();  // Si estás usando una lista de clientes, renombra a `updateVehicleList()`
    }
}
