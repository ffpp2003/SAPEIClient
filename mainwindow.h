/**
 * @file mainwindow.h
 * @brief Declaración de la clase MainWindow.
 *
 * Este archivo contiene la declaración de la clase MainWindow, que es la ventana principal de la aplicación.
 * Maneja la interacción con la interfaz gráfica de usuario, la comunicación serial, y la gestión de la base de datos.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "serialhandler.h"
#include "clientlistdialog.h"
#include "vehiclelistdialog.h"
#include "addcarddialog.h"
#include "addvehicledialog.h"
#include "balancehandler.h"
#include "lib/SAPEICore/DataBase.h"
#include "lib/SAPEICore/Client.h"
#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QDebug>
#include <QInputDialog>
#include <QLineEdit>
#include <cstdint>
#include <QMovie>
#include <QTimer>
#include <QGraphicsTextItem>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QLabel>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QResizeEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Clase principal que maneja la interfaz gráfica y la lógica de la aplicación.
 *
 * La clase MainWindow es responsable de interactuar con el usuario, manejar la recepción de IDs a través del puerto serie,
 * y gestionar la base de datos de tarjetas. Además, permite agregar nuevas tarjetas y seleccionar el puerto serie.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase MainWindow.
     * @param parent Puntero al widget padre. Si no se pasa, el valor predeterminado es nullptr.
     */
    MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor de la clase MainWindow.
     */
    ~MainWindow();

    /**
     * @brief Valida el ID recibido para determinar si es correcto.
     * @param id Cadena de texto con el ID a validar.
     * @return true si el ID es válido, false en caso contrario.
     */
    bool validateId(const QString &id);

    /**
     * @brief Agrega una tarjeta a la base de datos utilizando el ID proporcionado.
     * @param id Cadena de texto con el ID de la tarjeta.
     */
    bool addCard(const QString &id);


    void addVehicleToClient();
    void changePrice();
    void updatePriceDisplay();
    void openBalanceDialog();

private slots:
    /**
     * @brief Slot que maneja el evento cuando se presiona el botón para agregar una tarjeta.
     */
    void on_addCardButton_clicked();


    void onAddVehicleButtonClicked();
    /**
     * @brief Slot que maneja el evento cuando se recibe un ID desde el puerto serie.
     * @param id Cadena de texto con el ID recibido.
     */
    void onIdReceived(const QString &id);

    /**
     * @brief Slot que maneja la selección del puerto serie.
     */
    void onSelectSerialPortClicked();
    void onClientListButtonClicked();
    void onVehicleListButtonClicked();
    void updateConnectionStatus();
    void onConfirmPriceChangeClicked();
    void onBalanceUpdated(const QString &message);
    void onBalanceUpdateFailed(const QString &message);
    void updateAddCardState();
    void onClientNotFound();
private:
    void onClosedChargeWindow();
    SerialHandler *serialHandler; ///< Manejador de comunicación serial.
    DataBase *db; ///< Puntero a la base de datos utilizada para almacenar los clientes y tarjetas.
    BalanceHandler *balanceHandler;  // BalanceHandler como atributo
    QString currentId; ///< Almacena temporalmente el ID de la tarjeta actual.
    bool isAddingCardMode; ///< Indica si la aplicación está en modo de agregar tarjeta.
    bool isChargingMode; ///< Indica si la aplicación está en modo de carga de saldo.
    Ui::MainWindow *ui; ///< Puntero a la interfaz gráfica de la ventana principal.
    void setupFloatingGif();
protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // MAINWINDOW_H
