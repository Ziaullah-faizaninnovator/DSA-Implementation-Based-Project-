#include<iostream>
using namespace std;

class Order{
    public:
    string productName;
    int price;
    int quantity;
    string customerName;
    int priority;
    
    Order(){
        productName = "";
        price = 0;
        quantity = 0;
        customerName = "";
        priority = 0;
    }
    
    Order(string product, int p, int q, string customer, int pri){
        productName = product;
        price = p;
        quantity = q;
        customerName = customer;
        priority = pri;
    }
};

class Heap{
    public:
    Order orders[10];
    int size;
    
    Heap(){
        size = 0;
    }
    
    int getParent(int child) {
        if(child == 0) {
            return -1;
        }
        return (child - 1) / 2;
    }
    
    int getLeft(int parent) {
        int child = 2 * parent + 1;
        if(child < size) {
            return child;
        }
        return -1;
    }
    
    int getRight(int parent) {
        int child = 2 * parent + 2;
        if(child < size) {
            return child;
        }
        return -1;
    }
    
    void swap(Order *a, Order *b) {
        Order temp = *a;
        *a = *b;
        *b = temp;
    }
    
    void heapifyUp(int node){
        int parent = getParent(node);
        if(node >= 0 && parent >= 0 && orders[parent].priority < orders[node].priority){
            swap(&orders[parent], &orders[node]);
            heapifyUp(parent);
        }
    }
    
    void insertOrder(string product, int p, int q, string customer, int pri){
        if(size >= 10){
            cout << "Heap is full." << endl;
            return;
        }
        
        orders[size] = Order(product, p, q, customer, pri);
        heapifyUp(size);
        size++;
        cout << "Order added successfully!" << endl;
    }
    
    void heapifyDown(int node){
        int leftChild = getLeft(node);
        int rightChild = getRight(node);
        int largest = node;
        
        if(leftChild >= 0 && leftChild < size && orders[leftChild].priority > orders[largest].priority){
            largest = leftChild;
        }
        
        if(rightChild >= 0 && rightChild < size && orders[rightChild].priority > orders[largest].priority){
            largest = rightChild;
        }
        
        if(largest != node){
            swap(&orders[largest], &orders[node]);
            heapifyDown(largest);
        }
    }
    
    void serveOrder(){
        if(size == 0){
            cout << "No empty!" << endl;
            return;
        }
        
        cout <<endl<< "-----------------system is serving------------" << endl;
        cout << "Customer: " << orders[0].customerName << endl;
        cout << "Item: " << orders[0].productName << endl;
        cout << "Price: $" << orders[0].price << endl;
        cout << "Quantity: " << orders[0].quantity << endl;
        cout << "Priority: " << orders[0].priority << endl;
        swap(&orders[0], &orders[size-1]);
        size--;
        heapifyDown(0);
    }
    void displayOrders(){
        if(size == 0){
            cout << "No orders in the system!" << endl;
            return;
        }
        
        cout <<endl<<"----------------------------order list-------------------" << endl;
        for(int i = 0; i < size; i++){
            cout << "Order " << i+1 << ":" << endl;
            cout << "  Customer: " << orders[i].customerName << endl;
            cout << "  Item: " << orders[i].productName << endl;
            cout << "  Price: $" << orders[i].price << endl;
            cout << "  Quantity: " << orders[i].quantity << endl;
            cout << "  Priority: " << orders[i].priority << endl;
        }
    }
};
int main() {
    Heap orderHeap;
    int choice;
    do {
        cout << "----------------TKR. ORDER MANAGEMENT SYSTEM-------------------" << endl;
        cout << "1. Insert New Order" << endl;
        cout << "2. Serve Highest Priority Order" << endl;
        cout << "3. Display All Orders" << endl;
        cout << "4. Exit" << endl;
        cout << "Enter your choice (1   to   4): ";
        cin >> choice;
        switch(choice) {
            case 1: {
                string product, customer;
                int price, quantity, priority;
                cout << "Enter Product Name: ";
                cin.ignore();
                getline(cin, product);
                cout << "Enter Price: ";
                cin >> price;
                cout << "Enter Quantity: ";
                cin >> quantity;
                cout << "Enter Customer Name: ";
                cin.ignore();
                getline(cin, customer);
                cout << "Enter Priority: ";
                cin >> priority;
                orderHeap.insertOrder(product, price, quantity, customer, priority);
                break;
            }
            case 2:
                orderHeap.serveOrder();
                break;
            case 3:
                orderHeap.displayOrders();
                break;
            case 4:
                cout << "Thank you for using the system!" << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
    } while(choice != 4);
    return 0;
}