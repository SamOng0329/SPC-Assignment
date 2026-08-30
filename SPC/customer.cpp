#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>
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
                      const string& username, const string& password, bool isStaff){
    if (isUsernameTaken(logins, username)){
        cout << "  [!] Error: Username '" << username << "' is already taken." << endl;
        return false;
    }

    string generateNextCustID = generateNextCustomerID(customers);
    Customer newCust = {generateNextCustID, name, phone, email, "Regular", 0, true};
    CustomerLogin newLogin = {generateNextCustID, username, password};

    customers.push_back(newCust);
    logins.push_back(newLogin);

    if (isStaff) {
        cout << "  [+] Customer '" << name << "' (ID: " << generateNextCustID << ") registered successfully!" << endl;
    } else {
        cout << "  [+] Customer '" << name << "' registered successfully!" << endl;
    }
    return true;
}

bool isNumeric(const string &str){
    for (char c : str){
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool isValidEmail(const string& email) {
    regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email, pattern);
}

string formatPhoneNumber(const string& phone) {
    if (phone.length() == 10) {
        return phone.substr(0, 3) + " " + phone.substr(3, 3) + " " + phone.substr(6, 4);
    } else if (phone.length() == 11) {
        return phone.substr(0, 3) + " " + phone.substr(3, 4) + " " + phone.substr(7, 4);
    }
    return phone;
}


void registerCustomerPrompt(vector<Customer>& customers, vector<CustomerLogin>& logins, bool isStaff){
    string name, phone, email, username, password;

    // Clean buffer once at the start of the prompt
    if (cin.peek() == '\n') {
        cin.ignore();
    }

    cout << "\n  +-------------------------------------------+\n";
    cout << "  |            REGISTER NEW CUSTOMER          |\n";
    cout << "  +-------------------------------------------+\n";

    // Collect name
    cout << "  Enter Full Name -> ";
    getline(cin, name); 
    if (name.empty()){
        cout << "  [!] Error: Name cannot be empty\n";
        return;
    }

    // Collect phone
    cout << "  Enter Phone Number -> ";
    getline(cin, phone);
    if (phone.empty()){
        cout << "  [!] Error: Phone number cannot be empty.\n";
        return;
    }
    if (!isNumeric(phone)){
        cout << "  [!]Error: Invalid phone number format.\n";
        return;
    }

    // Collect email
    cout << "  Enter Email -> ";
    getline(cin, email);
    if (email.empty()){
        cout << "  [!] Error: Email cannot be empty.\n";
        return;
    }
    if (!isValidEmail(email)) {
        cout << "  [!] Error: Invalid email format.";
        return;
    }

    // Collect username
    cout << "  Enter Username -> ";
    getline(cin, username);
    if (username.empty()){
        cout << "  [!] Error: Username cannot be empty.\n";
        return;
    }

    // Collect password
    cout << "  Enter Password -> ";
    getline(cin, password);
    if (password.empty()){
        cout << "  [!] Error: Password cannot be empty.\n";
        return;
    }

    registerCustomer(customers, logins, name, phone, email, username, password, isStaff);
}

void viewAllCustomer(const vector<Customer>& customers){
    if (customers.empty()){
        cout << "  [!] Error: No customers found." << endl;
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
             << " | " << left << setw(12) << formatPhoneNumber(cust.phone)
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
    cout << "\n  Enter target Customer ID -> ";
    cin >> targetID;

    int foundIndex = -1;
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].id == targetID) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1){
        cout << "  [!] Error: Customer ID not found" << endl;
        return;
    }

    Customer& cust = customers[foundIndex];

    int choice;
    do{
        cout << "\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  |             CUSTOMER DETAILS              |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  | ID            : " << left << setw(26) << cust.id << "|\n";
        cout << "  | Name          : " << left << setw(26) << (cust.name.length() > 26 ? cust.name.substr(0, 23) + "..." : cust.name) << "|\n";
        cout << "  | Phone         : " << left << setw(26) << formatPhoneNumber(cust.phone) << "|\n";
        cout << "  | Email         : " << left << setw(26) << (cust.email.length() > 26 ? cust.email.substr(0, 23) + "..." : cust.email) << "|\n";
        cout << "  | Member Tier   : " << left << setw(26) << cust.memberType << "|\n";
        cout << "  | Total Rentals : " << left << setw(26) << cust.totalRentals << "|\n";
        cout << "  | Account Status: " << left << setw(26) << (cust.isActive ? "Active" : "Inactive") << "|\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  |            UPDATE CUSTOMER PROFILE        |\n";
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
            cout << "  [!] Error: Invalid input. Please enter a number (0-5).\n";
            continue;
        }

        // Clean buffer once since cin >> choice leaves a newline
        if (cin.peek() == '\n') {
            cin.ignore();
        }

        switch(choice){
            case 1: {
                string tempName;
                cout << "  Enter Name -> ";
                getline(cin, tempName);
                if (tempName.empty()){
                    cout << "  [!] Error: Name cannot be empty.\n";
                    break;
                }
                updateCustomerProfile(customers, targetID, tempName, cust.phone,
                                       cust.email, cust.memberType, cust.isActive);
                cout << "  [+] Name updated successfully.\n";
                break;
            }
            case 2: {
                string tempPhone;
                cout << "  Enter Phone -> ";
                getline(cin, tempPhone);
                if (tempPhone.empty()){
                    cout << "  [!] Error: Phone number cannot be empty.\n";
                    break;
                }
                updateCustomerProfile(customers, targetID, cust.name, tempPhone,
                                       cust.email, cust.memberType, cust.isActive);
                cout << "  [+] Phone number updated successfully.\n";
                break;
            }
            case 3: {
                string tempEmail;
                cout << "  Enter Email -> ";
                getline(cin, tempEmail);
                if (tempEmail.empty()){
                    cout << "  [!] Error: Email cannot be empty.\n";
                    break;
                }
                if (!isValidEmail(tempEmail)) {
                    cout << "  [!] Error: Invalid Email format.\n";
                    break;
                }
                updateCustomerProfile(customers, targetID, cust.name, cust.phone,
                                       tempEmail, cust.memberType, cust.isActive);
                cout << "  [+] Email updated successfully.\n";
                break;
            }
            case 4: {
                string tempTier;
                cout << "  Enter Tier -> ";
                getline(cin, tempTier);
                if (tempTier.empty()){
                    cout << "  [!] Error: Tier cannot be empty.\n";
                    break;
                }
                updateCustomerProfile(customers, targetID, cust.name, cust.phone,
                                       cust.email, tempTier, cust.isActive);
                cout << "  [+] Tier updated successfully.\n";
                break;
            }
            case 5: {
                char activeChar;
                cout << "  Enter Status (Y/N) -> ";
                cin >> activeChar;
                bool tempActive;
                if (tolower(activeChar) == 'y'){
                    tempActive = true;
                } else if (tolower(activeChar) == 'n'){
                    tempActive = false;
                } else {
                    cout << "  [!] Error: Invalid input. Please enter Y or N.\n";
                    break;
                }
                updateCustomerProfile(customers, targetID, cust.name, cust.phone,
                                       cust.email, cust.memberType, tempActive);
                cout << "  [+] Status updated successfully.\n";
                break;
            }
            case 0:
                break;
            default:
                cout << "  [!] Error: Invalid choice. Please enter a number (0-5).\n";
        }
    } while (choice != 0);
}

void customerUpdateProfilePrompt(Customer& cust, vector<Customer>& customers) {
    int foundIndex = -1;
    for (int i = 0; i < customers.size(); ++i) {
        if (customers[i].id == cust.id) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        cout << "\n  [!] Error: Customer record not found." << endl;
        return;
    }

    Customer& currentCust = customers[foundIndex];

    int choice;
    do {
        cout << "\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  |             CUSTOMER DETAILS              |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  | ID            : " << left << setw(26) << currentCust.id << "|\n";
        cout << "  | Name          : " << left << setw(26) << (currentCust.name.length() > 26 ? currentCust.name.substr(0, 23) + "..." : currentCust.name) << "|\n";
        cout << "  | Phone         : " << left << setw(26) << formatPhoneNumber(currentCust.phone) << "|\n";
        cout << "  | Email         : " << left << setw(26) << (currentCust.email.length() > 26 ? currentCust.email.substr(0, 23) + "..." : currentCust.email) << "|\n";
        cout << "  | Member Tier   : " << left << setw(26) << currentCust.memberType << "|\n";
        cout << "  | Total Rentals : " << left << setw(26) << currentCust.totalRentals << "|\n";
        cout << "  | Account Status: " << left << setw(26) << (currentCust.isActive ? "Active" : "Inactive") << "|\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  |            UPDATE YOUR PROFILE            |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "   1. Update Name\n";
        cout << "   2. Update Phone Number\n";
        cout << "   3. Update Email\n";
        cout << "   0. Back to Customer Portal\n";
        cout << "  ---------------------------------------------\n";
        cout << "  Choice -> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  [!] Error: Invalid input. Please enter a number (0-3).\n";
            continue;
        }

        if (cin.peek() == '\n') {
            cin.ignore();
        }

        switch (choice) {
            case 1: {
                string tempName;
                cout << "  Enter New Name -> ";
                getline(cin, tempName);
                if (tempName.empty()) {
                    cout << "  [!] Error: Name cannot be empty.\n";
                    break;
                }
                updateCustomerProfile(customers, currentCust.id, tempName, currentCust.phone,
                                       currentCust.email, currentCust.memberType, currentCust.isActive);
                cust.name = tempName;
                cout << "  [+] Name updated successfully.\n";
                break;
            }
            case 2: {
                string tempPhone;
                cout << "  Enter New Phone Number -> ";
                getline(cin, tempPhone);
                if (tempPhone.empty()) {
                    cout << "  [!] Error: Phone number cannot be empty.\n";
                    break;
                }
                if (!isNumeric(tempPhone)) {
                    cout << "  [!] Error: Invalid phone number format (numbers only).\n";
                    break;
                }
                updateCustomerProfile(customers, currentCust.id, currentCust.name, tempPhone,
                                       currentCust.email, currentCust.memberType, currentCust.isActive);
                cust.phone = tempPhone;
                cout << "  [+] Phone number updated successfully.\n";
                break;
            }
            case 3: {
                string tempEmail;
                cout << "  Enter New Email -> ";
                getline(cin, tempEmail);
                if (tempEmail.empty()) {
                    cout << "  [!] Error: Email cannot be empty.\n";
                    break;
                }
                if (!isValidEmail(tempEmail)) {
                    cout << "  [!] Error: Invalid Email format.\n";
                    break;
                }
                updateCustomerProfile(customers, currentCust.id, currentCust.name, currentCust.phone,
                                       tempEmail, currentCust.memberType, currentCust.isActive);
                cust.email = tempEmail;
                cout << "  [+] Email updated successfully.\n";
                break;
            }
            case 0:
                cout << "\n  Returning to Customer Portal...\n";
                break;
            default:
                cout << "  [!] Error: Invalid choice. Please enter a number between 0 and 3.\n";
        }
    } while (choice != 0);
}

void searchCustomers(const vector<Customer>& customers, const string& query){
    vector<Customer> results;

    for(const Customer& cust : customers){
     if (toLower(cust.id) == toLower(query) || 
         toLower(cust.name) == toLower(query) || 
         cust.phone == query) {
         results.push_back(cust);
        }
    }

    if (results.empty()){
        cout << "  [!] Error: No customer found!" << endl;
    } else {
        viewAllCustomer(results);
    }
}

void searchCustomerPrompt(const vector<Customer>& customers){
    string query;
    if (cin.peek() == '\n') {
        cin.ignore();
    }
    cout << "  Enter ID, Name, or Phone -> ";
    getline(cin, query);
    searchCustomers(customers, query);
}

bool deleteCustomer(vector<Customer>& customers, vector<CustomerLogin>& logins, const string& targetID){
    Customer* cust = findCustomerById(customers, targetID);
    if (cust == nullptr){
        cout << "  [!] Error: Customer ID not found." << endl;
        return false;
    }

    string deletedName = cust->name;

    for (int i = 0; i < customers.size(); i++){
        if (customers[i].id == targetID){
            customers.erase(customers.begin() + i);
            break;
        }
    }

    for (int i = 0; i < logins.size(); i++){
        if (logins[i].customerID == targetID){
            logins.erase(logins.begin() + i);
            break;
        }
    }

    cout << "  [-] Customer '" << deletedName << "' (ID: " << targetID << ") deleted successfully!" << endl;
    return true;
}

void deleteCustomerPrompt(vector<Customer>& customers, vector<CustomerLogin>& logins){
    string targetID;
    cout << "\n  Enter Customer ID to Delete -> ";
    cin >> targetID;
    deleteCustomer(customers, logins, targetID);
}

void filterCustomerByTier(const vector<Customer>& customers, const string& tier){
    vector<Customer> filteredTierCustomer;
    for(const Customer& cust : customers){
        if (toLower(cust.memberType) == toLower(tier)){
            filteredTierCustomer.push_back(cust);
        }
    }

    if (filteredTierCustomer.empty()){
        cout << "  [! Error: No customer found! " << endl;
    } else {
        viewAllCustomer(filteredTierCustomer);
    }
}

void filterCustomerByTierPrompt(const vector<Customer>& customers){
    int choice;
    cout << "\n  Select Tier to Filter:\n";
    cout << "   1. Regular\n";
    cout << "   2. Premium\n";
    cout << "  Choice -> ";
    cin >> choice;

    if(cin.fail() || choice < 0 || choice > 2){
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "  [!] Error: Invalid input. Please enter a number (1-2).\n";
        return;
    }

    if (choice == 1){
        filterCustomerByTier(customers, "Regular");
    } else if (choice == 2){
        filterCustomerByTier(customers, "Premium");
    }
}

void customerManagementMenu(vector<Customer>& customers, vector<CustomerLogin>& logins, bool isStaff){
    int choice;

    do{
        cout << "\n";
        cout << "  +-------------------------------------------+\n";
        cout << "  |            CUSTOMER MANAGEMENT            |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "   1. Register New Customer\n";
        cout << "   2. View All Customers\n";
        cout << "   3. Update Customer Profile\n";
        cout << "   4. Search Customer\n";
        cout << "   5. Filter Customer by Tier\n";
        cout << "   6. Delete Customer\n";
        cout << "   0. Back to Main Menu\n";
        cout << "  ---------------------------------------------\n";
        cout << "  Choice -> ";
        cin >> choice;

        if(cin.fail() || choice < 0 || choice > 6){
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
            cout << "\n  [!] Error: Invalid input. Please enter a number (0-6).\n";
            continue;
        }

        switch(choice){
            case 1:
                registerCustomerPrompt(customers, logins, isStaff);
                break;
            case 2:
                viewAllCustomer(customers);
                break;
            case 3:
                updateCustomerPrompt(customers, isStaff);
                break;
            case 4:
                searchCustomerPrompt(customers);
                break;
            case 5:
                filterCustomerByTierPrompt(customers);
                break;
            case 6:
                deleteCustomerPrompt(customers, logins);
                break;
            case 0:
                cout << "\n  Returning to Main Menu...\n";
                break;
            default:
                cout << "\n  [!] Invalid choice. Please select an option between 0 and 6.\n";
        }
    } while(choice != 0);
}
