#ifndef BALANCEHANDLER_H
#define BALANCEHANDLER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDialog>
#include "QString"
#include "lib/SAPEICore/DataBase.h"
#include "lib/SAPEICore/Error.h"

class BalanceHandler : public QObject {
    Q_OBJECT

signals:
    void balanceUpdated(const QString &message);  // Señal para notificar a MainWindow
    void balanceUpdateFailed(const QString &message);

public:
    explicit BalanceHandler(DataBase *db, QObject *parent = nullptr);

    // Métodos para manejar el balance de los clientes
    int credit(unsigned long long clientId, double amount);          // Sobrecarga con clientId
    int credit(const QString &name, double amount);                  // Sobrecarga con nombre
    int debit(unsigned long long clientId, double amount);           // Sobrecarga con clientId
    int debit(const QString &name, double amount);                   // Sobrecarga con nombre
    double loadPrice();
    void savePrice(double price);
    void setPrice(double newPrice);
    void openDialog();

private:
    int updateBalance(Client &client, double amount);
    double price;
    DataBase *db;  // Puntero a la base de datos para operar sobre los clientes
};

#endif // BALANCEHANDLER_H
