#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class MaxHeap {
private:
    struct Patient {
        int priority;
        string name;
        string condition;
        int arrivalTime;
        
        void display() const {
            cout << left << setw(15) << name 
                 << setw(20) << condition 
                 << "Severity: " << setw(2) << priority << "/10" 
                 << " | Arrival: " << arrivalTime << endl;
        }
    };
    
    Patient* heap;
    int capacity;
    int size;
    int patientCounter;
    
    // Helper functions - made const
    int getParent(int i) const { return (i - 1) / 2; }
    int leftChild(int i) const { return 2 * i + 1; }
    int rightChild(int i) const { return 2 * i + 2; }
    
    void heapifyUp(int index) {
        while (index > 0) {
            int parent = getParent(index);
            if (heap[index].priority > heap[parent].priority ||
                (heap[index].priority == heap[parent].priority && 
                 heap[index].arrivalTime < heap[parent].arrivalTime)) {
                swap(heap[index], heap[parent]);
                index = parent;
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
            swap(heap[index], heap[largest]);
            heapifyDown(largest);
        }
    }
    
    void resize() {
        int newCapacity = capacity * 2;
        Patient* newHeap = new Patient[newCapacity];
        
        for (int i = 0; i < size; i++) {
            newHeap[i] = heap[i];
        }
        
        delete[] heap;
        heap = newHeap;
        capacity = newCapacity;
    }
    
public:
    // Constructor
    MaxHeap(int initialCapacity = 5) {
        capacity = initialCapacity;
        size = 0;
        patientCounter = 0;
        heap = new Patient[capacity];
    }
    
    // Destructor
    ~MaxHeap() {
        delete[] heap;
    }
    
    // Copy Constructor (needed for displayByPriority)
    MaxHeap(const MaxHeap& other) {
        capacity = other.capacity;
        size = other.size;
        patientCounter = other.patientCounter;
        heap = new Patient[capacity];
        
        for (int i = 0; i < size; i++) {
            heap[i] = other.heap[i];
        }
    }
    
    // Insert operation
    void insert(int priority, string name, string condition) {
        if (priority < 1 || priority > 10) {
            cout << "Error: Priority must be between 1 and 10!" << endl;
            return;
        }
        
        if (size == capacity) {
            resize();
        }
        
        heap[size].priority = priority;
        heap[size].name = name;
        heap[size].condition = condition;
        heap[size].arrivalTime = ++patientCounter;
        
        heapifyUp(size);
        size++;
        
        cout << "Patient admitted: " << name 
             << " (Priority: " << priority << ")" << endl;
    }
    
    // Extract max element
    Patient extractMax() {
        if (isEmpty()) {
            cout << "Heap is empty!" << endl;
            return Patient{0, "None", "Empty", 0};
        }
        
        Patient maxPatient = heap[0];
        heap[0] = heap[size - 1];
        size--;
        
        if (size > 0) {
            heapifyDown(0);
        }
        
        return maxPatient;
    }
    
    // Peek at max element
    Patient peekMax() const {
        if (isEmpty()) {
            cout << "Heap is empty!" << endl;
            return Patient{0, "None", "Empty", 0};
        }
        return heap[0];
    }
    
    // Hospital functions
    void admitPatient(string name, string condition, int severity) {
        insert(severity, name, condition);
    }
    
    void treatNextPatient() {
        if (isEmpty()) {
            cout << "No patients to treat!" << endl;
            return;
        }
        
        Patient patient = extractMax();
        
        cout << "\n------------------------------" << endl;
        cout << "    TREATING PATIENT" << endl;
        cout << "------------------------------" << endl;
        cout << "Name: " << patient.name << endl;
        cout << "Condition: " << patient.condition << endl;
        cout << "Severity: " << patient.priority << "/10" << endl;
        cout << "Arrival Time: " << patient.arrivalTime << endl;
        cout << "------------------------------" << endl;
    }
    
    void emergencyCase(string name, string condition) {
        cout << "EMERGENCY CASE: " << name << endl;
        insert(10, name, condition);
    }
    
    // Search patient by name
    bool searchPatient(const string& name) const {
        for (int i = 0; i < size; i++) {
            if (heap[i].name == name) {
                cout << "Found patient: ";
                heap[i].display();
                return true;
            }
        }
        cout << "Patient '" << name << "' not found." << endl;
        return false;
    }
    
    // Count patients by severity
    int countBySeverity(int severity) const {
        int count = 0;
        for (int i = 0; i < size; i++) {
            if (heap[i].priority == severity) {
                count++;
            }
        }
        return count;
    }
    
    // Display statistics
    void getStatistics() const {
        cout << "\nHOSPITAL STATISTICS" << endl;
        cout << "-------------------" << endl;
        cout << "Total Patients: " << size << endl;
        cout << "Heap Capacity: " << capacity << endl;
        
        if (!isEmpty()) {
            Patient next = peekMax();
            cout << "Next Patient: " << next.name 
                 << " (Severity: " << next.priority << ")" << endl;
        }
        
        cout << "\nSeverity Distribution:" << endl;
        for (int i = 1; i <= 10; i++) {
            int count = countBySeverity(i);
            if (count > 0) {
                cout << "Level " << i << ": " << count << " patients" << endl;
            }
        }
        cout << "-------------------" << endl;
    }
    
    // Display patients in priority order
    void displayByPriority() {
        if (isEmpty()) {
            cout << "Waiting room is empty." << endl;
            return;
        }
        
        // Create a temporary heap copy
        MaxHeap temp = *this;
        
        cout << "\nHOSPITAL WAITING ROOM (" << size << " patients)" << endl;
        cout << "================================================" << endl;
        cout << left << setw(5) << "No." 
             << setw(15) << "Name" 
             << setw(20) << "Condition" 
             << setw(12) << "Severity" 
             << setw(12) << "Arrival" << endl;
        cout << "------------------------------------------------" << endl;
        
        int count = 1;
        while (!temp.isEmpty()) {
            Patient p = temp.extractMax();
            cout << setw(5) << count++ 
                 << setw(15) << p.name 
                 << setw(20) << p.condition 
                 << setw(12) << (to_string(p.priority) + "/10")
                 << setw(12) << p.arrivalTime << endl;
        }
        cout << "================================================" << endl;
    }
    
    // Display heap as array
    void displayHeapArray() const {
        cout << "\nHeap Array Structure:" << endl;
        cout << "Index | Priority | Name" << endl;
        cout << "------|----------|--------" << endl;
        for (int i = 0; i < size; i++) {
            cout << setw(5) << i << " | "
                 << setw(8) << heap[i].priority << " | "
                 << heap[i].name << endl;
        }
    }
    
    // Check if heap is valid (without calling non-const member functions)
    bool isValidHeap() const {
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
    
    // Utility functions
    bool isEmpty() const { return size == 0; }
    int getSize() const { return size; }
    int getCapacity() const { return capacity; }
    
    void clear() {
        size = 0;
        patientCounter = 0;
        cout << "Hospital queue cleared." << endl;
    }
};

// Menu interface
void displayMenu() {
    cout << "\nHOSPITAL MANAGEMENT SYSTEM" << endl;
    cout << "==========================" << endl;
    cout << "1. Admit Patient" << endl;
    cout << "2. Treat Next Patient" << endl;
    cout << "3. Emergency Case" << endl;
    cout << "4. Display Waiting Room" << endl;
    cout << "5. Search Patient" << endl;
    cout << "6. Show Statistics" << endl;
    cout << "7. Display Heap Structure" << endl;
    cout << "8. Clear Queue" << endl;
    cout << "9. Validate Heap" << endl;
    cout << "10. Exit" << endl;
    cout << "==========================" << endl;
    cout << "Enter choice: ";
}

int main() {
    MaxHeap hospital;
    int choice;
    
    cout << "HOSPITAL EMERGENCY ROOM SYSTEM" << endl;
    cout << "===============================" << endl;
    
    // Add some initial patients
    hospital.admitPatient("Ali Ahmed", "Heart Attack", 9);
    hospital.admitPatient("Sara Khan", "Broken Arm", 4);
    hospital.admitPatient("Ahmed Raza", "Stroke", 10);
    hospital.admitPatient("Fatima Noor", "Fever", 2);
    hospital.admitPatient("John Smith", "Appendicitis", 7);
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore();
        
        switch(choice) {
            case 1: {
                string name, condition;
                int severity;
                
                cout << "Enter patient name: ";
                getline(cin, name);
                cout << "Enter condition: ";
                getline(cin, condition);
                cout << "Enter severity (1-10): ";
                cin >> severity;
                
                hospital.admitPatient(name, condition, severity);
                break;
            }
            
            case 2:
                hospital.treatNextPatient();
                break;
                
            case 3: {
                string name, condition;
                cout << "Emergency Case Name: ";
                getline(cin, name);
                cout << "Condition: ";
                getline(cin, condition);
                hospital.emergencyCase(name, condition);
                break;
            }
                
            case 4:
                hospital.displayByPriority();
                break;
                
            case 5: {
                string name;
                cout << "Enter patient name to search: ";
                getline(cin, name);
                hospital.searchPatient(name);
                break;
            }
                
            case 6:
                hospital.getStatistics();
                break;
                
            case 7:
                hospital.displayHeapArray();
                break;
                
            case 8:
                hospital.clear();
                break;
                
            case 9:
                if (hospital.isEmpty()) {
                    cout << "Heap is empty." << endl;
                } else if (hospital.isValidHeap()) {
                    cout << "Heap is valid." << endl;
                } else {
                    cout << "Heap is not valid!" << endl;
                }
                break;
                
            case 10:
                cout << "Exiting system. Thank you!" << endl;
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
    } while (choice != 10);
    
    return 0;
}