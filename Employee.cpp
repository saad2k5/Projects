#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int stringToInt(const string& s) {
    int result = 0;
    int sign = 1;
    size_t i = 0;
    if (s[0] == '-') {
        sign = -1;
        i = 1;
    }
    for (; i < s.length(); ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            result = result * 10 + (s[i] - '0');
        } else {
            break;
        }
    }
    return sign * result;
}
class Employee {
public:
    int EmployeeID;
    string FirstName;
    string LastName;
    string Department;
    int Salary;
    Employee* left;
    Employee* right;
    Employee(int id, string fn, string ln, string dept, int sal) : EmployeeID(id), FirstName(fn), LastName(ln), Department(dept), Salary(sal), left(nullptr), right(nullptr) {}
    void displayRecord() const {
        cout << EmployeeID << "\t"
             << FirstName << " " << LastName << "\t"
             << Department << "\t"
             << Salary << endl;
    }
};
class EmployeeDirectory {
private:
    Employee* root;
    Employee* insert(Employee* node, Employee* newEmployee) {
        if (node == nullptr) {
            return newEmployee;
        }
        if (newEmployee->EmployeeID < node->EmployeeID) {
            node->left = insert(node->left, newEmployee);
        } else if (newEmployee->EmployeeID > node->EmployeeID) {
            node->right = insert(node->right, newEmployee);
        }
        return node;
    }
    Employee* search(Employee* node, int id) const {
        if (node == nullptr || node->EmployeeID == id) {
            return node;
        }

        if (id < node->EmployeeID) {
            return search(node->left, id);
        } else {
            return search(node->right, id);
        }
    }
    Employee* findMin(Employee* node) const {
        Employee* current = node;
        while (current && current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    Employee* deleteNode(Employee* node, int id) {
        if (node == nullptr) return node;
        if (id < node->EmployeeID) {
            node->left = deleteNode(node->left, id);
        } else if (id > node->EmployeeID) {
            node->right = deleteNode(node->right, id);
        } else {
            if (node->left == nullptr) {
                Employee* temp = node->right;
                delete node;
                return temp;
            } else if (node->right == nullptr) {
                Employee* temp = node->left;
                delete node;
                return temp;
            }
            Employee* temp = findMin(node->right);
            node->EmployeeID = temp->EmployeeID;
            node->FirstName = temp->FirstName;
            node->LastName = temp->LastName;
            node->Department = temp->Department;
            node->Salary = temp->Salary;
            node->right = deleteNode(node->right, temp->EmployeeID);
        }
        return node;
    }
    void InOrder(Employee* node) const {
        if (node != nullptr) {
            InOrder(node->left);
            node->displayRecord();
            InOrder(node->right);
        }
    }
    void reverseInorder(Employee* node) const {
        if (node != nullptr) {
            reverseInorder(node->right);
            node->displayRecord();
            reverseInorder(node->left);
        }
    }
    void destroyTree(Employee* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }
    void loadEmployeesFromFile(const string& filename) {
        ifstream file(filename);
        string line;
                if (!file.is_open()) {
            cout << "Error: Could not open file!!! " << filename << endl;
            return;
        }
        while (getline(file, line)) {
            size_t pos_start = 0;
            size_t pos_end = 0;
            string tokens[5];
            int token_count = 0;
            while ((pos_end = line.find(' ', pos_start)) != string::npos && token_count < 4) {
                tokens[token_count++] = line.substr(pos_start, pos_end - pos_start);
                pos_start = pos_end + 1;
            }
            if (token_count == 4) {
                 tokens[token_count++] = line.substr(pos_start); // Last token
            }
            if (token_count == 5) {
                int id = stringToInt(tokens[0]);
                string firstName = tokens[1];
                string lastName = tokens[2];
                string department = tokens[3];
                int salary = stringToInt(tokens[4]);
                Employee* newEmp = new Employee(id, firstName, lastName, department, salary);
                root = insert(root, newEmp);
            }
        }
        file.close();
        cout << "\nDirectory successfully loaded from " << filename << endl;
    }
public:
    EmployeeDirectory() : root(nullptr) {
        loadEmployeesFromFile("employee.txt"); 
    }
    ~EmployeeDirectory() {
        destroyTree(root);
    }
    void printHeader() const {
        cout << "--------------------------------------------------" << endl;
        cout << "ID\tName\t\tDepartment\tSalary" << endl;
        cout << "--------------------------------------------------" << endl;
    }
    void addEmployee(int id, const string& fn, const string& ln, const string& dept, int sal) {
        Employee* newEmp = new Employee(id, fn, ln, dept, sal);
        root = insert(root, newEmp);
        cout << "\nEmployee " << id << " added." << endl;
    }
    void deleteEmployee(int id) {
        if (search(root, id) == nullptr) {
            cout << "\nError: Employee ID " << id << " not found for deletion." << endl;
            return;
        }
        root = deleteNode(root, id);
        cout << "\nEmployee " << id << " deleted." << endl;
    }
    void searchAndDisplay(int id) const {
        Employee* emp = search(root, id);
        if (emp != nullptr) {
            cout << "\nEmployee Found (ID: " << id << "):" << endl;
            printHeader();
            emp->displayRecord();
        } else {
            cout << "\nError: Employee ID " << id << " not found." << endl;
        }
    }
    void displayAllAscending() const {
        cout << "\nEmployees (Ascending by ID):" << endl;
        printHeader();
        InOrder(root);
    }
    void displayAllDescending() const {
        cout << "\nEmployees (Descending by ID):" << endl;
        printHeader();
        reverseInorder(root);
    }
};
int main() {
    EmployeeDirectory directory;
    directory.addEmployee(1230, "Ali", "Asad", "Retail", 20000);
    directory.addEmployee(1240, "Hareem", "Khan", "IT", 50000);
    directory.addEmployee(1237, "Ammara", "Usman", "Sales", 45000);
    directory.searchAndDisplay(1234);
    directory.searchAndDisplay(9999); 
    directory.displayAllAscending();
    directory.displayAllDescending();
    directory.deleteEmployee(1233); 
    directory.displayAllAscending();
    return 0;
}