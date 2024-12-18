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

    // Aplicar tema oscuro
    dialog.setStyleSheet(R"(
        QDialog {
            background-color: #2e2e2e;
            color: #ffffff;
        }
        QLabel, QComboBox, QPushButton {
            color: #ffffff;
        }
        QComboBox {
            background-color: #3c3c3c;
            border: 1px solid #5e5e5e;
        }
        QComboBox QAbstractItemView {
            background-color: #3c3c3c;
            selection-background-color: #5e5e5e;
            color: #ffffff;
        }
        QDialogButtonBox QPushButton {
            background-color: #5e5e5e;
            border: 1px solid #5e5e5e;
            color: #ffffff;
        }
        QDialogButtonBox QPushButton:hover {
            background-color: #757575;
        }
    )");

    QVBoxLayout layout(&dialog);

    // ComboBox para listar los puertos serie disponibles
    QComboBox comboBox;

    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &port : ports) {
        comboBox.addItem(port.portName());
    }

    // Agregar puertos PTY manualmente
    for (int i = 0; i < 256; ++i) {
        QString ptName = QString("/dev/pts/%1").arg(i);
        if (QFile::exists(ptName)) {
            comboBox.addItem(ptName);
        }
    }

    layout.addWidget(&comboBox);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    layout.addWidget(&buttonBox);

    if (dialog.exec() == QDialog::Accepted) {
        QString selectedPortName = comboBox.currentText();
        if (!selectedPortName.isEmpty()) {
            if(connectSerialPort(selectedPortName))
              qDebug() << "Error para conectarse a: " << selectedPortName;
            else
              qDebug() << "Conectado a: " << selectedPortName;
        } else {
            qDebug() << "No se seleccionó ningún puerto.";
        }
    }
}

#include <QElapsedTimer>
#include <QThread>

int SerialHandler::connectSerialPort(const QString &portName) {
    if (serial->isOpen()) {
        serial->close();
        qDebug() << "Cerrando conexión en el puerto anterior.";
    }

    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Conexión serial abierta en" << portName;
            serial->waitForReadyRead(4000);
            QByteArray initResponse = serial->readAll();
            sendToArduino("SAPEI_INIT");
            if (serial->waitForReadyRead(3000)) {
                QByteArray connectedResponse = serial->readAll();
                if (connectedResponse.contains("CONNECTED")) {
                    qDebug() << "Conexión verificada: 'CONNECTED' recibido.";
                    connect(serial, &QSerialPort::readyRead, this, &SerialHandler::handleReadyRead);
                    return 0;
                } else {
                    qDebug() << "No se recibió respuesta 'CONNECTED'.";
                    serial->close();
                    return 1;
                }
            } else {
                qDebug() << "Timeout: No se recibió 'CONNECTED'.";
                serial->close();
                return 1;
            }
    } else {
        qDebug() << "No se pudo abrir la conexión serial en" << portName;
        qDebug() << "Error:" << serial->errorString();
        return 1;
    }
    return 1;
}

void SerialHandler::sendToArduino(const QString &message) {
    if (serial->isOpen()) {
        QByteArray byteArray = (message + "\n").toUtf8();
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
        QString line = serial->readLine().trimmed();
        emit idReceived(line);
        qDebug() << "Datos leídos del puerto serie:" << line;
    }
}



bool SerialHandler::isConnected() const {
    return serial->isOpen();
}

