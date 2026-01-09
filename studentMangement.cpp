#include<iostream>
#include<string>
using namespace std;

class Student {
public:
    int id;
    string name;
    string dept;
    double gpa;
    
    Student() : id(0), name(""), dept(""), gpa(0.0) {}
    
    Student(int i, string n, string d, double g) {
        id = i;
        name = n;
        dept = d;
        gpa = g;
    }
};

class Node {
public:
    Student data;
    Node* next;
    
    Node(Student s) : data(s), next(nullptr) {}
};

class HashTable {
private:
    Node** table;
    int size;
    
public:
    HashTable(int s = 10) {
        size = s;
        table = new Node*[size];
        for(int i = 0; i < size; i++) {
            table[i] = nullptr;
        }
    }
    
    ~HashTable() {
        for(int i = 0; i < size; i++) {
            Node* curr = table[i];
            while(curr != nullptr) {
                Node* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
        delete[] table;
    }
    
    // Simple hash function
    int hash(int id) {
        return id % size;
    }
    
    // Check if ID is unique
    bool isUniqueID(int id) {
        int index = hash(id);
        Node* temp = table[index];
        
        while(temp != nullptr) {
            if(temp->data.id == id) {
                return false; // ID already exists
            }
            temp = temp->next;
        }
        return true; // ID is unique
    }
    
    // Insert student with given ID
    bool insertStudent(int id, string name, string dept, double gpa) {
        // Check if ID is unique
        if(!isUniqueID(id)) {
            cout << "Error: Student ID " << id << " already exists!\n";
            return false;
        }
        
        Student s(id, name, dept, gpa);
        int index = hash(id);
        Node* newNode = new Node(s);
        
        // Insert at beginning
        if(table[index] == nullptr) {
            table[index] = newNode;
        } else {
            newNode->next = table[index];
            table[index] = newNode;
        }
        
        cout << "Student '" << name << "' added successfully!\n";
        return true;
    }
    
    // Delete student by ID
    bool deleteStudent(int id) {
        int index = hash(id);
        Node* curr = table[index];
        Node* prev = nullptr;
        
        while(curr != nullptr) {
            if(curr->data.id == id) {
                if(prev == nullptr) {
                    // First node in chain
                    table[index] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                
                cout << "Student '" << curr->data.name << "' deleted successfully!\n";
                delete curr;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        
        cout << "Error: Student with ID " << id << " not found!\n";
        return false;
    }
    
    // Search student
    void searchStudent(int id) {
        int index = hash(id);
        Node* temp = table[index];
        
        while(temp != nullptr) {
            if(temp->data.id == id) {
                cout << "\n=== Student Found ===\n";
                cout << "ID: " << temp->data.id << endl;
                cout << "Name: " << temp->data.name << endl;
                cout << "Department: " << temp->data.dept << endl;
                cout << "GPA: " << temp->data.gpa << endl;
                return;
            }
            temp = temp->next;
        }
        cout << "\nStudent with ID " << id << " not found!\n";
    }
    
    // Display all students
    void displayAll() {
        cout << "\n=== ALL STUDENTS ===\n";
        bool found = false;
        
        for(int i = 0; i < size; i++) {
            Node* temp = table[i];
            while(temp != nullptr) {
                cout << "ID: " << temp->data.id 
                     << " | Name: " << temp->data.name 
                     << " | Dept: " << temp->data.dept 
                     << " | GPA: " << temp->data.gpa << endl;
                temp = temp->next;
                found = true;
            }
        }
        
        if(!found) {
            cout << "No students in database!\n";
        }
    }
    
    // Display table structure
    void displayTable() {
        cout << "\n=== HASH TABLE ===\n";
        for(int i = 0; i < size; i++) {
            cout << "[" << i << "]: ";
            Node* temp = table[i];
            
            if(temp == nullptr) {
                cout << "Empty";
            } else {
                while(temp != nullptr) {
                    cout << temp->data.id << "->";
                    temp = temp->next;
                }
                cout << "NULL";
            }
            cout << endl;
        }
    }
};

// Main function - simple menu
int main() {
    HashTable ht(5);
    int choice, id;
    string name, dept;
    double gpa;
    
    while(true) {
        cout << "\n======== STUDENT SYSTEM ========\n";
        cout << "1. Add Student\n";
        cout << "2. Delete Student\n";
        cout << "3. Search Student\n";
        cout << "4. Display All Students\n";
        cout << "5. Show Hash Table\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        
        switch(choice) {
            case 1:
                cout << "Enter Student ID: ";
                cin >> id;
                cout << "Enter Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Department: ";
                getline(cin, dept);
                cout << "Enter GPA: ";
                cin >> gpa;
                ht.insertStudent(id, name, dept, gpa);
                break;
                
            case 2:
                cout << "Enter Student ID to delete: ";
                cin >> id;
                ht.deleteStudent(id);
                break;
                
            case 3:
                cout << "Enter Student ID to search: ";
                cin >> id;
                ht.searchStudent(id);
                break;
                
            case 4:
                ht.displayAll();
                break;
                
            case 5:
                ht.displayTable();
                break;
                
            case 6:
                cout << "Exiting program...\n";
                return 0;
                
            default:
                cout << "Invalid choice! Try again.\n";
        }
    }
    
    return 0;
}