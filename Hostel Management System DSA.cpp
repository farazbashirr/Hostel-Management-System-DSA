#include <iostream>
#include <string>
#include <map>
#include <list>
#include <queue>
#include <fstream>

using namespace std;

struct Student {
    string name;
    int age;
    string rollNumber;
    string roomNumber;
    double feeBalance;
    
    Student(const string& name, int age, const string& rollNumber, const string& roomNumber, double feeBalance)
        : name(name), age(age), rollNumber(rollNumber), roomNumber(roomNumber), feeBalance(feeBalance) {}
};

list<Student> students;
map<string, bool> rooms; // Using map instead of unordered_map
queue<string> complaints;

void addStudent(const string& name, int age, const string& rollNumber, const string& roomNumber, double feeBalance) {
    students.push_back(Student(name, age, rollNumber, roomNumber, feeBalance));
}

void viewStudents() {
    for (list<Student>::const_iterator it = students.begin(); it != students.end(); ++it) {
        cout << "Name: " << it->name << ", Age: " << it->age 
             << ", Roll Number: " << it->rollNumber
             << ", Room Number: " << it->roomNumber
             << ", Fee Balance: " << it->feeBalance << endl;
    }
}

Student* searchStudent(const string& rollNumber) {
    for (list<Student>::iterator it = students.begin(); it != students.end(); ++it) {
        if (it->rollNumber == rollNumber) {
            return &(*it);
        }
    }
    return NULL;
}

void deleteStudent(const string& rollNumber) {
    students.remove_if([&](const Student& student) { return student.rollNumber == rollNumber; });
}

void addRoom(const string& roomNumber) {
    rooms[roomNumber] = false; // false means room is vacant
}

void allocateRoom(const string& roomNumber, const string& rollNumber) {
    Student* student = searchStudent(rollNumber);
    if (student != NULL && rooms.find(roomNumber) != rooms.end() && !rooms[roomNumber]) {
        student->roomNumber = roomNumber;
        rooms[roomNumber] = true; // room is now occupied
        cout << "Room " << roomNumber << " allocated to " << student->name << endl;
    } else {
        cout << "Room allocation failed." << endl;
    }
}

void payFee(const string& rollNumber, double amount) {
    Student* student = searchStudent(rollNumber);
    if (student != NULL) {
        student->feeBalance -= amount;
        cout << "Fee payment successful. Remaining balance: " << student->feeBalance << endl;
    } else {
        cout << "Student not found." << endl;
    }
}

void addComplaint(const string& complaint) {
    complaints.push(complaint);
}

void resolveComplaint() {
    if (!complaints.empty()) {
        cout << "Resolving complaint: " << complaints.front() << endl;
        complaints.pop();
    } else {
        cout << "No complaints to resolve." << endl;
    }
}

void saveStudents(ofstream& out) {
    for (list<Student>::const_iterator it = students.begin(); it != students.end(); ++it) {
        out << it->name << " " << it->age << " " << it->rollNumber << " " 
            << it->roomNumber << " " << it->feeBalance << endl;
    }
}

void loadStudents(ifstream& in) {
    string name, rollNumber, roomNumber;
    int age;
    double feeBalance;
    while (in >> name >> age >> rollNumber >> roomNumber >> feeBalance) {
        addStudent(name, age, rollNumber, roomNumber, feeBalance);
    }
}

int main() {
    int choice;
    string name, rollNumber, roomNumber, complaint;
    int age;
    double feeBalance, amount;

    // Load students from file
    ifstream infile("students.txt");
    if (infile.is_open()) {
        loadStudents(infile);
        infile.close();
    }

    while (true) {
        cout << "------Hostel Management System------\n";
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Search Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Add Room\n";
        cout << "6. Allocate Room\n";
        cout << "7. Pay Fee\n";
        cout << "8. Add Complaint\n";
        cout << "9. Resolve Complaint\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: 
            cout << "Enter name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter age: ";
            cin >> age;
            cout << "Enter roll number: ";
            cin >> rollNumber;
            cout << "Enter room number: ";
            cin >> roomNumber;
            cout << "Enter fee balance: ";
            cin >> feeBalance;
            addStudent(name, age, rollNumber, roomNumber, feeBalance);
            cout << "Student added successfully." << endl;
            break;
        case 2: 
            viewStudents();
            break;
        case 3: 
            cout << "Enter roll number to search: ";
            cin >> rollNumber;
            if (Student* student = searchStudent(rollNumber)) {
                cout << "Name: " << student->name << ", Age: " << student->age 
                     << ", Roll Number: " << student->rollNumber << ", Room Number: " << student->roomNumber
                     << ", Fee Balance: " << student->feeBalance << endl;
            } else {
                cout << "Student not found." << endl;
            }
            break;
        case 4: 
            cout << "Enter roll number to delete: ";
            cin >> rollNumber;
            deleteStudent(rollNumber);
            cout << "Student deleted successfully." << endl;
            break;
        case 5: 
            cout << "Enter room number to add: ";
            cin >> roomNumber;
            addRoom(roomNumber);
            cout << "Room added successfully." << endl;
            break;
        case 6: 
            cout << "Enter room number to allocate: ";
            cin >> roomNumber;
            cout << "Enter roll number to allocate room: ";
            cin >> rollNumber;
            allocateRoom(roomNumber, rollNumber);
            break;
        case 7: 
            cout << "Enter roll number to pay fee: ";
            cin >> rollNumber;
            cout << "Enter amount to pay: ";
            cin >> amount;
            payFee(rollNumber, amount);
            break;
        case 8: 
            cout << "Enter complaint: ";
            cin.ignore();
            getline(cin, complaint);
            addComplaint(complaint);
            cout << "Complaint added successfully." << endl;
            break;
        case 9: 
            resolveComplaint();
            break;
        case 10: 
            // Save students to file
            {
                ofstream outfile("students.txt");
                if (outfile.is_open()) {
                    saveStudents(outfile);
                    outfile.close();
                }
            }
            cout << "THANK YOU..." << endl;
            return 0;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
