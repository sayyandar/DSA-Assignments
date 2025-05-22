#include <iostream>
#include <vector>
#include <variant>
#include <string>
#include <limits> 
#include <stdexcept>  // For bad_variant_access in case 6
using namespace std;

class Node {
	public:
    variant<int, string> data;
    Node* next;
    
    Node(const variant<int, string>& val) : data(val), next(nullptr) {}
};

class ExpenseList {
    Node* head;
    Node* tail;
    
    void copyList(const Node* source) {
        while(source) {
            psuh_back_at_end(source->data);
            source = source->next;
        }
    }
    
public:
    ExpenseList() : head(nullptr), tail(nullptr) {}
    
    // Copy constructor
    ExpenseList(const ExpenseList& other) : head(nullptr), tail(nullptr) {
        copyList(other.head);
    }
    
    ExpenseList& operator=(const ExpenseList& other) {
        if(this != &other) {
            // Clear existing nodes
            while(head) {
                Node* temp = head;
                head = head->next;
                delete temp;
            }
            tail = nullptr;
            // Copy new nodes
            copyList(other.head);
        }
        return *this; //Enables chained assignments
    }
    
    ~ExpenseList() {
        while(head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    
    void psuh_back_at_end(const variant<int, string>& value) {
        Node* newNode = new Node(value);
        if(!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    void display() const {
        Node* current = head;
        string labels[] = {"ID", "Item", "Price", "Date"};
        int counter = 0;
        
        while(current && counter < 4) {
            cout << labels[counter] << ": ";
            visit([](auto&& arg){ cout << arg; }, current->data);
            cout << endl;
            current = current->next;
            counter++;
        }
        cout << "-------------------" << endl;
    }
    
    int getPrice() const {
        Node* current = head;
        // Move to 3rd node (price)
        for(int i = 0; i < 2 && current; i++) {
            current = current->next;
        }
        if(current && holds_alternative<int>(current->data)) {
            return get<int>(current->data);
        }
        return 0;
    }
    // edit expense
	void updateNode(int position, const variant<int, string>& newValue) {
	    Node* current = head;
	    for(int i = 0; current && i < position; i++) {
	        current = current->next;
	    }
	    if(current) {
	        current->data = newValue;
	    }
	}

};
		void clearInputBuffer() {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');// prevent input buffer
		}
		
		int getIntInput(const string& prompt) {
		    int value;
		    while(true) {
		        cout << prompt;
		        cin >> value;
		        if(cin.fail()) {
		            cout << "Invalid input. Please enter a number.\n";
		            clearInputBuffer();
		        } else {
		            clearInputBuffer();
		            return value;
		        }
		    }
		}
		
		string getStringInput(const string& prompt) {
		    string value;
		    cout << prompt;
		    getline(cin, value);
		    return value;
		}


int main() {
    vector<ExpenseList> expenses;
    
    cout << "EXPENSE TRACKING SYSTEM\n";
    int initialExpenses = getIntInput("Enter number of initial expenses to add: ");
    
    for(int i = 0; i < initialExpenses; i++) {
        ExpenseList newExpense;
        
        cout << "\nExpense #" << i+1 << endl;
        int id = getIntInput("Enter ID: ");
        string name = getStringInput("Enter item name: ");
        int price = getIntInput("Enter price: ");
        string date = getStringInput("Enter date (DD/MM/YYYY): ");
        
        newExpense.psuh_back_at_end(id);
        newExpense.psuh_back_at_end(name);
        newExpense.psuh_back_at_end(price);
        newExpense.psuh_back_at_end(date);
        
        expenses.push_back(newExpense);
    }
    
    while(true) {
        cout << "\nMENU:\n";
		cout << "1. Add new expense\n";
		cout << "2. View all expenses\n";
		cout << "3. Generate report\n";
		cout << "4. View single expense\n";
		cout << "5. Delete expense\n";
		cout << "6. Edit expense\n";
		cout << "7. Exit\n";

        
        int choice = getIntInput("Choice: ");
        
        if(choice == 7) break;
        
        switch(choice) {
           case 1: {
                ExpenseList newExpense;//ExpenseList newExpense same as line 146
                
                int id = getIntInput("Enter ID: ");
                string name = getStringInput("Enter item name: ");
                int price = getIntInput("Enter price: ");
                string date = getStringInput("Enter date (DD/MM/YYYY): ");
                
                newExpense.psuh_back_at_end(id);
                newExpense.psuh_back_at_end(name);
                newExpense.psuh_back_at_end(price);
                newExpense.psuh_back_at_end(date);
                
                expenses.push_back(newExpense);
                cout << "Expense added!\n";
                break;
            }
            case 2:
                if(expenses.empty()) {
                    cout << "\nNo expenses to display!\n";
                } else {
                    cout << "\nALL EXPENSES:\n";
                    for(size_t i = 0; i < expenses.size(); i++) { // use (size_t) bcz size might increase
                        cout << "Expense #" << i+1 << endl;
                        expenses[i].display();
                    }
                }
                break;
            case 3: {
                if(expenses.empty()) {
                    cout << "\nNo expenses to report!\n";
                } else {
                    int total = 0;
                    for(const auto& expense : expenses) {
                        total += expense.getPrice();
                    }
                    cout << "\nTOTAL EXPENDITURE REPORT\n";
                    cout << "Total expenses: " << expenses.size() << endl;
                    cout << "Total amount spent: Rs" << total << endl;
                }
                break;
        }
            case 4: {
                if(expenses.empty()) {
                    cout << "\nNo expenses to display!\n";
                    break;
                }
                
                int index = getIntInput("Enter expense number to view: ");
                if(index < 1 || index > expenses.size()) {
                    cout << "Invalid expense number!\n";
                    break;
                }
                
                cout << "\nEXPENSE #" << index << endl;
                expenses[index-1].display();
                break;
            }
            
            case 5: {
                if(expenses.empty()) {
                    cout << "\nNo expenses to delete!\n";
                    break;
                }
                
                int index = getIntInput("Enter expense number to delete: ");
                if(index < 1 || index > expenses.size()) {
                    cout << "Invalid expense number!\n";
                    break;
                }
                
                expenses.erase(expenses.begin() + index - 1);
                cout << "Expense #" << index << " deleted!\n";
                break;
            }
            case 6: {
			    if(expenses.empty()) {
			        cout << "\nNo expenses to edit!\n";
			        break;
			    }
			    
			    int index = getIntInput("Enter expense number to edit: ");
			    if(index < 1 || index > expenses.size()) {
			        cout << "Invalid expense number!\n";
			        break;
			    }
			    
			    ExpenseList& expense = expenses[index-1];
			    cout << "Current details:\n";
			    expense.display();
			    
			    cout << "Select field to edit:\n";
			    cout << "1. ID\n2. Item Name\n3. Price\n4. Date\n";
			    int field = getIntInput("Choice: ");
			    
			    if(field < 1 || field > 4) {
			        cout << "Invalid field selection!\n";
			        break;
			    }
			    
			    try {
			        switch(field) {
			            case 1: {
			                int newId = getIntInput("Enter new ID: ");
			                expense.updateNode(0, newId);
			                break;
			            }
			            case 2: {
			                string newName = getStringInput("Enter new item name: ");
			                expense.updateNode(1, newName);
			                break;
			            }
			            case 3: {
			                int newPrice = getIntInput("Enter new price: ");
			                expense.updateNode(2, newPrice);
			                break;
			            }
			            case 4: {
			                string newDate = getStringInput("Enter new date (DD/MM/YYYY): ");
			                expense.updateNode(3, newDate);
			                break;
			            }
			        }
			        cout << "Expense updated successfully!\n";
			    } catch(const bad_variant_access&) {
			        cout << "Error: Invalid data type for this field!\n";
			    }
			    break;
			}
			
            default:
                cout << "Invalid choice! Please select 1-7.\n";
        }
    }
    
    cout << "Goodbye!";
    return 0;
}