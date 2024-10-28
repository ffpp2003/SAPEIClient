#ifndef ADDVEHICLEDIALOG_H
#define ADDVEHICLEDIALOG_H

#include <QDialog>

namespace Ui {
class AddVehicleDialog;
}

class AddVehicleDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddVehicleDialog(QWidget *parent = nullptr);
    ~AddVehicleDialog();

    QString getLicense() const;
    QString getType() const;
    QString getColor() const;
    QString getBrand() const;
    QString getModel() const;

private:
    Ui::AddVehicleDialog *ui;
};

#endif // ADDVEHICLEDIALOG_H
