#ifndef VEHICLELISTDIALOG_H
#define VEHICLELISTDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <QListWidgetItem>
#include "lib/SAPEICore/Client.h"
#include "lib/SAPEICore/DataBase.h"
#include "editvehicledialog.h"


namespace Ui {
    class VehicleListDialog;
}

class VehicleListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VehicleListDialog(DataBase* db, QWidget* parent = nullptr);
    ~VehicleListDialog();

private slots:
    void updateVehicleList();
    void onSearchTextChanged(const QString &text);
    void onEditVehicleButtonClicked();
    void onDeleteVehicleButtonClicked();
    void onVehicleDoubleClicked(QListWidgetItem *item);
private:
    Ui::VehicleListDialog *ui;
    DataBase *database;
};
#endif // VEHICLLISTDIALOG_H

