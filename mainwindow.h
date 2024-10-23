#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>  // Añade esta línea
#include <QListWidgetItem>  // Y esta línea
#include "serialhandler.h"
#include "lib/SAPEICore/DataBase.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  bool validateId(const QString &id);
  void addCard(const QString &id);


private slots:
    //void on_chargeButton_clicked();
    void on_addCardButton_clicked();
    //void on_cardListWidget_itemDoubleClicked(QListWidgetItem *item);
    void onIdReceived(const QString &id);  // Slot para manejar la señal de idReceived
    void onSelectSerialPortClicked();
                                         

private:
    
    SerialHandler *serialHandler;  
   
    DataBase *db;
    bool isAddingCardMode;
    bool isChargingMode;
    QString currentId;
    Ui::MainWindow *ui; 
};

#endif // MAINWINDOW_H
