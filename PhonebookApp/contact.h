#ifndef CONTACT_H
#define CONTACT_H

#include <QString>
#include <QObject>

class Contact {
public:
    Contact(const QString& n = "", const QString& p = "",
            const QString& e = "", const QString& a = "");

    // Getters
    QString getName() const;
    QString getPhone() const;
    QString getEmail() const;
    QString getAddress() const;

    // Setters
    void setName(const QString& n);
    void setPhone(const QString& p);
    void setEmail(const QString& e);
    void setAddress(const QString& a);

    QString display() const;

    bool operator<(const Contact& other) const;

private:
    QString name;
    QString phone;
    QString email;
    QString address;
};

#endif // CONTACT_H
