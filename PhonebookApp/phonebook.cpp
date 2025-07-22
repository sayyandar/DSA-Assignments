#include "phonebook.h"
#include <QFile>
#include <QDataStream>
#include <QDebug>
#include <algorithm>
#include <mutex>

Phonebook::Phonebook(QObject* parent) : QObject(parent), root(nullptr) {
    loadFromFile();
}

Phonebook::~Phonebook() {
    if (modified) {
        saveToFile();
    }
    clearBST(root);
}

void Phonebook::addContact(const Contact& contact) {
    std::lock_guard<std::mutex> lock(mtx);
    contacts.push_back(contact);
    root = insertBST(root, contact);
    modified = true;
    emit contactsChanged();
}

QVector<Contact> Phonebook::getAllContacts() const {
    // std::lock_guard<std::mutex> lock(mtx);  // Added lock for thread safety
    std::vector<Contact> result;
    inOrderTraversal(root, result);
    // Changed to range constructor for better compatibility
    return QVector<Contact>(result.begin(), result.end());
}

Contact* Phonebook::searchByName(const QString& name) {
    std::lock_guard<std::mutex> lock(mtx);
    BSTNode* result = searchBST(root, name);
    return result ? &result->contact : nullptr;
}

QVector<Contact> Phonebook::searchByPhone(const QString& phone) const {
    // std::lock_guard<std::mutex> lock(mtx);
    QVector<Contact> result;
    for (const auto& contact : contacts) {
        if (contact.getPhone() == phone) {
            result.append(contact);
        }
    }
    return result;
}

bool Phonebook::deleteContact(const QString& name) {
    std::lock_guard<std::mutex> lock(mtx);
    auto it = std::remove_if(contacts.begin(), contacts.end(),
                             [&name](const Contact& c) { return c.getName() == name; });

    if (it != contacts.end()) {
        contacts.erase(it, contacts.end());
        clearBST(root);
        root = nullptr;
        for (const auto& contact : contacts) {
            root = insertBST(root, contact);
        }
        modified = true;
        emit contactsChanged();
        return true;
    }
    return false;
}

bool Phonebook::editContact(const QString& name, const Contact& newContact) {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& contact : contacts) {
        if (contact.getName() == name) {
            contact = newContact;
            clearBST(root);
            root = nullptr;
            for (const auto& c : contacts) {
                root = insertBST(root, c);
            }
            modified = true;
            emit contactsChanged();
            return true;
        }
    }
    return false;
}

void Phonebook::saveToFile() {
    std::lock_guard<std::mutex> lock(mtx);
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Error opening file for writing!";
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_15);  // Added version for compatibility
    for (const auto& contact : contacts) {
        out << contact.getName() << contact.getPhone()
        << contact.getEmail() << contact.getAddress();
        if (out.status() != QDataStream::Ok) {  // Added error checking
            qWarning() << "Error writing contact data";
            break;
        }
    }

    modified = false;
    qDebug() << "Phonebook saved to file.";
}

void Phonebook::loadFromFile() {
    std::lock_guard<std::mutex> lock(mtx);
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "No existing phonebook found. Starting fresh.";
        return;
    }

    contacts.clear();
    clearBST(root);
    root = nullptr;

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_15);  // Added version for compatibility
    while (!in.atEnd()) {
        QString name, phone, email, address;
        in >> name >> phone >> email >> address;
        if (in.status() != QDataStream::Ok) {  // Added error checking
            qWarning() << "Error reading contact data";
            break;
        }
        contacts.emplace_back(name, phone, email, address);
        root = insertBST(root, contacts.back());
    }

    qDebug() << "Phonebook loaded from file with" << contacts.size() << "contacts.";
    emit loadedFromFile();
}

BSTNode* Phonebook::insertBST(BSTNode* node, const Contact& contact) {
    if (node == nullptr) {
        return new BSTNode(contact);
    }

    if (contact.getName() < node->contact.getName()) {
        node->left = insertBST(node->left, contact);
    } else {
        node->right = insertBST(node->right, contact);
    }

    return node;
}

void Phonebook::inOrderTraversal(BSTNode* node, std::vector<Contact>& result) const {
    if (node == nullptr) return;
    inOrderTraversal(node->left, result);
    result.push_back(node->contact);
    inOrderTraversal(node->right, result);
}

BSTNode* Phonebook::searchBST(BSTNode* node, const QString& name) const {
    if (node == nullptr) return nullptr;

    if (name == node->contact.getName()) {
        return node;
    } else if (name < node->contact.getName()) {
        return searchBST(node->left, name);
    } else {
        return searchBST(node->right, name);
    }
}

void Phonebook::clearBST(BSTNode* node) {
    if (node == nullptr) return;
    clearBST(node->left);
    clearBST(node->right);
    delete node;
}
