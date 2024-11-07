#include "balancehandler.h"
#include "ui_balancehandlerdialog.h"  // Incluir el archivo .ui

BalanceHandler::BalanceHandler(DataBase *db, QObject *parent)
    : QObject(parent), db(db) {}

bool BalanceHandler::credit(unsigned long long clientId, double amount) {
    Client client = db->getClientById(clientId);
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con ID:" << clientId;
        return false;
    }
    return updateBalance(client, amount);
}

bool BalanceHandler::credit(const QString &name, double amount) {
    Client client = db->getClientByName(name.toStdString());
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con nombre:" << name;
        return false;
    }
    return updateBalance(client, amount);
}

bool BalanceHandler::debit(unsigned long long clientId, double amount) {
    Client client = db->getClientById(clientId);
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con ID:" << clientId;
        return false;
    }
    return updateBalance(client, -amount);  // Restar del balance
}

bool BalanceHandler::debit(const QString &name, double amount) {
    Client client = db->getClientByName(name.toStdString());
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con nombre:" << name;
        return false;
    }
    return updateBalance(client, -amount);  // Restar del balance
}

bool BalanceHandler::updateBalance(Client &client, double amount) {
    double newBalance = client.getBalance() + amount;

    if (newBalance < 0) {
        emit balanceUpdateFailed("Fondos insuficientes para el cliente: " + QString::fromStdString(client.getName()));
        return false;
    }

    client.setBalance(newBalance);
    try {
        db->updateClient(client);
        emit balanceUpdated("Balance actualizado para el cliente: " + QString::fromStdString(client.getName()) + " Nuevo balance: " + QString::number(newBalance));
        return true;
    } catch (const std::runtime_error &e) {
        emit balanceUpdateFailed("Error al actualizar el balance: " + QString::fromStdString(e.what()));
        return false;
    }
}


double BalanceHandler::loadPrice() {
    QFile file("culocacapis.txt");
    double price;
    if (file.exists()) {
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            in >> price;
            file.close();
            qDebug() << "Precio cargado desde archivo:" << price;
            return price;
        } else {
            qDebug() << "No se pudo abrir el archivo para lectura.";
        }
    } else {
        price = 600.0;  // Precio por defecto
        savePrice(price);
        qDebug() << "Archivo no encontrado. Precio por defecto establecido a:" << price;
        return price;
    }
    return 0;
}

void BalanceHandler::savePrice(double price) {
    QFile file("culocacapis.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << price;
        file.close();
        qDebug() << "Precio guardado en archivo:" << price;
    } else {
        qDebug() << "No se pudo abrir el archivo para escritura.";
    }
}

void BalanceHandler::setPrice(double newPrice) {
    savePrice(newPrice);  // Guardar el nuevo precio en el archivo
}




void BalanceHandler::openDialog() {
    QDialog dialog;  // Crear el diálogo como variable local
    Ui::BalanceHandlerDialog ui;  // Instancia temporal para configurar la UI
    ui.setupUi(&dialog);

    // Configurar conexiones, por ejemplo, conectar el botón de aceptar o rechazar
    if (dialog.exec() == QDialog::Accepted) {
        QString clientName = ui.clientNameLineEdit->text();
        double amount = QLocale::system().toDouble(ui.amountSpinBox->text());

        if (!clientName.isEmpty() && amount != 0) {
            credit(clientName, amount);
        } else {
            qDebug() << "Nombre de cliente o monto inválido.";
        }
    }
}
