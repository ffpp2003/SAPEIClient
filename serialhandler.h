/**
 * @file serialhandler.h
 * @brief Declaración de la clase SerialHandler.
 *
 * Este archivo contiene la declaración de la clase SerialHandler, responsable de manejar la comunicación con el puerto serie,
 * enviar y recibir datos, y procesar los mensajes recibidos desde un dispositivo conectado por serial.
 */

#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <QSerialPort>
#include <QObject>

/**
 * @class SerialHandler
 * @brief Clase que maneja la comunicación serie con un dispositivo (por ejemplo, Arduino).
 *
 * La clase SerialHandler permite seleccionar un puerto serie, conectarse a él, enviar mensajes, 
 * y recibir datos desde el dispositivo conectado. También proporciona una señal que emite cuando se recibe un ID.
 */
class SerialHandler : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase SerialHandler.
     * @param parent Puntero al objeto padre. Si no se pasa, el valor predeterminado es nullptr.
     */
    SerialHandler(QObject *parent = nullptr);

    /**
     * @brief Selecciona el puerto serie para establecer la conexión.
     *
     * Este método abre un diálogo para que el usuario seleccione el puerto serie al que desea conectarse.
     */
    void selectSerialPort();

    /**
     * @brief Conecta al puerto serie especificado.
     * @param portName Nombre del puerto serie al que se conectará.
     */
    void connectSerialPort(const QString &portName);

    /**
     * @brief Envía un mensaje al dispositivo conectado por el puerto serie.
     * @param message Cadena de texto que contiene el mensaje a enviar.
     */
    void sendToArduino(const QString &message);

    /**
     * @brief Verifica si hay una conexión activa con el puerto serie.
     * @return true si está conectado, false en caso contrario.
     */
    bool isConnected() const;

signals:
    /**
     * @brief Señal que se emite cuando se recibe un ID desde el dispositivo serie.
     * @param id Cadena de texto que contiene el ID recibido.
     */
    void idReceived(const QString &id);

private slots:
    /**
     * @brief Slot privado que maneja la lectura de los datos entrantes desde el puerto serie.
     *
     * Este método es llamado automáticamente cuando hay datos disponibles para leer en el puerto serie.
     */
    void handleReadyRead();

private:
    QSerialPort *serial; ///< Puntero al objeto QSerialPort que maneja la comunicación serie.
};

#endif // SERIALHANDLER_H
