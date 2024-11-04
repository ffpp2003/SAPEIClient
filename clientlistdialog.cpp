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

    connect(ui->editClientButton, &QPushButton::clicked, this, &ClientListDialog::onEditClientButtonClicked);
    connect(ui->deleteClientButton, &QPushButton::clicked, this, &ClientListDialog::onDeleteClientButtonClicked);
}

ClientListDialog::~ClientListDialog()
{
    delete ui;
}

void ClientListDialog::updateClientList()
{
    ui->clientListWidget->clear();  // Usa el widget desde el puntero `ui`

    std::vector<Client> clients = database->getAllClients();
    for (const Client &client : clients) {
        QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(client.getName()));
        ui->clientListWidget->addItem(item);  // Usa el widget desde el puntero `ui`
    }
}

void ClientListDialog::onSearchTextChanged(const QString &text){
    ui->clientListWidget->clear();  // Usa el widget desde el puntero `ui`

    std::vector<Client> clients = database->getAllClients();
    for (const Client &client : clients) {
        if (QString::fromStdString(client.getName()).contains(text, Qt::CaseInsensitive)) {
            QListWidgetItem *item = new QListWidgetItem(QString::fromStdString(client.getName()));
            ui->clientListWidget->addItem(item);  // Usa el widget desde el puntero `ui`
        }
    }
}

void ClientListDialog::onClientDoubleClicked(QListWidgetItem *item)
{
}

void ClientListDialog::onEditClientButtonClicked()
{
    // Obtener el cliente seleccionado
    QListWidgetItem *selectedItem = ui->clientListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un cliente para editar.");
        return;
    }
    
    // Obtener el nombre del cliente seleccionado y buscar sus datos
    std::string clientName = selectedItem->text().toStdString();
    Client client = database->getClientByName(clientName);
    
    unsigned long long clientId = client.getId();
    // Guardar los vehículos del cliente original
    std::vector<Vehicle> originalVehicles = client.getVehicles();
    double originalBalance = client.getBalance();
    
    // Abrir EditClientDialog con los datos del cliente
    EditClientDialog editDialog(this);
    editDialog.setName(QString::fromStdString(client.getName()));
    editDialog.setDNI(QString::number(client.getDni()));  
    editDialog.setEmail(QString::fromStdString(client.getEmail()));
    editDialog.setPhone(QString::fromStdString(client.getPhone()));
    editDialog.setAddress(QString::fromStdString(client.getAddress()));
    editDialog.setAge(client.getAge());
    
    if (editDialog.exec() == QDialog::Accepted) {
        // Crear un nuevo objeto Client con los datos editados
        
            clientId,
            editDialog.getName().toStdString(),
            editDialog.getAge(),
            editDialog.getDNI().toUInt(),
            editDialog.getAddress().toStdString(),
            editDialog.getEmail().toStdString(),
            editDialog.getPhone().toStdString()
        );
        
        // Restaurar el balance original
        updatedClient.setBalance(originalBalance);
        
        
        std::cout<< updatedClient << std::endl;
        // Guardar el cliente actualizado en la base de datos
        database->updateClient(updatedClient);
        updateClientList();  // Refrescar la lista de clientes
    }
}

void ClientListDialog::onDeleteClientButtonClicked(){
    // Obtener el cliente seleccionado
    QListWidgetItem *selectedItem = ui->clientListWidget->currentItem();
    if (!selectedItem) {
        QMessageBox::warning(this, "Advertencia", "Por favor, selecciona un cliente para eliminar.");
        return;
    }
    
    // Obtener el nombre del cliente seleccionado y buscar su ID
    std::string clientName = selectedItem->text().toStdString();
    Client client = database->getClientByName(clientName);
    unsigned long long clientId = client.getId();

    QString clientNameQString = QString::fromStdString(clientName);
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmar eliminación",
                                  QString("¿Estás seguro de que deseas eliminar al cliente '%1'?").arg(clientNameQString),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        // Eliminar el cliente de la base de datos
        database->rmClient(clientId);
        
        // Refrescar la lista de clientes
        updateClientList();
    }
}
