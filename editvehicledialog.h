#ifndef EDITVEHICLEDIALOG_H
#define EDITVEHICLEDIALOG_H

#include <QDialog>

namespace Ui {
class EditVehicleDialog;
}

class EditVehicleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditVehicleDialog(QWidget *parent = nullptr);
    ~EditVehicleDialog();

    void setLicensePlate(const QString &licensePlate);
    void setType(const QString &type);
    void setColor(const QString &color);
    void setBrand(const QString &brand);
    void setModel(const QString &model);

    QString getLicensePlate() const;
    QString getType() const;
    QString getColor() const;
    QString getBrand() const;
    QString getModel() const;

private:
    Ui::EditVehicleDialog *ui;
};

#endif // EDITVEHICLEDIALOG_H
