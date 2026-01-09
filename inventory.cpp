#include <iostream>
#include <string>
using namespace std;

class Product {
public:
    int Id;
    string name;
    int price;
    Product(int id, string n, int p) : Id(id), name(n), price(p) {}
};

class Node {
public:
    Product products;
    Node* next;
    Node* prev;
    Node(int id, string n, int p) : products(id, n, p), next(NULL), prev(NULL) {}
};

class DoublyLinkedList {
private:
    Node* head;
public:
    DoublyLinkedList() : head(NULL) {}
    bool isIdUnique(int id) {
        Node* temp = head;
        while (temp != NULL) {
            if (temp->products.Id == id) return false;
            temp = temp->next;
        }
        return true;
    }
    void insert(int id, string n, int p) {
        if (!isIdUnique(id)) return;
        Node* newNode = new Node(id, n, p);
        if (head == NULL) {
            head = newNode;
            return;
        }
        Node* temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newNode;
        newNode->prev = temp;
    }
    Node* searching(int id) {
        Node* temp = head;
        while (temp != NULL) {
            if (temp->products.Id == id) return temp;
            temp = temp->next;
        }
        return NULL;
    }
    void RemoveProduct(int id) {
        Node* target = searching(id);    // we create one function for all searching purpose in order reduce code 
        if (!target) return;
        if (target == head) {
            head = head->next;
            if (head) head->prev = NULL;
        } else {
            if (target->prev) target->prev->next = target->next;
            if (target->next) target->next->prev = target->prev;
        }
         cout<<"The '"<<target->products.name<<"' was removed from the inventory list ! "<<endl;
        delete target;
    }
    void UpdatePrice(int id, int newPrice) {
        Node* target = searching(id);
        if (!target) return;
        target->products.price = newPrice;
        cout<<"The price of the '"<< target->products.name<<"' was updated ! "<<endl;
    }
    void display() {
        Node* temp = head;
        while (temp != NULL) {
            cout << "ID: " << temp->products.Id
                 << ", Name: " << temp->products.name
                 << ", Price: " << temp->products.price << endl;
            temp = temp->next;
        }
    }
};

int main() {
    DoublyLinkedList inventory;
    inventory.insert(1, "Laptop", 1200);
    inventory.insert(2, "Mouse", 50);
    inventory.insert(2, " Mouse", 60);    //this was rejected and donot create  node due to duplicate 
    inventory.insert(3, "Keyboard", 75);
    inventory.display();
    Node* product = inventory.searching(2);
    if (product) cout << "Found the product : " << product->products.name << " " << product->products.price << endl;
    inventory.UpdatePrice(3, 100);
    inventory.RemoveProduct(1);
    inventory.display();
    return 0;
}
