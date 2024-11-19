#include "addcarddialog.h"
#include "ui_addcarddialog.h"

AddCardDialog::AddCardDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddCardDialog) {
    ui->setupUi(this);
}

AddCardDialog::~AddCardDialog() {
    delete ui;
}

QString AddCardDialog::getName() const {
    return ui->nameLineEdit->text();
}

QString AddCardDialog::getLastName() const {
    return ui->lastNameLineEdit->text();
}

QString AddCardDialog::getDNI() const {
    return ui->dniLineEdit->text();
}

QString AddCardDialog::getEmail() const {
    return ui->emailLineEdit->text();
}

QString AddCardDialog::getPhone() const {
    return ui->phoneLineEdit->text();
}

QString AddCardDialog::getAddress() const {
    return ui->addressLineEdit->text();
}

int AddCardDialog::getAge() const {
    return ui->ageSpinBox->value();}

QString AddCardDialog::getLicense() const {
    return ui->licenseLineEdit->text();
}

QString AddCardDialog::getType() const {
    return ui->typeLineEdit->text();
}

QString AddCardDialog::getColor() const {
    return ui->colorLineEdit->text();
}

QString AddCardDialog::getBrand() const {
    return ui->brandLineEdit->text();
}

QString AddCardDialog::getModel() const {
    return ui->modelLineEdit->text();
}
