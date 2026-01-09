#include<iostream>
using namespace std;
class Product {
public:
    int Id;
    string Name;
    int Price;
    Product(int id=0, string n="", int pri=0) {
        Id = id;
        Name = n;
        Price = pri;
    }
};
class Menu {
public:
    Product products[10];
    int countProducts;
    Menu() {
        countProducts = 0;
    }
    void addProduct(int id, string n, int pri) {
        if (countProducts < 10) {
            products[countProducts] = Product(id, n, pri);
            countProducts++;
        } else {
            cout << "Menu is full!" << endl;
        }
    }

    void displayMenu() {
        cout << endl<<"********** Cafeteria Menu *************" << endl;
        for (int i = 0; i < countProducts; i++) {
            cout << products[i].Id << " "
                 << products[i].Name << " Rs."
                 << products[i].Price << endl;
        }
    }
    Product getProductById(int id) {
        for (int i = 0; i < countProducts; i++) {
            if (products[i].Id == id) return products[i];
        }
        return 0; 
    }
};

class Node {
public:
    Product orderProduct;
    Node* next;
    Node(Product p) {
        orderProduct = p;
        next = nullptr;
    }
};

class QueueForOrder {
public:
    Node* front;
    Node* rear;
    QueueForOrder() {
        front = rear = nullptr;
    }
    bool isEmpty() {
        return (front == nullptr);
    }
    void enqueueOrder(Product p) {
        Node* newNode = new Node(p);
        if (rear == nullptr) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        cout << "Order was Added with Name : " << p.Name << endl;
    }
    void dequeueOrder() {
        if (isEmpty()) {
            cout << "No orders to process yet !" << endl;
            return;
        }
        Node* temp = front;
        cout << "Processing Order: " << temp->orderProduct.Name << endl;
        front = front->next;
        if (front == nullptr) rear = nullptr;
        delete temp;
    }

    void displayOrders() {
        if (isEmpty()) {
            cout << "No orders in queue." << endl;
            return;
        }
        cout << endl<< "--------Current Orders------------" << endl;
        Node* temp = front;
        while (temp != nullptr) {
            cout << temp->orderProduct.Id << " "
                 << temp->orderProduct.Name << " Rs."
                 << temp->orderProduct.Price << endl;
            temp = temp->next;
        }
    }
};

int main() {
    Menu menu;
    menu.addProduct(1, "Aloo Qeema", 150);
    menu.addProduct(2, "Biryani", 220);
    menu.addProduct(3, "QabaliPalaw", 220);
    menu.addProduct(4, "BurgerAnda", 150);
    menu.addProduct(5, "Tea", 50);
    QueueForOrder orderQueue;
    int choice;
    while (true) {
        cout << endl<<"------------------- Cafeteria Ordering System -------------" << endl;
        cout << "1. Show Menu" << endl;
        cout << "2. Place Order" << endl;
        cout << "3. Process Next Order" << endl;
        cout << "4. Show Current Orders" << endl;
        cout << "5. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        if (choice == 1) {
            menu.displayMenu();
        } 
        else if (choice == 2) {
            menu.displayMenu();
            int id;
            cout << "Enter Product ID to order: ";
            cin >> id;
            Product selected = menu.getProductById(id);  //selected this is our newobject for giving to order class in order reduce hard code..
            if (selected.Id != 0) {
                orderQueue.enqueueOrder(selected);
            } else {
                cout << "Invalid Product ID Please try again !" << endl;
            }
        } 
        else if (choice == 3) {
            orderQueue.dequeueOrder();
        } 
        else if (choice == 4) {
            orderQueue.displayOrders();
        } 
        else if (choice == 5) {
            cout << "Exiting system.......!" << endl;
            break;
        } 
        else {
            cout << "Invalid choice. Try again." << endl;
        }
    }
    return 0;
}
