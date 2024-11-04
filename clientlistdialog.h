#ifndef CLIENTLISTDIALOG_H
#define CLIENTLISTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QListWidgetItem>
#include "lib/SAPEICore/Client.h"
#include "lib/SAPEICore/DataBase.h"
#include "clientinfodialog.h"  // Incluir el nuevo diálogo de vehículos
#include "editclientdialog.h"


namespace Ui {
class ClientListDialog; // Asegúrate de que esta línea esté presente
}

class ClientListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClientListDialog(DataBase* db, QWidget* parent = nullptr);
    ~ClientListDialog();

private slots:
    void updateClientList();               // Muestra los nombres de los clientes en la lista
    void onSearchTextChanged(const QString &text);  // Filtra los clientes por nombre
    void onClientDoubleClicked(QListWidgetItem* item);  // Slot para doble clic en cliente
    void onEditClientButtonClicked();
    void onDeleteClientButtonClicked();
private:
    Ui::ClientListDialog *ui;
    DataBase *database;
};
#endif // CLIENTLISTDIALOG_H
