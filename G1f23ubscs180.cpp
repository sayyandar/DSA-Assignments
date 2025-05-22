/*
	G1F23UBSCS180
	AHMED
*/
#include <iostream>
using namespace std;

struct Node {
    string data;
    Node* next;

    Node(string d) {
        data = d;
        next = nullptr;
    }
};

struct Expense {
    Node* head;
    Expense* next;

    Expense() {
        head = nullptr;
        next = nullptr;
    }
};

class ExpenseTracker {
private:
    Expense* head;

public:
    ExpenseTracker() {
        head = nullptr;
    }

    void addExpense() {
        string id, name, price, date;

        cout << "Enter Expense ID: ";
        cin >> id;
        cout << "Enter Product Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Price: ";
        cin >> price;
        cout << "ADD Date : ";
        cin >> date;

        Expense* newExpense = new Expense();
        newExpense->head = new Node(id);
        newExpense->head->next = new Node(name);
        newExpense->head->next->next = new Node(price);
        newExpense->head->next->next->next = new Node(date);

        if (head == nullptr) {
            head = newExpense;
        } else {
            Expense* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newExpense;
        }

        cout << "Expense added successfully.\n";
    }

    void viewExpenses() {
        if (head == nullptr) {
            cout << "No expenses to display.\n";
            return;
        }

        Expense* temp = head;
        int count = 1;

        while (temp != nullptr) {
            Node* node = temp->head;
            cout << "\nExpense #" << count++ << endl;
            cout << "ID: " << node->data << endl;
            node = node->next;
            cout << "Name: " << node->data << endl;
            node = node->next;
            cout << "Price: " << node->data << endl;
            node = node->next;
            cout << "Date: " << node->data << endl;
            temp = temp->next;
        }
    }

    void showExpenseById(string searchId) {
        Expense* temp = head;
        while (temp != nullptr) {
            if (temp->head->data == searchId) {
                Node* node = temp->head;
                cout << "\nExpense Details:\n";
                cout << "ID: " << node->data << endl;
                node = node->next;
                cout << "Name: " << node->data << endl;
                node = node->next;
                cout << "Price: " << node->data << endl;
                node = node->next;
                cout << "Date: " << node->data << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "Expense with ID " << searchId << " not found.\n";
    }

    void deleteExpenseById(string searchId) {
        Expense* temp = head;
        Expense* prev = nullptr;

        while (temp != nullptr) {
            if (temp->head->data == searchId) {
                if (prev == nullptr) {
                    head = temp->next;
                } else {
                    prev->next = temp->next;
                }

                deleteNodes(temp->head);
                delete temp;

                cout << "Expense deleted successfully.\n";
                return;
            }
            prev = temp;
            temp = temp->next;
        }
        cout << "Expense with ID " << searchId << " not found.\n";
    }

    void editExpenseById(string searchId) {
        Expense* temp = head;

        while (temp != nullptr) {
            if (temp->head->data == searchId) {
                string name, price, date;

                cout << "Enter new Product Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter new Price: ";
                cin >> price;
                cout << "Enter new Date (YYYY-MM-DD): ";
                cin >> date;

                Node* node = temp->head->next;
                node->data = name;
                node = node->next;
                node->data = price;
                node = node->next;
                node->data = date;

                cout << "Expense updated successfully.\n";
                return;
            }
            temp = temp->next;
        }
        cout << "Expense with ID " << searchId << " not found.\n";
    }

    void generateReport() {
        Expense* temp = head;
        float total = 0;

        while (temp != nullptr) {
            Node* node = temp->head->next->next; // price
            total += stof(node->data);
            temp = temp->next;
        }

        cout << "\n----- Expense Report -----\n";
        cout << "Total Expenses Amount: " << total << endl;
    }

    void deleteNodes(Node* node) {
        while (node != nullptr) {
            Node* next = node->next;
            delete node;
            node = next;
        }
    }

    ~ExpenseTracker() {
        while (head != nullptr) {
            Expense* next = head->next;
            deleteNodes(head->head);
            delete head;
            head = next;
        }
    }
};

int main() {
    ExpenseTracker tracker;
    int choice;
    string id;

    while (true) {
        cout << "\n\nExpense Tracking Menu:\n";
        cout << "1. Add Expense\n";
        cout << "2. View All Expenses\n";
        cout << "3. Show Expense by ID\n";
        cout << "4. Edit Expense by ID\n";
        cout << "5. Delete Expense by ID\n";
        cout << "6. Generate Report\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                tracker.addExpense();
                break;
            case 2:
                tracker.viewExpenses();
                break;
            case 3:
                cout << "Enter Expense ID to show: ";
                cin >> id;
                tracker.showExpenseById(id);
                break;
            case 4:
                cout << "Enter Expense ID to edit: ";
                cin >> id;
                tracker.editExpenseById(id);
                break;
            case 5:
                cout << "Enter Expense ID to delete: ";
                cin >> id;
                tracker.deleteExpenseById(id);
                break;
            case 6:
                tracker.generateReport();
                break;
            case 7:
                cout << "Exiting program.\n";
                return 0;
            default:
                cout << "Invalid choice. Try again.\n";
        }
    }
}