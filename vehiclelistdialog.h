#ifndef VEHICLELISTDIALOG_H
#define VEHICLELISTDIALOG_H

#include <QDialog>
#include <QListWidget>
#include "lib/SAPEICore/DataBase.h"

namespace Ui {
class VehicleListDialog;
}

class VehicleListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VehicleListDialog(const std::vector<Vehicle>& vehicles, QWidget* parent = nullptr);
    ~VehicleListDialog();

private:
    Ui::VehicleListDialog *ui;
    QListWidget *vehicleListWidget;  // Lista de vehículos
};

#endif // VEHICLELISTDIALOG_H
