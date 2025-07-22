#include "contact.h"

Contact::Contact(const QString& n, const QString& p, const QString& e, const QString& a)
    : name(n), phone(p), email(e), address(a) {}

// Getters
QString Contact::getName() const { return name; }
QString Contact::getPhone() const { return phone; }
QString Contact::getEmail() const { return email; }
QString Contact::getAddress() const { return address; }

// Setters
void Contact::setName(const QString& n) { name = n; }
void Contact::setPhone(const QString& p) { phone = p; }
void Contact::setEmail(const QString& e) { email = e; }
void Contact::setAddress(const QString& a) { address = a; }

QString Contact::display() const {
    return QString("Name: %1\nPhone: %2\nEmail: %3\nAddress: %4\n")
    .arg(name).arg(phone).arg(email).arg(address);
}

bool Contact::operator<(const Contact& other) const {
    return name < other.name;
}
