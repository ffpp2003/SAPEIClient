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
#include "ui_balancehandlerdialog.h"

class BalanceHandler : public QObject {
    Q_OBJECT

signals:
    void balanceUpdated(const QString &message);
    void balanceUpdateFailed(const QString &message);
    void windowClosed();  
    void clientNotFound();


public:
    explicit BalanceHandler(DataBase *db, QObject *parent = nullptr);

    int credit(unsigned long long clientId, double amount);
    int credit(const QString &name, double amount);
    int debit(unsigned long long clientId, double amount);
    int debit(const QString &name, double amount);
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
    DataBase *db;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // BALANCEHANDLER_H
