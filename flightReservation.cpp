#include<iostream>
#include<string>
using namespace std;

class FlightTicket {
public:
    int ticketID;
    string passengerName;
    string flightNumber;
    string destination;
    string departureTime;
    int seatNumber;
    
    FlightTicket() : ticketID(0), passengerName(""), flightNumber(""), 
                    destination(""), departureTime(""), seatNumber(0) {}
    
    FlightTicket(int id, string name, string flight, string dest, 
                string time, int seat) {
        ticketID = id;
        passengerName = name;
        flightNumber = flight;
        destination = dest;
        departureTime = time;
        seatNumber = seat;
    }
    
    void display() const {
        cout << "Ticket ID: " << ticketID << " | Passenger: " << passengerName 
             << " | Flight: " << flightNumber << " | To: " << destination
             << " | Time: " << departureTime << " | Seat: " << seatNumber << endl;
    }
};

class TicketNode {
public:
    FlightTicket ticket;
    TicketNode* next;
    
    TicketNode(FlightTicket t) : ticket(t), next(nullptr) {}
};

class FlightReservationSystem {
private:
    TicketNode** hashTable;
    int tableSize;
    int totalTickets;
    
    int hashFunction(int ticketID) {
        return ticketID % tableSize;
    }
    
    bool isTicketIDUnique(int ticketID) {
        int index = hashFunction(ticketID);
        TicketNode* current = hashTable[index];
        
        while(current != nullptr) {
            if(current->ticket.ticketID == ticketID) {
                return false;
            }
            current = current->next;
        }
        return true;
    }
    
public:
    FlightReservationSystem(int size = 10) {
        tableSize = size;
        totalTickets = 0;
        hashTable = new TicketNode*[tableSize];
        
        for(int i = 0; i < tableSize; i++) {
            hashTable[i] = nullptr;
        }
    }
    
    ~FlightReservationSystem() {
        for(int i = 0; i < tableSize; i++) {
            TicketNode* current = hashTable[i];
            while(current != nullptr) {
                TicketNode* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] hashTable;
    }
    
    bool bookTicket(int ticketID, string name, string flight, 
                   string dest, string time, int seat) {
        
        if(!isTicketIDUnique(ticketID)) {
            cout << "Error: Ticket ID " << ticketID << " already exists!\n";
            return false;
        }
        
        FlightTicket newTicket(ticketID, name, flight, dest, time, seat);
        int index = hashFunction(ticketID);
        TicketNode* newNode = new TicketNode(newTicket);
        
        if(hashTable[index] == nullptr) {
            hashTable[index] = newNode;
        } else {
            newNode->next = hashTable[index];
            hashTable[index] = newNode;
        }
        
        totalTickets++;
        cout << "Ticket booked successfully! Ticket ID: " << ticketID << endl;
        return true;
    }
    
    bool cancelTicket(int ticketID) {
        int index = hashFunction(ticketID);
        TicketNode* current = hashTable[index];
        TicketNode* previous = nullptr;
        
        while(current != nullptr) {
            if(current->ticket.ticketID == ticketID) {
                if(previous == nullptr) {
                    hashTable[index] = current->next;
                } else {
                    previous->next = current->next;
                }
                
                cout << "Ticket cancelled for passenger: " 
                     << current->ticket.passengerName << endl;
                delete current;
                totalTickets--;
                return true;
            }
            previous = current;
            current = current->next;
        }
        
        cout << "Ticket ID " << ticketID << " not found!\n";
        return false;
    }
    
    void searchTicket(int ticketID) {
        int index = hashFunction(ticketID);
        TicketNode* current = hashTable[index];
        
        while(current != nullptr) {
            if(current->ticket.ticketID == ticketID) {
                cout << "\n=== TICKET FOUND ===\n";
                current->ticket.display();
                return;
            }
            current = current->next;
        }
        
        cout << "Ticket ID " << ticketID << " not found!\n";
    }
    
    void searchByPassenger(string name) {
        bool found = false;
        cout << "\nSearching for passenger: " << name << endl;
        
        for(int i = 0; i < tableSize; i++) {
            TicketNode* current = hashTable[i];
            while(current != nullptr) {
                if(current->ticket.passengerName == name) {
                    if(!found) {
                        cout << "=== FOUND TICKETS ===\n";
                        found = true;
                    }
                    current->ticket.display();
                }
                current = current->next;
            }
        }
        
        if(!found) {
            cout << "No tickets found for passenger: " << name << endl;
        }
    }
    
    void displayAllTickets() {
        cout << "\n=== ALL BOOKED TICKETS (" << totalTickets << " tickets) ===\n";
        
        if(totalTickets == 0) {
            cout << "No tickets booked yet.\n";
            return;
        }
        
        for(int i = 0; i < tableSize; i++) {
            TicketNode* current = hashTable[i];
            while(current != nullptr) {
                current->ticket.display();
                current = current->next;
            }
        }
    }
    
    void displayFlightTickets(string flightNumber) {
        bool found = false;
        cout << "\nTickets for Flight " << flightNumber << ":\n";
        
        for(int i = 0; i < tableSize; i++) {
            TicketNode* current = hashTable[i];
            while(current != nullptr) {
                if(current->ticket.flightNumber == flightNumber) {
                    if(!found) {
                        cout << "Passenger List:\n";
                        found = true;
                    }
                    cout << "  " << current->ticket.passengerName 
                         << " (Seat: " << current->ticket.seatNumber 
                         << ", Ticket ID: " << current->ticket.ticketID << ")\n";
                }
                current = current->next;
            }
        }
        
        if(!found) {
            cout << "No tickets found for flight " << flightNumber << endl;
        }
    }
    
    void displayHashTableStructure() {
        cout << "\n=== HASH TABLE STRUCTURE ===\n";
        cout << "Table Size: " << tableSize << endl;
        cout << "Total Tickets: " << totalTickets << endl;
        cout << "\nBucket Details:\n";
        
        for(int i = 0; i < tableSize; i++) {
            cout << "[" << i << "]: ";
            TicketNode* current = hashTable[i];
            
            if(current == nullptr) {
                cout << "Empty";
            } else {
                int count = 0;
                while(current != nullptr) {
                    if(count > 0) cout << " -> ";
                    cout << current->ticket.ticketID;
                    current = current->next;
                    count++;
                }
                cout << " (" << count << " tickets)";
            }
            cout << endl;
        }
    }
    
    void displayStatistics() {
        cout << "\n=== SYSTEM STATISTICS ===\n";
        cout << "Total Tickets Booked: " << totalTickets << endl;
        cout << "Hash Table Size: " << tableSize << endl;
        
        int maxChainLength = 0;
        int emptyBuckets = 0;
        double loadFactor = (double)totalTickets / tableSize;
        
        for(int i = 0; i < tableSize; i++) {
            int chainLength = 0;
            TicketNode* current = hashTable[i];
            
            while(current != nullptr) {
                chainLength++;
                current = current->next;
            }
            
            if(chainLength > maxChainLength) {
                maxChainLength = chainLength;
            }
            
            if(chainLength == 0) {
                emptyBuckets++;
            }
        }
        
        cout << "Maximum Chain Length: " << maxChainLength << endl;
        cout << "Empty Buckets: " << emptyBuckets << endl;
        cout << "Load Factor: " << loadFactor << endl;
        
        if(loadFactor > 0.7) {
            cout << "Warning: High load factor! Consider resizing.\n";
        }
    }
    
    void updateTicket(int ticketID, string newTime, int newSeat) {
        int index = hashFunction(ticketID);
        TicketNode* current = hashTable[index];
        
        while(current != nullptr) {
            if(current->ticket.ticketID == ticketID) {
                cout << "Updating ticket " << ticketID << ":\n";
                cout << "Old Time: " << current->ticket.departureTime 
                     << " -> New Time: " << newTime << endl;
                cout << "Old Seat: " << current->ticket.seatNumber 
                     << " -> New Seat: " << newSeat << endl;
                
                current->ticket.departureTime = newTime;
                current->ticket.seatNumber = newSeat;
                
                cout << "Ticket updated successfully!\n";
                return;
            }
            current = current->next;
        }
        
        cout << "Ticket ID " << ticketID << " not found for update!\n";
    }
};

void displayMenu() {
    cout << "\n=== FLIGHT RESERVATION SYSTEM ===\n";
    cout << "1. Book New Ticket\n";
    cout << "2. Cancel Ticket\n";
    cout << "3. Search Ticket by ID\n";
    cout << "4. Search Tickets by Passenger Name\n";
    cout << "5. Display All Tickets\n";
    cout << "6. Display Tickets for Specific Flight\n";
    cout << "7. Update Ticket Details\n";
    cout << "8. Display Hash Table Structure\n";
    cout << "9. Display System Statistics\n";
    cout << "10. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    FlightReservationSystem reservationSystem(7);
    int choice;
    
    // Add some sample tickets
    reservationSystem.bookTicket(101, "Ali Ahmed", "PK-201", "New York", "10:00 AM", 15);
    reservationSystem.bookTicket(202, "Sara Khan", "PK-202", "London", "02:30 PM", 8);
    reservationSystem.bookTicket(303, "Ahmed Raza", "PK-201", "New York", "10:00 AM", 22);
    reservationSystem.bookTicket(404, "Fatima Noor", "PK-203", "Dubai", "06:45 PM", 12);
    reservationSystem.bookTicket(505, "John Smith", "PK-202", "London", "02:30 PM", 5);
    
    cout << "Flight Reservation System Initialized\n";
    cout << "5 sample tickets have been added.\n";
    
    do {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline from buffer
        
        switch(choice) {
            case 1: {
                int ticketID, seat;
                string name, flight, dest, time;
                
                cout << "Enter Ticket ID: ";
                cin >> ticketID;
                cout << "Enter Passenger Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter Flight Number: ";
                getline(cin, flight);
                cout << "Enter Destination: ";
                getline(cin, dest);
                cout << "Enter Departure Time: ";
                getline(cin, time);
                cout << "Enter Seat Number: ";
                cin >> seat;
                
                reservationSystem.bookTicket(ticketID, name, flight, dest, time, seat);
                break;
            }
                
            case 2: {
                int ticketID;
                cout << "Enter Ticket ID to cancel: ";
                cin >> ticketID;
                reservationSystem.cancelTicket(ticketID);
                break;
            }
                
            case 3: {
                int ticketID;
                cout << "Enter Ticket ID to search: ";
                cin >> ticketID;
                reservationSystem.searchTicket(ticketID);
                break;
            }
                
            case 4: {
                string name;
                cout << "Enter Passenger Name to search: ";
                getline(cin, name);
                reservationSystem.searchByPassenger(name);
                break;
            }
                
            case 5:
                reservationSystem.displayAllTickets();
                break;
                
            case 6: {
                string flight;
                cout << "Enter Flight Number: ";
                getline(cin, flight);
                reservationSystem.displayFlightTickets(flight);
                break;
            }
                
            case 7: {
                int ticketID, newSeat;
                string newTime;
                
                cout << "Enter Ticket ID to update: ";
                cin >> ticketID;
                cout << "Enter New Departure Time: ";
                cin.ignore();
                getline(cin, newTime);
                cout << "Enter New Seat Number: ";
                cin >> newSeat;
                
                reservationSystem.updateTicket(ticketID, newTime, newSeat);
                break;
            }
                
            case 8:
                reservationSystem.displayHashTableStructure();
                break;
                
            case 9:
                reservationSystem.displayStatistics();
                break;
                
            case 10:
                cout << "Exiting Flight Reservation System. Thank you!\n";
                break;
                
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        
    } while(choice != 10);
    
    return 0;
}