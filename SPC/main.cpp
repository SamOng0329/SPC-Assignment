#include <iostream>
#include <iomanip>
#include <vector>
#include "globals.h"
#include "booking.h"
#include "bicycle.h"
#include "payment.h"
#include "customer.h"

using namespace std;


// --- Function Prototypes ---
void displayGateway();
void loadSampleData(vector<Customer> &customers, vector<CustomerLogin> &customerLogins, vector<Bicycle> &bicycles, vector<Staff> &staffList);
Customer customerLogin(vector<Customer>& customers, vector<CustomerLogin>& customerLogins);   
Staff loginStaff(const vector<Staff> &staffList);

void runStaffMenu(const Staff& staff, vector<Customer>& customers,
	vector<CustomerLogin>& customerLogins,
	vector<Bicycle>& bicycles, vector<Booking>& bookings,
	vector<Payment>& payments);

void runCustomerMenu(Customer& customer, vector<Customer>& customers, 
	vector<Bicycle>& bicycles, vector<Booking>& bookings, vector<Payment>& payments);


// --- Main Function ---

int main() {
	vector<Staff> staffList;
	vector<Customer> customers;
	vector<CustomerLogin> customerLogins;
	vector<Bicycle> bicycles;
	vector<Booking> bookings;
	vector<Payment> payments;

	loadSampleData(customers, customerLogins, bicycles, staffList);

	// displayLogo();
	cout << "\n  Welcome to Bicycle Rental Management System!" << endl;

	int gatewayChoice;
	do {
		displayGateway();
		cout << "  Select option -> ";
		cin >> gatewayChoice;

		if (gatewayChoice == 1) {
			Staff activeStaff = loginStaff(staffList);
			if(activeStaff.id != "") {
				runStaffMenu(activeStaff, customers, customerLogins, bicycles, bookings, payments);
			}
		}
		else if (gatewayChoice == 2) {
			Customer activeCustomer = customerLogin(customers, customerLogins);
			if (activeCustomer.id != "") {
				runCustomerMenu(activeCustomer, customers, bicycles, bookings, payments);
			}
		}
		else if (gatewayChoice == 3) {
			registerCustomerPrompt(customers, customerLogins);
		}
		else if (gatewayChoice == 0) {
			cout << "\n  Exiting application... Have a great day!" << endl;
		}
		else {
			cout << "\n  [!] Invalid option. Please try again." << endl;
		}
	} while (gatewayChoice != 0);


	return 0;
}

void displayGateway() {
	cout << endl;
	cout << "  +----------------------------------------------+" << endl;
	cout << "  |       BICYCLE RENTAL MANAGEMENT SYSTEM       |" << endl;
	cout << "  +----------------------------------------------+" << endl;
	cout << "  |   1. Staff Login                             |" << endl;
	cout << "  |   2. Customer Login                          |" << endl;
	cout << "  |   3. Customer Register                       |" << endl;
	cout << "  |   0. Exit Application                        |" << endl;
	cout << "  +----------------------------------------------+" << endl;
}

Staff loginStaff(const vector<Staff> &staffList) {
	Staff emptyStaff = { "", "", "", "" };
	string username, password;
	cout << "\n\n  +------------------------------------+" << endl;
	cout << "  |        STAFF AUTHENTICATION        |" << endl;
	cout << "  +------------------------------------+" << endl;
	cout << "  Enter Username -> ";
	cin >> username;
	cout << "  Enter Password -> ";
	cin >> password;

	for(int i = 0; i < staffList.size(); ++i) {
		if(staffList[i].username == username && staffList[i].password == password) {
			cout << "\n  [+] Login successful." << endl;
			return staffList[i];
		}
	}

	cout << "\n  [!] Invalid credentials. Please try again." << endl << endl;
	return emptyStaff;
}

Customer customerLogin(vector<Customer>& customers, vector<CustomerLogin>& customerLogins) {
	Customer emptyCustomer = { "", "", "", "", "", 0, false };
	string username, password;
	cout << "\n\n  +-------------------------------------+" << endl;
	cout << "  |       CUSTOMER AUTHENTICATION       |" << endl;
	cout << "  +-------------------------------------+" << endl;
	cout << "  Enter Username -> ";
	cin >> username;
	cout << "  Enter Password -> ";
	cin >> password;

	for (int i = 0; i < customerLogins.size(); ++i) {
		if (customerLogins[i].username == username && customerLogins[i].password == password) {
			// Credentials matched, now find the linked Customer profile
			string custID = customerLogins[i].customerID;
			for (int j = 0; j < customers.size(); ++j) {
				if (customers[j].id == custID) {
					cout << "\n  [+] Login successful." << endl;
					return customers[j];
				}
			}
		}
	}

	cout << "\n  [!] Invalid credentials. Please try again." << endl << endl;
	return emptyCustomer;
}

void runStaffMenu(const Staff& staff, vector<Customer>& customers, vector<CustomerLogin>& customerLogins, vector<Bicycle>& bicycles, vector<Booking>& bookings, vector<Payment>& payments) {
	int choice;
	do {
		cout << "\n\n  +----------------------------------------------+" << endl;
		cout << "  |                 STAFF PORTAL                 |" << endl;
		cout << "  +----------------------------------------------+" << endl;
		cout << "  |" << left << setw(45) << " Welcome, " + staff.name << " |" << endl;
		cout << "  |" << left << setw(45) << " Role -> Staff" << " |" << endl;
		cout << "  +----------------------------------------------+" << endl;
		cout << "   1. Customer Management" << endl;
		cout << "   2. Bicycle Registration & Inventory Tracking" << endl;
		cout << "   3. Rental Booking & Schedule Management" << endl;
		cout << "   4. Billing & Statistics Reporting" << endl;
		cout << "   0. Logout" << endl;
		cout << "  ------------------------------------------------" << endl;
		cout << "  Choice -> ";
		cin >> choice;

		switch(choice) {
			case 1:
				// Call customer management submenu
				customerManagementMenu(customers, customerLogins, true);
				break;
			case 2:
				bicycleMenu(bicycles, true);
				// Call bicycle management submenu
				break;
			case 3:
				bookingMenu(staff.name, staff.id, "", true, bookings, bicycles, customers);
				break;
			case 4:
				billingMenu(bookings, payments, customers, bicycles, true);
				break;
			case 0:
				cout << "\n  Logging out..." << endl << endl;
				break;
			default:
				cout << "\n  [!] Invalid option. Please try again." << endl << endl;
		}
	} while (choice != 0);
}

void runCustomerMenu(Customer& cust, vector<Customer>& customers, vector<Bicycle>& bicycles, vector<Booking>& bookings, vector<Payment>& payments) {

	int choice;
	do {
		cout << "\n\n  +---------------------------------------------+" << endl;
		cout << "  |               CUSTOMER PORTAL               |" << endl;
		cout << "  +---------------------------------------------+" << endl;
		cout << "  |" << left << setw(44) << " Welcome, " + cust.name << " |" << endl;
		cout << "  |" << left << setw(44) << " Tier -> "  + cust.memberType << " |" << endl;
		cout << "  +---------------------------------------------+" << endl;
		cout << "   1. Browse Available Bicycles" << endl;
		cout << "   2. Booking & Schedule Services" << endl;
		cout << "   3. Billing & Payment Gateway" << endl;
		cout << "   4. Update Profile" << endl;
		cout << "   0. Logout" << endl;
		cout << "  -----------------------------------------------" << endl;
		cout << "  Choice -> ";
		cin >> choice;

		switch (choice) {
		case 1:
			bicycleMenu(bicycles, false);
			// Inventory (Bicycle Availability Check)
			break;
		case 2:
			bookingMenu(cust.name, cust.id, cust.memberType, false, bookings, bicycles, customers);
			break;
		case 3:
			billingMenu(bookings, payments, customers, bicycles, false);
			break;
		case 4:
			customerUpdateProfilePrompt(cust, customers);
			break;
		case 0:
			cout << "\n  Logging out..." << endl << endl;
			break;
		default:
			cout << "\n  [!] Invalid option. Please try again." << endl << endl;
		}
	} while (choice != 0);
}

void loadSampleData(vector<Customer> &customers, vector<CustomerLogin> &customerLogins, vector<Bicycle> &bicycles, vector<Staff> &staffList){
	customers.push_back({"C001", "Tan Mei Ling", "0123456789", "tanml@gmail.com", "Regular", 3, true}); 
	customers.push_back({"C002", "Chan Jun Jie", "0123456789", "chanjj@mail.com", "Premium", 3, true});

	customerLogins.push_back({"C001", "tanml", "meiling123"});
	customerLogins.push_back({"C002", "chanjj", "junjie456"});
 
    bicycles.push_back({"BK001", "Trek", "FX2", "Hybrid", "Good", 8.00, true});
    bicycles.push_back({"BK002", "Giant", "Talon", "Mountain", "Good", 10.00, true});
    bicycles.push_back({"BK003", "Specialized", "Allez", "Road", "Maintenance", 12.00, false});

	staffList.push_back({"S001", "Chong Wen Chun", "chongwc", "chongwc123"});
	staffList.push_back({"S002", "Natalie Wong Eei Qi", "natalie", "natalie456"});
}