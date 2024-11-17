#ifndef BALANCEHANDLER_H
#define BALANCEHANDLER_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDialog>
#include <QCloseEvent>
#include "QString"
#include "lib/SAPEICore/DataBase.h"
#include "lib/SAPEICore/Error.h"
#include "ui_balancehandlerdialog.h"  // Incluir el archivo .ui

class BalanceHandler : public QObject {
    Q_OBJECT

signals:
    void balanceUpdated(const QString &message);  // Señal para notificar a MainWindow
    void balanceUpdateFailed(const QString &message);
    void windowClosed();  // Custom signal to indicate window closure
    void clientNotFound();

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
    void completeName(const QString &name, Ui::BalanceHandlerDialog &ui);
    int openDialog(int isCharging = 0, const QString &name = "");

    Ui::BalanceHandlerDialog ui;

private:
    QDialog dialog;
    int updateBalance(Client &client, double amount);
    double price;
    DataBase *db;  // Puntero a la base de datos para operar sobre los clientes

protected:
    // Event filter method to detect the close event of the dialog
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // BALANCEHANDLER_H
