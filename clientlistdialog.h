#ifndef CLIENTLISTDIALOG_H
#define CLIENTLISTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QListWidgetItem>
#include "lib/SAPEICore/Client.h"
#include "lib/SAPEICore/DataBase.h"
#include "editclientdialog.h"


namespace Ui {
class ClientListDialog;}

class ClientListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientListDialog(DataBase* db, QWidget* parent = nullptr);
    ~ClientListDialog();

private slots:
    void updateClientList();
    void onSearchTextChanged(const QString &text);
    void onClientDoubleClicked(QListWidgetItem* item);
    void onEditClientButtonClicked();
    void onDeleteClientButtonClicked();
private:
    Ui::ClientListDialog *ui;
    DataBase *database;
    void updateClient();
    void deleteClient();
};
#endif // CLIENTLISTDIALOG_H
