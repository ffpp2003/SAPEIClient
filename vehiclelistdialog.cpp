#include "vehiclelistdialog.h"
#include "ui_vehiclelistdialog.h"

VehicleListDialog::VehicleListDialog(const std::vector<Vehicle>& vehicles, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VehicleListDialog)
{
    ui->setupUi(this);
    
    // Configura el número de filas
    ui->vehicleTableWidget->setRowCount(vehicles.size());
    
    // Rellena la tabla con los datos
    for (int i = 0; i < vehicles.size(); ++i) {
        const auto& vehicle = vehicles[i];
        ui->vehicleTableWidget->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(vehicle.getLicensePlate())));
        ui->vehicleTableWidget->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(vehicle.getType())));
        ui->vehicleTableWidget->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(vehicle.getColor())));
        ui->vehicleTableWidget->setItem(i, 3, new QTableWidgetItem(QString::fromStdString(vehicle.getBrand())));
        ui->vehicleTableWidget->setItem(i, 4, new QTableWidgetItem(QString::fromStdString(vehicle.getModel())));
    }
    
    ui->vehicleTableWidget->resizeColumnsToContents();
}

VehicleListDialog::~VehicleListDialog()
{
    delete ui;
}
