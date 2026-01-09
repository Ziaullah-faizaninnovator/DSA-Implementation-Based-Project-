#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class RestaurantOrderSystem {
private:
    struct Order {
        int orderID;
        int priority;
        string customerName;
        string foodItems;
        int prepTime;
        int arrivalTime;
        double billAmount;
        
        void display() const {
            cout << left << setw(8) << orderID 
                 << setw(15) << customerName 
                 << setw(25) << foodItems 
                 << "Priority: " << setw(2) << priority << "/10 | "
                 << "Time: " << setw(3) << prepTime << "min | "
                 << "Bill: $" << fixed << setprecision(2) << billAmount << endl;
        }
    };
    
    Order* heap;
    int capacity;
    int size;
    int orderCounter;
    
    int parent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }
    
    void heapifyUp(int index) {
        while (index > 0) {
            int p = parent(index);
            
            if (heap[index].priority > heap[p].priority ||
                (heap[index].priority == heap[p].priority && 
                 heap[index].arrivalTime < heap[p].arrivalTime)) {
                Order temp = heap[index];
                heap[index] = heap[p];
                heap[p] = temp;
                index = p;
            } else {
                break;
            }
        }
    }
    
    void heapifyDown(int index) {
        int largest = index;
        int left = leftChild(index);
        int right = rightChild(index);
        
        if (left < size) {
            if (heap[left].priority > heap[largest].priority ||
                (heap[left].priority == heap[largest].priority && 
                 heap[left].arrivalTime < heap[largest].arrivalTime)) {
                largest = left;
            }
        }
        
        if (right < size) {
            if (heap[right].priority > heap[largest].priority ||
                (heap[right].priority == heap[largest].priority && 
                 heap[right].arrivalTime < heap[largest].arrivalTime)) {
                largest = right;
            }
        }
        
        if (largest != index) {
            Order temp = heap[index];
            heap[index] = heap[largest];
            heap[largest] = temp;
            heapifyDown(largest);
        }
    }
    
    void resize() {
        int newCapacity = capacity * 2;
        Order* newHeap = new Order[newCapacity];
        
        for (int i = 0; i < size; i++) {
            newHeap[i] = heap[i];
        }
        
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }
    
public:
    RestaurantOrderSystem(int cap = 10) {
        capacity = cap;
        size = 0;
        orderCounter = 1000;
        heap = new Order[capacity];
    }
    
    ~RestaurantOrderSystem() {
        delete[] heap;
    }
    
    void placeOrder(string customer, string items, int priority, int prepTime, double bill) {
        if (priority < 1 || priority > 10) {
            cout << "Error: Priority must be between 1 and 10!" << endl;
            return;
        }
        
        if (size == capacity) {
            resize();
        }
        
        heap[size].orderID = ++orderCounter;
        heap[size].customerName = customer;
        heap[size].foodItems = items;
        heap[size].priority = priority;
        heap[size].prepTime = prepTime;
        heap[size].billAmount = bill;
        heap[size].arrivalTime = size;
        
        heapifyUp(size);
        size++;
        
        cout << "Order #" << orderCounter << " placed successfully!" << endl;
        cout << "Customer: " << customer << " | Priority: " << priority << endl;
    }
    
    Order processNextOrder() {
        if (isEmpty()) {
            return Order{0, 0, "None", "No orders", 0, 0, 0.0};
        }
        
        Order nextOrder = heap[0];
        heap[0] = heap[size - 1];
        size--;
        
        if (size > 0) {
            heapifyDown(0);
        }
        
        return nextOrder;
    }
    
    Order viewNextOrder() const {
        if (isEmpty()) {
            return Order{0, 0, "None", "No orders", 0, 0, 0.0};
        }
        return heap[0];
    }
    
    void vipOrder(string customer, string items, double bill) {
        cout << "VIP ORDER" << endl;
        placeOrder(customer, items, 10, 15, bill);
    }
    
    bool cancelOrder(int orderID) {
        for (int i = 0; i < size; i++) {
            if (heap[i].orderID == orderID) {
                cout << "Order #" << orderID << " cancelled for " 
                     << heap[i].customerName << endl;
                
                heap[i] = heap[size - 1];
                size--;
                
                heapifyDown(i);
                if (i > 0) heapifyUp(i);
                
                return true;
            }
        }
        cout << "Order #" << orderID << " not found!" << endl;
        return false;
    }
    
    void searchByCustomer(string name) const {
        bool found = false;
        cout << "Search results for '" << name << "':" << endl;
        cout << "----------------------------------------" << endl;
        
        for (int i = 0; i < size; i++) {
            bool match = false;
            string customer = heap[i].customerName;
            
            if (customer.length() >= name.length()) {
                for (size_t j = 0; j <= customer.length() - name.length(); j++) {
                    bool foundMatch = true;
                    for (size_t k = 0; k < name.length(); k++) {
                        if (customer[j + k] != name[k]) {
                            foundMatch = false;
                            break;
                        }
                    }
                    if (foundMatch) {
                        match = true;
                        break;
                    }
                }
            }
            
            if (match) {
                heap[i].display();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No orders found for customer '" << name << "'" << endl;
        }
    }
    
    bool updatePriority(int orderID, int newPriority) {
        for (int i = 0; i < size; i++) {
            if (heap[i].orderID == orderID) {
                int oldPriority = heap[i].priority;
                heap[i].priority = newPriority;
                heap[i].arrivalTime = size;
                
                cout << "Order #" << orderID << " priority updated from " 
                     << oldPriority << " to " << newPriority << endl;
                
                heapifyDown(i);
                if (i > 0) heapifyUp(i);
                
                return true;
            }
        }
        cout << "Order #" << orderID << " not found!" << endl;
        return false;
    }
    
    void displayStatistics() const {
        cout << "RESTAURANT STATISTICS" << endl;
        cout << "======================" << endl;
        cout << "Pending Orders: " << size << endl;
        cout << "System Capacity: " << capacity << endl;
        
        if (!isEmpty()) {
            Order next = viewNextOrder();
            cout << "Next Order to Process:" << endl;
            next.display();
        }
        
        int totalPrepTime = 0;
        double totalRevenue = 0;
        int highPriority = 0;
        
        for (int i = 0; i < size; i++) {
            totalPrepTime += heap[i].prepTime;
            totalRevenue += heap[i].billAmount;
            if (heap[i].priority >= 8) highPriority++;
        }
        
        cout << "Total Estimated Prep Time: " << totalPrepTime << " minutes" << endl;
        cout << "Potential Revenue: $" << fixed << setprecision(2) << totalRevenue << endl;
        cout << "High Priority Orders (8+): " << highPriority << endl;
        cout << "======================" << endl;
    }
    
    void displayAllOrders() const {
        if (isEmpty()) {
            cout << "No pending orders." << endl;
            return;
        }
        
        cout << "PENDING ORDERS (" << size << " orders)" << endl;
        cout << "================================================================" << endl;
        cout << left << setw(8) << "Order#" 
             << setw(15) << "Customer" 
             << setw(25) << "Items" 
             << setw(10) << "Priority" 
             << setw(10) << "Prep Time" 
             << setw(10) << "Bill" << endl;
        cout << "----------------------------------------------------------------" << endl;
        
        for (int i = 0; i < size; i++) {
            cout << setw(8) << heap[i].orderID 
                 << setw(15) << heap[i].customerName 
                 << setw(25) << heap[i].foodItems 
                 << setw(10) << (to_string(heap[i].priority) + "/10")
                 << setw(10) << (to_string(heap[i].prepTime) + "min")
                 << "$" << fixed << setprecision(2) << setw(9) << heap[i].billAmount << endl;
        }
        cout << "================================================================" << endl;
    }
    
    void kitchenWorkflow() {
        if (isEmpty()) {
            cout << "Kitchen is idle. No orders to process." << endl;
            return;
        }
        
        cout << "KITCHEN WORKFLOW" << endl;
        cout << "=================" << endl;
        
        RestaurantOrderSystem temp = *this;
        int ordersProcessed = 0;
        int totalTime = 0;
        
        while (!temp.isEmpty() && ordersProcessed < 3) {
            Order current = temp.processNextOrder();
            ordersProcessed++;
            totalTime += current.prepTime;
            
            cout << "Processing Order #" << current.orderID << endl;
            cout << "Customer: " << current.customerName << endl;
            cout << "Items: " << current.foodItems << endl;
            cout << "Prep Time: " << current.prepTime << " minutes" << endl;
        }
        
        cout << "Completed " << ordersProcessed << " order(s)" << endl;
        cout << "Total kitchen time: " << totalTime << " minutes" << endl;
        cout << "Pending orders: " << size << endl;
    }
    
    bool isHeapValid() const {
        for (int i = 0; i < size; i++) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            
            if (left < size) {
                if (heap[i].priority < heap[left].priority ||
                    (heap[i].priority == heap[left].priority && 
                     heap[i].arrivalTime > heap[left].arrivalTime)) {
                    return false;
                }
            }
            
            if (right < size) {
                if (heap[i].priority < heap[right].priority ||
                    (heap[i].priority == heap[right].priority && 
                     heap[i].arrivalTime > heap[right].arrivalTime)) {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }
    
    void clearAll() {
        size = 0;
        orderCounter = 1000;
        cout << "All orders cleared. System reset." << endl;
    }
};

void displayMenu() {
    cout << "RESTAURANT ORDER MANAGEMENT SYSTEM" << endl;
    cout << "==================================" << endl;
    cout << "1. Place New Order" << endl;
    cout << "2. Process Next Order" << endl;
    cout << "3. VIP Order" << endl;
    cout << "4. Cancel Order" << endl;
    cout << "5. Search Customer" << endl;
    cout << "6. Update Order Priority" << endl;
    cout << "7. View All Orders" << endl;
    cout << "8. Kitchen Workflow" << endl;
    cout << "9. Display Statistics" << endl;
    cout << "10. Validate Heap" << endl;
    cout << "11. Clear All Orders" << endl;
    cout << "12. Exit" << endl;
    cout << "==================================" << endl;
    cout << "Enter choice: ";
}

int main() {
    RestaurantOrderSystem restaurant;
    
    restaurant.placeOrder("Ali Khan", "Burger, Fries, Coke", 5, 10, 15.99);
    restaurant.placeOrder("Sara Ahmed", "Pizza, Salad", 7, 15, 22.50);
    restaurant.placeOrder("John Smith", "Steak, Mashed Potatoes", 8, 25, 35.75);
    restaurant.placeOrder("Fatima Noor", "Pasta, Garlic Bread", 4, 12, 18.25);
    restaurant.vipOrder("Mr. Johnson", "Lobster, Wine", 85.99);
    
    cout << "Welcome to Restaurant Order Management System!" << endl;
    cout << "Sample orders have been added." << endl;
    
    int choice;
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1: {
                string name, items;
                int priority, prepTime;
                double bill;
                
                cout << "Customer Name: ";
                getline(cin, name);
                cout << "Food Items: ";
                getline(cin, items);
                cout << "Priority (1-10): ";
                cin >> priority;
                cout << "Preparation Time (minutes): ";
                cin >> prepTime;
                cout << "Bill Amount: $";
                cin >> bill;
                
                restaurant.placeOrder(name, items, priority, prepTime, bill);
                break;
            }
                
            case 2: {
                Order order = restaurant.processNextOrder();
                if (order.orderID != 0) {
                    cout << "ORDER COMPLETED" << endl;
                    cout << "Order #" << order.orderID << " for " 
                         << order.customerName << " is ready!" << endl;
                    cout << "Total: $" << fixed << setprecision(2) << order.billAmount << endl;
                }
                break;
            }
                
            case 3: {
                string name, items;
                double bill;
                
                cout << "VIP Customer Name: ";
                getline(cin, name);
                cout << "Food Items: ";
                getline(cin, items);
                cout << "Bill Amount: $";
                cin >> bill;
                
                restaurant.vipOrder(name, items, bill);
                break;
            }
                
            case 4: {
                int orderID;
                cout << "Enter Order ID to cancel: ";
                cin >> orderID;
                restaurant.cancelOrder(orderID);
                break;
            }
                
            case 5: {
                string name;
                cout << "Enter customer name to search: ";
                getline(cin, name);
                restaurant.searchByCustomer(name);
                break;
            }
                
            case 6: {
                int orderID, newPriority;
                cout << "Enter Order ID: ";
                cin >> orderID;
                cout << "Enter new priority (1-10): ";
                cin >> newPriority;
                restaurant.updatePriority(orderID, newPriority);
                break;
            }
                
            case 7:
                restaurant.displayAllOrders();
                break;
                
            case 8:
                restaurant.kitchenWorkflow();
                break;
                
            case 9:
                restaurant.displayStatistics();
                break;
                
            case 10:
                if (restaurant.isHeapValid()) {
                    cout << "Heap structure is valid!" << endl;
                } else {
                    cout << "Heap structure is corrupted!" << endl;
                }
                break;
                
            case 11:
                restaurant.clearAll();
                break;
                
            case 12:
                cout << "Thank you for using Restaurant Management System!" << endl;
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
    } while (choice != 12);
    
    return 0;
}