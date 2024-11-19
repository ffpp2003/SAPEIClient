#include "editvehicledialog.h"
#include "ui_editvehicledialog.h"

EditVehicleDialog::EditVehicleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditVehicleDialog)
{
    ui->setupUi(this);

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

EditVehicleDialog::~EditVehicleDialog()
{
    delete ui;
}

void EditVehicleDialog::setLicensePlate(const QString &licensePlate) {
    ui->licensePlateLineEdit->setText(licensePlate);
}

void EditVehicleDialog::setType(const QString &type) {
    ui->typeLineEdit->setText(type);
}

void EditVehicleDialog::setColor(const QString &color) {
    ui->colorLineEdit->setText(color);
}

void EditVehicleDialog::setBrand(const QString &brand) {
    ui->brandLineEdit->setText(brand);
}

void EditVehicleDialog::setModel(const QString &model) {
    ui->modelLineEdit->setText(model);
}

QString EditVehicleDialog::getLicensePlate() const {
    return ui->licensePlateLineEdit->text();
}

QString EditVehicleDialog::getType() const {
    return ui->typeLineEdit->text();
}

QString EditVehicleDialog::getColor() const {
    return ui->colorLineEdit->text();
}

QString EditVehicleDialog::getBrand() const {
    return ui->brandLineEdit->text();
}

QString EditVehicleDialog::getModel() const {
    return ui->modelLineEdit->text();
}
