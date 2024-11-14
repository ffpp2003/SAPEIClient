#include "balancehandler.h"
#include "ui_balancehandlerdialog.h"  // Incluir el archivo .ui

BalanceHandler::BalanceHandler(DataBase *db, QObject *parent)
    : QObject(parent), price(600), db(db) {}

int BalanceHandler::credit(unsigned long long clientId, double amount) {
    Client client = db->getClientById(clientId);
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con ID:" << clientId;
        return TR_CLIENT_NOT_FOUND;
    }
    return updateBalance(client, amount);
}

int BalanceHandler::credit(const QString &name, double amount) {
    Client client = db->getClientByName(name.toStdString());
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con nombre:" << name;
        return TR_CLIENT_NOT_FOUND;
    }
    return updateBalance(client, amount);
}

int BalanceHandler::debit(unsigned long long clientId, double amount) {
    Client client = db->getClientById(clientId);
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con ID:" << clientId;
        return TR_CLIENT_NOT_FOUND;
    }
    return updateBalance(client, -amount);  // Restar del balance
}

int BalanceHandler::debit(const QString &name, double amount) {
    Client client = db->getClientByName(name.toStdString());
    if (client.isNull()) {
        qDebug() << "Cliente no encontrado con nombre:" << name;
        return TR_CLIENT_NOT_FOUND;
    }
    return updateBalance(client, -amount);  // Restar del balance
}

int BalanceHandler::updateBalance(Client &client, double amount) {
    double newBalance = client.getBalance() + amount;

    if (newBalance < 0) {
        emit balanceUpdateFailed("Fondos insuficientes para el cliente: " + QString::fromStdString(client.getName()));
        return TR_NOT_ENOUGH_FUNDS;
    }

    client.setBalance(newBalance);
    try {
        db->updateClient(client);
        emit balanceUpdated("Balance actualizado para el cliente: " + QString::fromStdString(client.getName()) + " Nuevo balance: " + QString::number(newBalance));
        if(newBalance < price)
          return TR_JUST_ENOUGH_FUNDS;
        return OK;
    } catch (const std::runtime_error &e) {
        emit balanceUpdateFailed("Error al actualizar el balance: " + QString::fromStdString(e.what()));
        return TR_ERROR;
    }
}


double BalanceHandler::loadPrice() {
    QFile file("culocacapis.txt");
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
