/**
 * @file addcarddialog.h
 * @brief Declaración de la clase AddCardDialog.
 *
 * Este archivo contiene la declaración de la clase AddCardDialog, 
 * que proporciona una interfaz de diálogo para agregar información de una tarjeta y sus detalles asociados.
 */

#ifndef ADDCARDIALOG_H
#define ADDCARDIALOG_H

#include <QDialog>

namespace Ui {
class AddCardDialog;
}

/**
 * @class AddCardDialog
 * @brief Clase que representa un cuadro de diálogo para agregar información de una tarjeta.
 *
 * La clase AddCardDialog hereda de QDialog y proporciona un diálogo en la interfaz gráfica
 * para ingresar detalles como nombre, apellido, DNI, email, teléfono, dirección, edad, 
 * información de licencia de vehículo y datos del vehículo (marca, modelo, color y tipo).
 */
class AddCardDialog : public QDialog {
    Q_OBJECT

public:
    /**
     * @brief Constructor de la clase AddCardDialog.
     * @param parent Puntero al widget padre. Si no se pasa, el valor predeterminado es nullptr.
     */
    explicit AddCardDialog(QWidget *parent = nullptr);

    /**
     * @brief Destructor de la clase AddCardDialog.
     */
    ~AddCardDialog();

    /**
     * @brief Obtiene el nombre ingresado en el diálogo.
     * @return Cadena de texto con el nombre.
     */
    QString getName() const;

    /**
     * @brief Obtiene el apellido ingresado en el diálogo.
     * @return Cadena de texto con el apellido.
     */
    QString getLastName() const;

    /**
     * @brief Obtiene el DNI ingresado en el diálogo.
     * @return Cadena de texto con el DNI.
     */
    QString getDNI() const;

    /**
     * @brief Obtiene el email ingresado en el diálogo.
     * @return Cadena de texto con el email.
     */
    QString getEmail() const;

    /**
     * @brief Obtiene el teléfono ingresado en el diálogo.
     * @return Cadena de texto con el número de teléfono.
     */
    QString getPhone() const;

    /**
     * @brief Obtiene la dirección ingresada en el diálogo.
     * @return Cadena de texto con la dirección.
     */
    QString getAddress() const;

    /**
     * @brief Obtiene la edad ingresada en el diálogo.
     * @return Número entero con la edad.
     */
    int getAge() const;

    /**
     * @brief Obtiene la licencia ingresada en el diálogo.
     * @return Cadena de texto con el número de licencia.
     */
    QString getLicense() const;

    /**
     * @brief Obtiene el tipo de vehículo ingresado en el diálogo.
     * @return Cadena de texto con el tipo de vehículo.
     */
    QString getType() const;

    /**
     * @brief Obtiene el color del vehículo ingresado en el diálogo.
     * @return Cadena de texto con el color del vehículo.
     */
    QString getColor() const;

    /**
     * @brief Obtiene la marca del vehículo ingresada en el diálogo.
     * @return Cadena de texto con la marca del vehículo.
     */
    QString getBrand() const;

    /**
     * @brief Obtiene el modelo del vehículo ingresado en el diálogo.
     * @return Cadena de texto con el modelo del vehículo.
     */
    QString getModel() const;

private:
    Ui::AddCardDialog *ui; ///< Puntero a la interfaz de usuario del diálogo.
};

#endif // ADDCARDIALOG_H
