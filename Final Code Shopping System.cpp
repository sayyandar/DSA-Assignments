#include <iostream> // mainlt for cout ststements
#include <fstream> // for file handling
#include <string> // for string commands
#include <vector> // for vector utilization
#include <ctime>  // for time display
#include <iomanip> // For std::setw and std::setfill


using namespace std;

// Product class
class Product {
private:
    string id;
    string name;
    double price;
    int quantity;

public:
    Product(string id, string name, double price, int quantity)
        : id(id), name(name), price(price), quantity(quantity) {}

    string getId() const { return id; }
    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void updateQuantity(int qty) { quantity -= qty; }
};

// CartItem class to hold product and quantity
class CartItem {
private:
    Product product;
    int quantity;

public:
    CartItem(const Product& product, int quantity)
        : product(product), quantity(quantity) {}

    Product getProduct() const { return product; }
    int getQuantity() const { return quantity; }
};

// ShoppingCart class
class ShoppingCart {
private:
    vector<CartItem> items;

public:
    void addProduct(const Product& product, int qty) {
        items.emplace_back(product, qty);
    }

    void showCart() const {
        cout << "\nYour Shopping Cart:\n";
        for (const auto& item : items) {
            const Product& product = item.getProduct();
            cout << "Product ID: " << product.getId() << ", Name: " << product.getName()
                 << ", Price: " << product.getPrice() << ", Quantity: " << item.getQuantity() << endl;
        }
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : items) {
            total += item.getProduct().getPrice() * item.getQuantity();
        }
        return total;
    }

    const vector<CartItem>& getItems() const { return items; } // Added for access to items
};

// Customer class
class Customer {
private:
    string id;
    string name;

public:
    Customer(string id, string name) : id(id), name(name) {}

    string getId() const { return id; }
    string getName() const { return name; }
};

// Payment class (abstract)
class Payment {
public:
    virtual void processPayment(double amount) = 0; // Pure virtual function
};

// CreditCardPayment class
class CreditCardPayment : public Payment {
public:
    void processPayment(double amount) override {
        cout << "Processing credit card payment of : " << amount << " Rs" << endl;
    }
};

// PayPalPayment class
class PayPalPayment : public Payment {
public:
    void processPayment(double amount) override {
        cout << "Processing PayPal payment of Rs " << amount << endl;
    }
};

// Shop class
class Shop {
private:
    vector<Product> inventory;

public:
    void loadProducts() {
        ifstream file("products.txt");
        if (!file.is_open()) {
            cout << "Error opening products file!" << endl;
            return;
        }

        string id, name;
        double price;
        int quantity;
        while (file >> id >> name >> price >> quantity) {
            inventory.emplace_back(id, name, price, quantity);
        }
        file.close();
    }

    void saveProducts() {
        ofstream file("products.txt");
        if (!file.is_open()) {
            cout << "Error opening products file for writing!" << endl;
            return;
        }

        for (const auto& product : inventory) {
            file << product.getId() << " " << product.getName() << " "
                 << product.getPrice() << " " << product.getQuantity() << endl;
        }
        file.close();
    }

    void showProducts() const {
        cout << "\nAvailable Products:\n";
        for (const auto& product : inventory) {
            cout << "Product ID: " << product.getId() << ", Name: " << product.getName()
                 << ", Price: " << product.getPrice() << ", Quantity: " << product.getQuantity() << endl;
        }
    }

    Product* findProduct(const string& id) {
        for (auto& product : inventory) {
            if (product.getId() == id) {
                return &product;
            }
        }
        return nullptr;
    }

    void addProduct(const Product& product) {
        inventory.push_back(product);
        cout << "Product added: " << product.getName() << endl;
    }
};

// Customer Management
class CustomerManager {
private:
    vector<Customer> customers;

public:
    void loadCustomers() {
        ifstream file("customers.txt");
        if (!file.is_open()) {
            cout << "Error opening customers file!" << endl;
            return;
        }

        string id, name;
        while (file >> id >> name) {
            customers.emplace_back(id, name);
        }
        file.close();
    }

    void saveCustomers() {
        ofstream file("customers.txt");
        if (!file.is_open()) {
            cout << "Error opening customers file for writing!" << endl;
            return;
        }

        for (const auto& customer : customers) {
            file << customer.getId() << " " << customer.getName() << endl;
        }
        file.close();
    }

    void showCustomers() const {
        cout << "\nRegistered Customers:\n";
        for (const auto& customer : customers) {
            cout << "Customer ID: " << customer.getId() << ", Name: " << customer.getName() << endl;
        }
    }

    void addCustomer(const string& id, const string& name) {
        if (id.empty() || name.empty()) {
            cout << "Customer ID and Name cannot be empty!" << endl;
            return; // Exit the method if input is invalid
        }
        customers.emplace_back(id, name);
        cout << "Customer added: " << name << endl;
    }
    
        // Method to find a customer by ID
    Customer* findCustomer(const string& id) {
        for (auto& customer : customers) {
            if (customer.getId() == id) {
                return &customer; // Return pointer to the found customer
            }
        }
        return nullptr; // Return nullptr if customer not found
    }
};

// Order class
class Order {
private:
    ShoppingCart cart;
    int bno;
    string customerId;
    string customerName;
    Payment* paymentMethod; // Pointer to payment method

public:
	static int n;
    Order(const ShoppingCart& cart, const string& customerId, const string& customerName, Payment* paymentMethod)
        : cart(cart), customerId(customerId), customerName(customerName), paymentMethod(paymentMethod) {}

    void processOrder() {
    	bno=++n;
    	cout<<"Bill no :"<<bno<<endl;
        double total = cart.calculateTotal();
        cout << "Processing order for Customer ID: " << customerId << " (" << customerName << ") with total amount: Rs " << total << endl;
        if (paymentMethod) {
            paymentMethod->processPayment(total); // Process payment
        }
    }
};
int Order::n=0;

// Manager class
class Manager {
private:
    string id;
    string password;

public:
    Manager(string id, string password) : id(id), password(password) {}

    string getId() const { return id; }

    bool validatePassword(const string& inputPassword) const {
        return password == inputPassword;
    }
};

// InventoryManager class
class InventoryManager {
private:
    Shop& shop;

public:
    InventoryManager(Shop& shop) : shop(shop) {}

    void searchProduct(const string& id) {
        Product* product = shop.findProduct(id);
        if (product) {
            cout << "Product found: ID: " << product->getId() << ", Name: " << product->getName()
                 << ", Price: " << product->getPrice() << ", Quantity: " << product->getQuantity() << endl;
        } else {
            cout << "Product not found!" << endl;
        }
    }
};
// time display
class DateTime {
private:
    int assigned_dd;
    int assigned_mm;
    int assigned_yy;
    bool isAssigned; // Flag to check if a date is assigned

public:
    DateTime() : isAssigned(false) {} // Constructor initializes isAssigned to false

    // Method to display the current date
    void displayCurrentDate() const {
        time_t timestamp = time(nullptr);
        struct tm datetime = *localtime(&timestamp);

        cout << "Current Date: "
             << datetime.tm_mday << "/"
             << (datetime.tm_mon + 1) << "/" // Months are 0-11
             << (datetime.tm_year + 1900) << endl; // Years since 1900
    }

    // Method to display the current time
    void displayCurrentTime() const {
        time_t timestamp = time(nullptr);
        struct tm datetime = *localtime(&timestamp);

        cout << "Current Time: "
             << setw(2) << setfill('0') << datetime.tm_hour << ":"
             << setw(2) << setfill('0') << datetime.tm_min << ":"
             << setw(2) << setfill('0') << datetime.tm_sec << endl; // Second
    }

    // Method to display the current day of the week
    void displayCurrentDay() const {
        time_t timestamp = time(nullptr);
        struct tm datetime = *localtime(&timestamp);

        const char* days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        cout << "Current Day: " << days[datetime.tm_wday] << endl; // tm_wday gives the day of the week (0-6)
    }

    // Method to display the full date and time
    void displayDateTime() const {
        displayCurrentDate();
        displayCurrentTime();
        displayCurrentDay();
    }
};


// ShoppingSystem class
class ShoppingSystem {
private:
    Shop shop;
    ShoppingCart cart;
    CustomerManager customerManager;
    InventoryManager inventoryManager;
    Manager manager; // Add manager instance
    DateTime dt; // Create an instance of DateTime

public:
    ShoppingSystem() : inventoryManager(shop), manager("sammy", "open") {}
    
    void searchCustomerById() {
    string customerId;
    cout << "Enter Customer ID to search: ";
    cin >> customerId;

    Customer* customer = customerManager.findCustomer(customerId);
    	if (customer) {
        	cout << "Customer found: ID: " << customer->getId() << ", Name: " << customer->getName() << endl;
    	} else {
        	cout << "Customer not found!" << endl;
    	}
	}

    void getCustomerDetails(string& customerId, string& customerName) {
        cout << "Enter Customer ID: ";
        cin >> customerId;
        cout << "Enter Customer Name: ";
        cin >> customerName;
    }

    void run() {
        // Manager login
        string inputId, inputPassword;
        cout << "\t\t\t\t---------------------------------------------" << endl;
	    cout << "\t\t\t\t\t   WE VALUE YOUR EXPERIENCE" << endl;
	    cout << "\t\t\t\t---------------------------------------------" << endl;
	    cout << "\t\t\t\t\t    Enter Manager ID: "; cin >> inputId;
	    cout << "\t\t\t\t\t Enter Manager Password: "; cin >> inputPassword;

        // Validate manager credentials
        if (manager.getId() == inputId && manager.validatePassword(inputPassword)) {
            cout << "Manager login successful!" << endl;
            

            // Proceed with the shopping system operations
            shop.loadProducts();
            customerManager.loadCustomers();
            int choice;
            do {
                cout << "\t\t\t\t---------------------------------------------" << endl;
		        cout << "\t\t\t\t\t    SHOPPING SYSTEM" << endl;
		        cout << "\t\t\t\t---------------------------------------------" << endl;
		        cout << "\t\t\t\t\t    1. Available Products At Your store"<< endl;
		        cout << "\t\t\t\t\t    2. Add to Cart"<< endl;
		        cout << "\t\t\t\t\t    3. See Your Selected Products(Cart)"<< endl;
		        cout << "\t\t\t\t\t    4. Checkout"<< endl;
		        cout << "\t\t\t\t\t    5. Your Registered Customers"<< endl;
		        cout << "\t\t\t\t\t    6. Add Customer To your List"<< endl;
		        cout << "\t\t\t\t\t    7. Add New Product to Your Record"<< endl;
		        cout << "\t\t\t\t\t    8. Search From Available Product"<< endl;
		        cout << "\t\t\t\t\t    9. Search Recent Customers "<< endl;
		        cout << "\t\t\t\t\t    10. Exit"<< endl;
		        cout << "\t\t\t\t---------------------------------------------" << endl;
		        cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                        shop.showProducts();
                        break;
                    case 2: {
                        shop.showProducts();
                        string productId;
                        cout << "Enter Product ID to add to cart: ";
                        cin >> productId;
                        Product* product = shop.findProduct(productId);
                        if (product) {
                            int qty;
                            cout << "Enter quantity: ";
                            cin >> qty;
                            if (qty <= product->getQuantity()) {
                                product->updateQuantity(qty); // Update the product quantity in inventory
                                cart.addProduct(*product, qty); // Add product to cart with quantity
                                cout << "Product added to cart." << endl;
                            } else {
                                cout << "Insufficient quantity!" << endl;
                            }
                        } else {
                            cout << "Product not found!" << endl;
                        }
                        break;
                    }
                    case 3:
                        cart.showCart();
                        break;
                    case 4: {
                        string customerId, customerName;
                        getCustomerDetails(customerId, customerName);
                        // Choose payment method
                        int paymentChoice;
                        cout << "Choose Payment Method:\n1. Credit Card\n2. PayPal\nEnter choice: ";
                        cin >> paymentChoice;
                        Payment* paymentMethod = nullptr;
                        if (paymentChoice == 1) {
                            paymentMethod = new CreditCardPayment();
                        } else if (paymentChoice == 2) {
                            paymentMethod = new PayPalPayment();
                        } else {
                            cout << "Invalid payment method!" << endl;
                            break;
                        }
                        Order order(cart, customerId, customerName, paymentMethod);
                        customerManager.saveCustomers();
                        order.processOrder();
                        delete paymentMethod; // Clean up
                        
                        
   						// Display current date, time, day, and assigned date
   						dt.displayDateTime();
                        break;
                    }
                    case 5:
                        customerManager.showCustomers();
                        break;
                    case 6: {
                        string id, name;
                        cout << "Enter Customer ID: ";
                        cin >> id;
                        cout << "Enter Customer Name: ";
                        cin >> name;
                        customerManager.addCustomer(id, name);
                        break;
                    }
                    case 7: {
                        string id, name;
                        double price;
                        int quantity;
                        cout << "Enter Product ID: ";
                        cin >> id;
                        cout << "Enter Product Name: ";
                        cin >> name;
                        cout << "Enter Product Price: ";
                        cin >> price;
                        cout << "Enter Product Quantity: ";
                        cin >> quantity;
                        shop.addProduct(Product(id, name, price, quantity));
                        break;
                    }
                    case 8: {
                        string productId;
                        cout << "Enter Product ID to search: ";
                        cin >> productId;
                        inventoryManager.searchProduct(productId);
                        break;
                    } 
                    case 9: {
    					searchCustomerById(); // Call the function without parameters
    					break;
					}
                    case 10:
                        cout << "Exiting the program. Goodbye!" << endl;
                        break;
                    default:
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
            } while (choice != 10);
            shop.saveProducts();
            customerManager.saveCustomers();
        } else {
            cout << "Invalid Manager ID or Password!" << endl;
        }
    }
};

int main() {
    ShoppingSystem system;
    system.run();
    return 0;
}