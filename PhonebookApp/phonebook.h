#ifndef PHONEBOOK_H
#define PHONEBOOK_H

#include <QObject>
#include <QVector>
#include <vector>
#include <QString>
#include <mutex>

class Contact {
public:
    Contact(const QString& name = "", const QString& phone = "",
            const QString& email = "", const QString& address = "");

    // Getters
    QString getName() const;
    QString getPhone() const;
    QString getEmail() const;
    QString getAddress() const;

    // Setters
    void setName(const QString& name);
    void setPhone(const QString& phone);
    void setEmail(const QString& email);
    void setAddress(const QString& address);

    bool operator<(const Contact& other) const;

private:
    QString name;
    QString phone;
    QString email;
    QString address;
};

class BSTNode {
public:
    Contact contact;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Contact& c) : contact(c), left(nullptr), right(nullptr) {}
};

class Phonebook : public QObject {
    Q_OBJECT

public:
    explicit Phonebook(QObject* parent = nullptr);
    ~Phonebook();

    void addContact(const Contact& contact);
    QVector<Contact> getAllContacts() const;
    Contact* searchByName(const QString& name);
    QVector<Contact> searchByPhone(const QString& phone) const;
    bool deleteContact(const QString& name);
    bool editContact(const QString& name, const Contact& newContact);
    void saveToFile();
    void loadFromFile();

signals:
    void contactsChanged();
    void loadedFromFile();

private:
    std::vector<Contact> contacts;
    BSTNode* root;
    QString filename = "phonebook.dat";
    bool modified = false;
    std::mutex mtx;

    // BST operations
    BSTNode* insertBST(BSTNode* node, const Contact& contact);
    void inOrderTraversal(BSTNode* node, std::vector<Contact>& result) const;
    BSTNode* searchBST(BSTNode* node, const QString& name) const;
    void clearBST(BSTNode* node);
};

#endif // PHONEBOOK_H
