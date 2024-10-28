#include "addvehicledialog.h"
#include "ui_addvehicledialog.h"

AddVehicleDialog::AddVehicleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddVehicleDialog) {
    ui->setupUi(this);
}

AddVehicleDialog::~AddVehicleDialog() {
    delete ui;
}

QString AddVehicleDialog::getLicense() const {
    return ui->licenseLineEdit->text();
}

QString AddVehicleDialog::getType() const {
    return ui->typeLineEdit->text();
}

QString AddVehicleDialog::getColor() const {
    return ui->colorLineEdit->text();
}

QString AddVehicleDialog::getBrand() const {
    return ui->brandLineEdit->text();
}

QString AddVehicleDialog::getModel() const {
    return ui->modelLineEdit->text();
}

