#ifndef CLIENTLISTDIALOG_H
#define CLIENTLISTDIALOG_H

#include <QDialog>
#include "lib/SAPEICore/DataBase.h"
#include "vehiclelistdialog.h"  // Incluir el nuevo diálogo de vehículos


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

private:
    Ui::ClientListDialog *ui;
    DataBase *database;
};
#endif // CLIENTLISTDIALOG_H
