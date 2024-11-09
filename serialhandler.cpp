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

#include <QElapsedTimer>
#include <QThread>

void SerialHandler::connectSerialPort(const QString &portName) {
    // Si el puerto serie ya está abierto, ciérralo antes de cambiar al nuevo puerto
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

        if (serial->waitForReadyRead(4000)) {  // Espera hasta 3 segundos para datos
            QByteArray initResponse = serial->readAll();
            // Enviar "SAPEI_INIT" al puerto serie
            sendToArduino("SAPEI_INIT");

            // Esperar "CONNECTED" en la respuesta
            if (serial->waitForReadyRead(3000)) {  // Espera hasta 3 segundos
                QByteArray connectedResponse = serial->readAll();
                if (connectedResponse.contains("CONNECTED")) {
                    qDebug() << "Conexión verificada: 'CONNECTED' recibido.";
                    connect(serial, &QSerialPort::readyRead, this, &SerialHandler::handleReadyRead);
                } else {
                    qDebug() << "No se recibió respuesta 'CONNECTED'.";
                    serial->close();
                }
            } else {
                qDebug() << "Timeout: No se recibió 'CONNECTED'.";
                serial->close();
            }
        }
    } else {
        qDebug() << "No se pudo abrir la conexión serial en" << portName;
        qDebug() << "Error:" << serial->errorString();
    }
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
        QString line = serial->readLine().trimmed();  // Lee la línea recibida
        emit idReceived(line);  // Emite la señal con el ID recibido
        qDebug() << "Datos leídos del puerto serie:" << line;
    }
}



bool SerialHandler::isConnected() const {
    // Devuelve true si el puerto serie está abierto
    return serial->isOpen();
}

