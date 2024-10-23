#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QSerialPort>
#include <QObject>

class SerialHandler : public QObject {
    Q_OBJECT

public:
    SerialHandler(QObject *parent = nullptr);
    void selectSerialPort();  // Nueva función para seleccionar el puerto
    void connectSerialPort(const QString &portName);
    void sendToArduino(const QString &message);
    bool isConnected() const;           // Método para verificar si está conectado

signals:
    void idReceived(const QString &id); // Señal que se emitirá al recibir un ID

private slots:
    void handleReadyRead();

private:
    QSerialPort *serial;
};

#endif // SERIALHANDLER_H
