#ifndef BALANCEHANDLER_H
#define BALANCEHANDLER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDialog>
#include "lib/SAPEICore/DataBase.h"

class BalanceHandler : public QObject {
    Q_OBJECT

public:
    explicit BalanceHandler(DataBase *db, QObject *parent = nullptr);

    // Métodos para manejar el balance de los clientes
    bool credit(unsigned long long clientId, double amount);          // Sobrecarga con clientId
    bool credit(const QString &name, double amount);                  // Sobrecarga con nombre
    bool debit(unsigned long long clientId, double amount);           // Sobrecarga con clientId
    bool debit(const QString &name, double amount);                   // Sobrecarga con nombre
    double loadPrice();
    void savePrice(double price);
    void setPrice(double newPrice);
    void openDialog();

private:
    bool updateBalance(Client &client, double amount);
    
    DataBase *db;  // Puntero a la base de datos para operar sobre los clientes
};

#endif // BALANCEHANDLER_H
