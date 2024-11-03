#ifndef EDITCLIENTDIALOG_H
#define EDITCLIENTDIALOG_H

#include <QDialog>

namespace Ui {
class EditClientDialog;
}

class EditClientDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditClientDialog(QWidget *parent = nullptr);
    ~EditClientDialog();

    // Métodos para obtener los datos del cliente
    QString getName() const;
    QString getDNI() const;
    QString getEmail() const;
    QString getPhone() const;
    QString getAddress() const;
    int getAge() const;

    // Métodos para establecer los datos del cliente (al abrir en modo edición)
    void setName(const QString &name);
    void setDNI(const QString &dni);
    void setEmail(const QString &email);
    void setPhone(const QString &phone);
    void setAddress(const QString &address);
    void setAge(int age);

private:
    Ui::EditClientDialog *ui;
};

#endif // EDITCLIENTDIALOG_H
