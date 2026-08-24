#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "globals.h"
#include "customer.h"

using namespace std;

string generateNextCustomerID(const vector<Customer> &customers){
    int maxID = 0;
    for (const Customer& cust : customers){ 
        if(cust.id.length() > 1 && cust.id[0] == 'C'){
            int currentID = stoi(cust.id.substr(1)); 
            if (currentID > maxID) { 
                maxID = currentID;  
            }
        }
    }
    stringstream ss;
    ss << "C" << setfill('0') << setw(3) << (maxID + 1); 
    return ss.str();
}

Customer* findCustomerById(vector<Customer>& customers, const string& customerID) {
    for (Customer& cust : customers) {
        if (cust.id == customerID) {
            return &cust;
        }
    }
    return nullptr;
}

string toLower(string str){
    for(int i = 0; i < str.length(); i++){
        str[i] = tolower(str[i]);
    }
    return str;
}

bool isUsernameTaken(const vector<CustomerLogin>& logins, const string& username){
    string usernameLower = toLower(username);
    for (const CustomerLogin& login : logins){
        if (toLower(login.username) == usernameLower){
            return true;
        }
    }
    return false;
}

bool registerCustomer(vector<Customer>& customers, vector<CustomerLogin>& logins,
                      const string& name, const string& phone, const string& email,
                      const string& username, const string& password){
    if (isUsernameTaken(logins, username)){
        cout << "Error: Username '" << username << "' is already taken." << endl;
        return false;
    }

    string generateNextCustID = generateNextCustomerID(customers);
    Customer newCust = {generateNextCustID, name, phone, email, "Regular", 0, true};
    CustomerLogin newLogin = {generateNextCustID, username, password};

    customers.push_back(newCust);
    logins.push_back(newLogin);

    cout << "Customer '" << name << "' (ID: " << generateNextCustID << ") registered successfully!" << endl;
    return true;
}

bool isNumeric(const string &str){
    for (char c : str){
        if (!isdigit(c))
            return false;
    }
    return true;
}


void registerCustomerPrompt(vector<Customer>& customers, vector<CustomerLogin>& logins){
    string name, phone, email, username, password;

    cout << "\n+-------------------------------------------+\n";
    cout << "|            REGISTER NEW CUSTOMER          |\n";
    cout << "+-------------------------------------------+\n";

    // Collect name
    cout << "Enter Full Name -> ";
    getline(cin >> ws, name); 
    if (name.empty()){
        cout << "Error: Name cannot be empty.\n";
        return;
    }

    // Collect phone
    cout << "  Enter Phone Number -> ";
    getline(cin >> ws, phone);
    if (phone.empty()){
        cout << "Error: Phone number cannot be empty.\n";
        return;
    }
    if (!isNumeric(phone)){
        cout << "Error: Invalid phone number format.\n";
        return;
    }

    // Collect email
    cout << "  Enter Email -> ";
    getline(cin >> ws, email);
    if (email.empty()){
        cout << "Error: Email cannot be empty.\n";
        return;
    }

    // Collect username
    cout << "  Enter Username -> ";
    getline(cin >> ws, username);
    if (username.empty()){
        cout << "Error: Username cannot be empty.\n";
        return;
    }

    // Collect password
    cout << "  Enter Password -> ";
    getline(cin >> ws, password);
    if (password.empty()){
        cout << "Error: Password cannot be empty.\n";
        return;
    }

    registerCustomer(customers, logins, name, phone, email, username, password);
}

void viewAllCustomer(const vector<Customer>& customers){
    if (customers.empty()){
        cout << "Error: No customers found." << endl;
        return;
    }

    cout << "\n  +---------------------------------------------------------------------------------------------------------+" << endl;
    cout << "  |                                            CUSTOMER LIST                                                |" << endl;
    cout << "  +------+--------------------+--------------+-------------------------+---------+---------------+----------+" << endl;
    cout << "  | ID   | Name               | Phone        | Email                   | Tier    | Total Rentals | Status   |" << endl;
    cout << "  +------+--------------------+--------------+-------------------------+---------+---------------+----------+" << endl;
    for (const auto& cust : customers) {
        cout << "  | " << left << setw(4) << cust.id
             << " | " << left << setw(18) << (cust.name.length() > 18 ? cust.name.substr(0, 15) + "..." : cust.name)
             << " | " << left << setw(12) << cust.phone
             << " | " << left << setw(23) << (cust.email.length() > 23 ? cust.email.substr(0, 20) + "..." : cust.email)
             << " | " << left << setw(7) << cust.memberType
             << " | " << right << setw(13) << cust.totalRentals
             << " | " << left << setw(8) << (cust.isActive ? "Active" : "Inactive") << " |" << endl;
            }
            cout << "  +------+--------------------+--------------+-------------------------+---------+---------------+----------+" << endl;
}

bool updateCustomerProfile(vector<Customer>& customers, const string& targetID,
                         const string& name, const string& phone, const string& email,
                         const string& tier, bool isActive) {
    Customer* cust = findCustomerById(customers, targetID);
    if (cust != nullptr){
        cust->name = name;
        cust->phone = phone;
        cust->email = email;
        cust->memberType = tier;
        cust->isActive = isActive;
        return true;
    }
    return false;
}

void updateCustomerPrompt(vector<Customer>& customers, bool isStaff){
    string targetID;
    cout << "\n Enter target Customer ID -> ";
    cin >> targetID;

    Customer* cust = findCustomerById(customers, targetID);
    if (cust == nullptr){
        cout << "Error: Customer ID not found" << endl;
        return;
    }

    int choice;
    do{
        cout << "\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  |            UPDATE CUSTOMER PROFILE          |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  | " << left << setw(41) << "Welcome, Placeholder" << " |\n";
        cout << "  | " << left << setw(41) << "Role -> Placeholder" << " |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "   1. Update Name\n";
        cout << "   2. Update Phone\n";
        cout << "   3. Update Email\n";
        cout << "   4. Update Tier\n";
        cout << "   5. Update Status\n";
        cout << "   0. Back\n";
        cout << "  ---------------------------------------------\n";
        cout << "  Choice -> ";
        cin >> choice;

        if (cin.fail()){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Invalid input. Please enter a number (0-5).\n";
            continue;
        }

        if (choice == 1){
            cout << "Enter Name -> ";
            getline(cin >> ws, cust->name);
            if (cust->name.empty()){
                cout << "Error: Name cannot be empty.\n";
                return;
            }
            cout << "Name updated successfully.\n";
        }
        else if (choice == 2){
            cout << "Enter Phone -> ";
            getline(cin >> ws, cust->phone);
            if (cust->phone.empty()){
                cout << "Error: Phone number cannot be empty.\n";
                return;
            }
            cout << "Phone number updated successfully.\n";
        }
        else if (choice == 3){
            cout << "Enter Email -> ";
            getline(cin >> ws, cust->email);
            if (cust->email.empty()){
                cout << "Error: Email cannot be empty.\n";
                return;
            }
            cout << "Email updated successfully.\n";
        }
        else if (choice == 4){
            cout << "Enter Tier -> ";
            getline(cin >> ws, cust->memberType);
            if (cust->memberType.empty()){
                cout << "Error: Tier cannot be empty.\n";
                return;
            }
            cout << "Tier updated successfully.\n";
        }
        else if (choice == 5){
            char activeChar;
            cout << "Enter Status (Y/N) -> ";
            cin >> activeChar;
            if (tolower(activeChar) == 'y'){
                cust->isActive = true;
            }
            else if (tolower(activeChar) == 'n'){
                cust->isActive = false;
            }
            else {
                cout << "Error: Invalid input. Please enter Y or N.\n";
                return;
            }
            cout << "Status updated successfully.\n";
        }
    } while (choice != 0);
}

void customerManagementMenu(vector<Customer>& customers, vector<CustomerLogin>& logins, bool isStaff){
    int choice;

    do{
        cout << "\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  |            CUSTOMER MANAGEMENT              |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "   1. Register New Customer\n";
        cout << "   2. View All Customers\n";
        cout << "   3. Search Customer\n";
        cout << "   4. Filter Customer by Tier\n";
        cout << "   5. Update Customer Profile\n";
        cout << "   6. Deactivate Customer\n";
        cout << "   0. Back to Main Menu\n";
        cout << "  ---------------------------------------------\n";
        cout << "  Choice -> ";
        cin >> choice;

        if(choice != 1){
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\n  Error: Invalid input. Please enter a number (0-6).\n";
            continue;
        }

        switch(choice){
            case 1:
                registerCustomerPrompt(customers, logins);
                break;
            case 2:
                viewAllCustomer(customers);
                break;
            case 3:
                // searchCustomerPrompt(customers, currentUserID, isStaff);
                break;
            case 4:
                // filterCustomerPrompt(customers, currentUserID, isStaff);
                break;
            case 5:
                updateCustomerPrompt(customers, isStaff);
                break;
            case 6:
                // deactivateCustomerPrompt(customers, currentUserID, isStaff);
                break;
            case 0:
                cout << "\n  Returning to Main Menu...\n";
                break;
            default:
                cout << "\n  [!] Invalid choice. Please select an option between 0 and 6.\n";
        }
    } while(choice != 0);
}