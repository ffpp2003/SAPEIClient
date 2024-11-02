#include "clientlistdialog.h"
#include "ui_clientlistdialog.h"

ClientListDialog::ClientListDialog(DataBase* db, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ClientListDialog),
    database(db)
{
    ui->setupUi(this);

    // Cargar lista inicial
    updateClientList();

    // Conectar el cambio de texto del campo de búsqueda al slot de actualización de la lista
    connect(ui->searchLineEdit, &QLineEdit::textChanged, this, &ClientListDialog::onSearchTextChanged);
    connect(ui->clientListWidget, &QListWidget::itemDoubleClicked, this, &ClientListDialog::onClientDoubleClicked);
}

ClientListDialog::~ClientListDialog()
{
    delete ui;
}

void ClientListDialog::updateClientList()
{
    ui->clientListWidget->clear();  // Usa el widget desde el puntero `ui`

    std::vector<Client> clients = database->getClients();
    for (const Client &client : clients) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(client.getName()));
        ui->clientListWidget->addItem(item);  // Usa el widget desde el puntero `ui`
    }
}

void ClientListDialog::onSearchTextChanged(const QString &text){
    ui->clientListWidget->clear();  // Usa el widget desde el puntero `ui`

    std::vector<Client> clients = database->getClients();
    for (const Client &client : clients) {
        if (QString::fromStdString(client.getName()).contains(text, Qt::CaseInsensitive)) {
            QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(client.getName()));
            ui->clientListWidget->addItem(item);  // Usa el widget desde el puntero `ui`
        }
    }
}

void ClientListDialog::onClientDoubleClicked(QListWidgetItem *item)
{
    std::string clientName = item->text().toStdString(); // Obtener el nombre del cliente seleccionado
    std::vector<Vehicle> vehicles = database->getVehicleByName(clientName);

    // Crear y mostrar la ventana de lista de vehículos
    VehicleListDialog *vehicleDialog = new VehicleListDialog(vehicles, this);
    vehicleDialog->exec();
}
