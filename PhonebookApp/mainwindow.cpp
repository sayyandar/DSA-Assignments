#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Configure the table
    ui->contactsTable->setColumnCount(4);
    ui->contactsTable->setHorizontalHeaderLabels({"Name", "Phone", "Email", "Address"});
    ui->contactsTable->horizontalHeader()->setStretchLastSection(true);

    // Connect signals
    connect(&phonebook, &Phonebook::contactsChanged, this, &MainWindow::updateContactsTable);
    connect(&phonebook, &Phonebook::loadedFromFile, this, &MainWindow::updateContactsTable);

    // Initial update
    updateContactsTable();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_addButton_clicked() {
    Contact contact(
        ui->nameLineEdit->text(),
        ui->phoneLineEdit->text(),
        ui->emailLineEdit->text(),
        ui->addressLineEdit->text()
        );

    if (contact.getName().isEmpty()) {
        QMessageBox::warning(this, "Error", "Name cannot be empty!");
        return;
    }

    phonebook.addContact(contact);
    clearInputFields();
    ui->statusLabel->setText("Contact added successfully");
}

void MainWindow::on_searchButton_clicked() {
    QString searchTerm = QInputDialog::getText(this, "Search Contact",
                                               "Enter name or phone to search:");
    if (searchTerm.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a search term!");
        return;
    }

    // Try searching by name first
    Contact* contact = phonebook.searchByName(searchTerm);
    if (contact) {
        showContactDetails(*contact);
        return;
    }

    // If not found by name, try by phone
    QVector<Contact> contacts = phonebook.searchByPhone(searchTerm);
    if (!contacts.isEmpty()) {
        QStringList contactStrings;
        for (const auto& contact : contacts) {
            contactStrings << contact.getName() + " - " + contact.getPhone();
        }
        QMessageBox::information(this, "Search Results", contactStrings.join("\n"));
    } else {
        QMessageBox::information(this, "Not Found", "Contact not found.");
    }
}

void MainWindow::on_deleteButton_clicked() {
    QString name = QInputDialog::getText(this, "Delete Contact", "Enter name to delete:");
    if (!name.isEmpty()) {
        if (phonebook.deleteContact(name)) {
            ui->statusLabel->setText("Contact deleted successfully");
        } else {
            QMessageBox::warning(this, "Error", "Contact not found.");
        }
    }
}

void MainWindow::on_editButton_clicked() {
    QString oldName = QInputDialog::getText(this, "Edit Contact", "Enter name of contact to edit:");
    if (oldName.isEmpty()) return;

    Contact* oldContact = phonebook.searchByName(oldName);
    if (!oldContact) {
        QMessageBox::warning(this, "Error", "Contact not found.");
        return;
    }

    bool ok;
    QString newName = QInputDialog::getText(this, "Edit Contact", "New name:",
                                            QLineEdit::Normal, oldContact->getName(), &ok);
    if (!ok) return;

    QString newPhone = QInputDialog::getText(this, "Edit Contact", "New phone:",
                                             QLineEdit::Normal, oldContact->getPhone(), &ok);
    if (!ok) return;

    QString newEmail = QInputDialog::getText(this, "Edit Contact", "New email:",
                                             QLineEdit::Normal, oldContact->getEmail(), &ok);
    if (!ok) return;

    QString newAddress = QInputDialog::getText(this, "Edit Contact", "New address:",
                                               QLineEdit::Normal, oldContact->getAddress(), &ok);
    if (!ok) return;

    Contact newContact(newName, newPhone, newEmail, newAddress);
    if (phonebook.editContact(oldName, newContact)) {
        ui->statusLabel->setText("Contact updated successfully");
    } else {
        QMessageBox::warning(this, "Error", "Failed to update contact.");
    }
}

void MainWindow::on_saveButton_clicked() {
    phonebook.saveToFile();
    ui->statusLabel->setText("Phonebook saved to file");
}

void MainWindow::on_actionSave_triggered() {
    on_saveButton_clicked();
}

void MainWindow::on_actionLoad_triggered() {
    phonebook.loadFromFile();
    updateContactsTable();
    ui->statusLabel->setText("Phonebook loaded from file");
}

void MainWindow::on_actionExit_triggered() {
    this->close();
}

void MainWindow::on_actionAbout_triggered() {
    QMessageBox::about(this, "About Phonebook",
    "Phonebook Application\nVersion 1.0\nA simple contact management system\nFree Version.\nContact Sayyandar47@gmail.com to get the updated version with features like export to PDF");
}

void MainWindow::updateContactsTable() {
    QVector<Contact> contacts = phonebook.getAllContacts();
    ui->contactsTable->setRowCount(contacts.size());

    for (int i = 0; i < contacts.size(); ++i) {
        const Contact& contact = contacts[i];
        ui->contactsTable->setItem(i, 0, new QTableWidgetItem(contact.getName()));
        ui->contactsTable->setItem(i, 1, new QTableWidgetItem(contact.getPhone()));
        ui->contactsTable->setItem(i, 2, new QTableWidgetItem(contact.getEmail()));
        ui->contactsTable->setItem(i, 3, new QTableWidgetItem(contact.getAddress()));
    }

    ui->contactsTable->resizeColumnsToContents();
}

void MainWindow::clearInputFields() {
    ui->nameLineEdit->clear();
    ui->phoneLineEdit->clear();
    ui->emailLineEdit->clear();
    ui->addressLineEdit->clear();
}

void MainWindow::showContactDetails(const Contact& contact) {
    QMessageBox::information(this, "Contact Details",
                             QString("Name: %1\nPhone: %2\nEmail: %3\nAddress: %4")
                                 .arg(contact.getName())
                                 .arg(contact.getPhone())
                                 .arg(contact.getEmail())
                                 .arg(contact.getAddress()));
}
