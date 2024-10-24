#include "serialhandler.h"
#include <QDebug>
#include <QDialog>
#include <QVBoxLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QSerialPortInfo>
#include <QFile>

SerialHandler::SerialHandler(QObject *parent) 
    : QObject(parent), serial(new QSerialPort(this)) {
}

void SerialHandler::selectSerialPort() {
    QDialog dialog;
    dialog.setWindowTitle("Seleccionar Puerto Serie");

    QVBoxLayout layout(&dialog);

    // ComboBox para listar los puertos serie disponibles
    QComboBox comboBox;
    
    // Lista de puertos serie
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        comboBox.addItem(port.portName());  // Agrega los puertos disponibles (ejemplo: COM3, /dev/ttyUSB0)
    }

    // Agregar puertos PTY manualmente
    for (int i = 0; i < 256; ++i) {  // Cambia el límite según sea necesario
        QString ptName = QString("/dev/pts/%1").arg(i);
        if (QFile::exists(ptName)) {
            comboBox.addItem(ptName);
        }
    }

    layout.addWidget(&comboBox);

    // Botones OK y Cancelar
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout.addWidget(&buttonBox);

    // Ejecutar diálogo
    if (dialog.exec() == QDialog::Accepted) {
        QString selectedPortName = comboBox.currentText();
        if (!selectedPortName.isEmpty()) {
            connectSerialPort(selectedPortName);  // Conectar al puerto seleccionado
            qDebug() << "Conectado a: " << selectedPortName;
        } else {
            qDebug() << "No se seleccionó ningún puerto.";
        }
    }
}
void SerialHandler::connectSerialPort(const QString &portName) {
    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        connect(serial, &QSerialPort::readyRead, this, &SerialHandler::handleReadyRead);
        qDebug() << "Serial connection opened on" << portName;
    } else {
        qDebug() << "Failed to open serial connection on" << portName;
        qDebug() << "Error:" << serial->errorString();  // Output detailed error
    }
}

void SerialHandler::sendToArduino(const QString &message) {
    if (serial->isOpen()) {
        QByteArray byteArray = (message + "\r\n").toUtf8();
        serial->write(byteArray);

        if (!serial->waitForBytesWritten(1000)) {
            qDebug() << "Error al enviar datos al Arduino:" << serial->errorString();
        }
    } else {
        qDebug() << "El puerto serie no está abierto.";
    }
}

void SerialHandler::handleReadyRead() {
    while (serial->canReadLine()) {
        QString line = serial->readLine().trimmed();  // Lee la línea recibida
        emit idReceived(line);  // Emite la señal con el ID recibido
        qDebug() << "Datos leídos del puerto serie:" << line;
    }
}



bool SerialHandler::isConnected() const {
    // Devuelve true si el puerto serie está abierto
    return serial->isOpen();
}

