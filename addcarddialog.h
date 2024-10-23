
#ifndef ADDCARDIALOG_H
#define ADDCARDIALOG_H

#include <QDialog>

namespace Ui {
class AddCardDialog;
}

class AddCardDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddCardDialog(QWidget *parent = nullptr);
    ~AddCardDialog();
    
    QString getName() const;
    QString getLastName() const;
    QString getDNI() const;
    QString getEmail() const;
    QString getPhone() const;
    QString getAddress() const;
    int getAge() const;
    QString getLicense() const;
    QString getType() const;
    QString getColor() const;
    QString getBrand() const;
    QString getModel() const;

private:
    Ui::AddCardDialog *ui;
};

#endif // ADDCARDIALOG_H
