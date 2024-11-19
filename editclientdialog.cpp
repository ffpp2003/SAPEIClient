#include "editclientdialog.h"
#include "ui_editclientdialog.h"

EditClientDialog::EditClientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditClientDialog)
{
    ui->setupUi(this);

    connect(ui->saveButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

EditClientDialog::~EditClientDialog()
{
    delete ui;
}

QString EditClientDialog::getName() const {
    return ui->nameLineEdit->text();
}

QString EditClientDialog::getDNI() const {
    return ui->dniLineEdit->text();
}

QString EditClientDialog::getEmail() const {
    return ui->emailLineEdit->text();
}

QString EditClientDialog::getPhone() const {
    return ui->phoneLineEdit->text();
}

QString EditClientDialog::getAddress() const {
    return ui->addressLineEdit->text();
}

int EditClientDialog::getAge() const {
    return ui->ageSpinBox->value();
}

void EditClientDialog::setName(const QString &name) {
    ui->nameLineEdit->setText(name);
}

void EditClientDialog::setDNI(const QString &dni) {
    ui->dniLineEdit->setText(dni);
}

void EditClientDialog::setEmail(const QString &email) {
    ui->emailLineEdit->setText(email);
}

void EditClientDialog::setPhone(const QString &phone) {
    ui->phoneLineEdit->setText(phone);
}

void EditClientDialog::setAddress(const QString &address) {
    ui->addressLineEdit->setText(address);
}

void EditClientDialog::setAge(int age) {
    ui->ageSpinBox->setValue(age);
}
