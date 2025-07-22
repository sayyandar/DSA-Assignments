#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>
#include <ctime>
#include <limits>

using namespace std;

mutex mtx; // For thread safety

// Contact class to store individual contact information
class Contact {
private:
    string name;
    string phone;
    string email;
    string address;

public:
    Contact(const string& n, const string& p, const string& e, const string& a)
        : name(n), phone(p), email(e), address(a) {}

    // Getters
    string getName() const { return name; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    string getAddress() const { return address; }

    // Setters
    void setName(const string& n) { name = n; }
    void setPhone(const string& p) { phone = p; }
    void setEmail(const string& e) { email = e; }
    void setAddress(const string& a) { address = a; }

    void display() const {
        cout << "Name: " << name << "\nPhone: " << phone 
             << "\nEmail: " << email << "\nAddress: " << address << "\n\n";
    }

    // For sorting
    bool operator<(const Contact& other) const {
        return name < other.name;
    }
};

// Binary Search Tree Node for contacts
class BSTNode {
public:
    Contact contact;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Contact c) : contact(c), left(nullptr), right(nullptr) {}
};

// Phonebook class to manage all contacts
class Phonebook {
private:
    vector<Contact> contacts;
    BSTNode* root = nullptr;
    string filename = "phonebook.dat";
    bool modified = false;

    // BST operations
    BSTNode* insertBST(BSTNode* node, Contact contact) {
        if (node == nullptr) return new BSTNode(contact);
        
        if (contact.getName() < node->contact.getName())
            node->left = insertBST(node->left, contact);
        else
            node->right = insertBST(node->right, contact);
            
        return node;
    }

    void inOrderTraversal(BSTNode* node, vector<Contact>& result) const {
        if (node == nullptr) return;
        inOrderTraversal(node->left, result);
        result.push_back(node->contact);
        inOrderTraversal(node->right, result);
    }

    BSTNode* searchBST(BSTNode* node, const string& name) {
        if (node == nullptr) return nullptr;
        
        if (name == node->contact.getName())
            return node;
        else if (name < node->contact.getName())
            return searchBST(node->left, name);
        else
            return searchBST(node->right, name);
    }

    void clearBST(BSTNode* node) {
        if (node == nullptr) return;
        clearBST(node->left);
        clearBST(node->right);
        delete node;
    }

    // File operations
    void saveToFile() {
        lock_guard<mutex> lock(mtx);
        ofstream outFile(filename, ios::binary);
        if (!outFile) {
            cerr << "Error opening file for writing!\n";
            return;
        }

        for (const auto& contact : contacts) {
            size_t size;
            
            // Write name
            size = contact.getName().size();
            outFile.write(reinterpret_cast<char*>(&size), sizeof(size));
            outFile.write(contact.getName().c_str(), size);
            
            // Write phone
            size = contact.getPhone().size();
            outFile.write(reinterpret_cast<char*>(&size), sizeof(size));
            outFile.write(contact.getPhone().c_str(), size);
            
            // Write email
            size = contact.getEmail().size();
            outFile.write(reinterpret_cast<char*>(&size), sizeof(size));
            outFile.write(contact.getEmail().c_str(), size);
            
            // Write address
            size = contact.getAddress().size();
            outFile.write(reinterpret_cast<char*>(&size), sizeof(size));
            outFile.write(contact.getAddress().c_str(), size);
        }
        
        modified = false;
        cout << "Phonebook saved to file.\n";
    }

    void loadFromFile() {
        lock_guard<mutex> lock(mtx);
        ifstream inFile(filename, ios::binary);
        if (!inFile) {
            cout << "No existing phonebook found. Starting fresh.\n";
            return;
        }

        contacts.clear();
        clearBST(root);
        root = nullptr;

        while (true) {
            size_t size;
            string name, phone, email, address;
            
            // Read name
            if (!inFile.read(reinterpret_cast<char*>(&size), sizeof(size))) break;
            name.resize(size);
            if (!inFile.read(&name[0], size)) break;
            
            // Read phone
            inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
            phone.resize(size);
            inFile.read(&phone[0], size);
            
            // Read email
            inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
            email.resize(size);
            inFile.read(&email[0], size);
            
            // Read address
            inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
            address.resize(size);
            inFile.read(&address[0], size);
            
            contacts.emplace_back(name, phone, email, address);
            root = insertBST(root, contacts.back());
        }
        
        cout << "Phonebook loaded from file with " << contacts.size() << " contacts.\n";
    }

    // Auto-save thread function
    void autoSave() {
        while (true) {
            this_thread::sleep_for(chrono::minutes(5)); // Auto-save every 5 minutes
            if (modified) {
                saveToFile();
            }
        }
    }

public:
    Phonebook() {
        loadFromFile();
        // Start auto-save thread
        thread autoSaveThread(&Phonebook::autoSave, this);
        autoSaveThread.detach();
    }

    ~Phonebook() {
        if (modified) {
            saveToFile();
        }
        clearBST(root);
    }

    void addContact(const Contact& contact) {
        lock_guard<mutex> lock(mtx);
        contacts.push_back(contact);
        root = insertBST(root, contact);
        modified = true;
        cout << "Contact added successfully.\n";
    }

    void displayAll() const {
        if (contacts.empty()) {
            cout << "Phonebook is empty.\n";
            return;
        }
        
        vector<Contact> sortedContacts;
        inOrderTraversal(root, sortedContacts);
        
        cout << "\n--- All Contacts (Sorted by Name) ---\n";
        for (const auto& contact : sortedContacts) {
            contact.display();
        }
    }

    void searchByName(const string& name) {
        BSTNode* result = searchBST(root, name);
        if (result) {
            cout << "\n--- Contact Found ---\n";
            result->contact.display();
        } else {
            cout << "Contact not found.\n";
        }
    }

    void searchByPhone(const string& phone) {
        bool found = false;
        for (const auto& contact : contacts) {
            if (contact.getPhone() == phone) {
                if (!found) {
                    cout << "\n--- Contact Found ---\n";
                    found = true;
                }
                contact.display();
            }
        }
        if (!found) {
            cout << "Contact not found.\n";
        }
    }

    void deleteContact(const string& name) {
        lock_guard<mutex> lock(mtx);
        auto it = remove_if(contacts.begin(), contacts.end(), 
            [&name](const Contact& c) { return c.getName() == name; });
        
        if (it != contacts.end()) {
            contacts.erase(it, contacts.end());
            // Rebuild BST
            clearBST(root);
            root = nullptr;
            for (const auto& contact : contacts) {
                root = insertBST(root, contact);
            }
            modified = true;
            cout << "Contact deleted successfully.\n";
        } else {
            cout << "Contact not found.\n";
        }
    }

    void editContact(const string& name) {
        lock_guard<mutex> lock(mtx);
        for (auto& contact : contacts) {
            if (contact.getName() == name) {
                cout << "Editing contact:\n";
                contact.display();
                
                string input;
                cout << "Enter new phone (or press enter to keep current): ";
                getline(cin, input);
                if (!input.empty()) contact.setPhone(input);
                
                cout << "Enter new email (or press enter to keep current): ";
                getline(cin, input);
                if (!input.empty()) contact.setEmail(input);
                
                cout << "Enter new address (or press enter to keep current): ";
                getline(cin, input);
                if (!input.empty()) contact.setAddress(input);
                
                // Rebuild BST as the name might have changed
                clearBST(root);
                root = nullptr;
                for (const auto& c : contacts) {
                    root = insertBST(root, c);
                }
                
                modified = true;
                cout << "Contact updated successfully.\n";
                return;
            }
        }
        cout << "Contact not found.\n";
    }

    void saveManual() {
        saveToFile();
    }
};

// User interface functions
void displayMenu() {
    cout << "\n=== Phonebook Application ===\n";
    cout << "1. Add Contact\n";
    cout << "2. Display All Contacts\n";
    cout << "3. Search by Name\n";
    cout << "4. Search by Phone\n";
    cout << "5. Delete Contact\n";
    cout << "6. Edit Contact\n";
    cout << "7. Save to File\n";
    cout << "8. Exit\n";
    cout << "Enter your choice: ";
}

Contact getContactInfo() {
    string name, phone, email, address;
    
    cout << "Enter name: ";
    getline(cin, name);
    
    cout << "Enter phone: ";
    getline(cin, phone);
    
    cout << "Enter email: ";
    getline(cin, email);
    
    cout << "Enter address: ";
    getline(cin, address);
    
    return Contact(name, phone, email, address);
}

int main() {
    Phonebook phonebook;
    int choice;
    string input;

    while (true) {
        displayMenu();
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        switch (choice) {
            case 1:
                phonebook.addContact(getContactInfo());
                break;
            case 2:
                phonebook.displayAll();
                break;
            case 3:
                cout << "Enter name to search: ";
                getline(cin, input);
                phonebook.searchByName(input);
                break;
            case 4:
                cout << "Enter phone to search: ";
                getline(cin, input);
                phonebook.searchByPhone(input);
                break;
            case 5:
                cout << "Enter name to delete: ";
                getline(cin, input);
                phonebook.deleteContact(input);
                break;
            case 6:
                cout << "Enter name to edit: ";
                getline(cin, input);
                phonebook.editContact(input);
                break;
            case 7:
                phonebook.saveManual();
                break;
            case 8:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

/*
Key Features Implemented
	
Binary Search Tree (BST): Used for efficient searching and sorting of contacts by name

Multithreading: Auto-save feature runs in a separate thread

File Handling: Contacts are saved to a binary file

OOP Principles: Proper encapsulation with Contact and Phonebook classes

Mutex Locks: For thread-safe operations on shared data

Possible Enhancements

Add contact import/export functionality

This implementation provides a solid foundation for your DSA project while
demonstrating key concepts like BST, multithreading, and file handling.
*/