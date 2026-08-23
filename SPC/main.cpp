#include <iostream>
#include <iomanip>
#include <vector>
#include "globals.h"

using namespace std;


// --- Function Prototypes ---
void displayGateway();
Customer customerLogin(vector<Customer>& customers, vector<CustomerLogin>& customerLogins);   

Staff loginStaff();
void runStaffMenu(const Staff& staff, vector<Customer>& customers,
	vector<Bicycle>& bicycles, vector<Booking>& bookings,
	vector<Payment>& payments);

void runCustomerMenu(const Customer& customer, vector<Bicycle>& bicycles,
	vector<Booking>& bookings, vector<Payment>& payments);

const vector<Staff> staffList = {
	{"S001", "Chong Wen Chun", "chongwc", "chongwc123"},
	{"S002", "Natalie Wong Eei Qi", "natalie", "natalie456"}
};


// --- Main Function ---

int main() {
	vector<Customer> customers{
		{"C001", "Tan Mei Ling", "0123456789", "tanml@gmail.com", "Regular", 3, true}
	};

	vector<CustomerLogin> customerLogins{
		{"C001", "tanml", "meiling123"}
	};

	vector<Bicycle> bicycles;
	vector<Booking> bookings;
	vector<Payment> payments;

	// displayLogo();
	cout << "\n  Welcome to Bicycle Rental Management System!" << endl;

	int gatewayChoice;
	do {
		displayGateway();
		cout << "  Select option -> ";
		cin >> gatewayChoice;

		if (gatewayChoice == 1) {
			Staff activeStaff = loginStaff();
			if(activeStaff.id != "") {
				runStaffMenu(activeStaff, customers, bicycles, bookings, payments);
			}
		}
		else if (gatewayChoice == 2) {
			Customer activeCustomer = customerLogin(customers, customerLogins);
			if (activeCustomer.id != "") {
				runCustomerMenu(activeCustomer, bicycles, bookings, payments);
			}
		}
		else if (gatewayChoice == 3) {
			cout << "Customer Register Test" << endl;
			// Customer Register
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

Staff loginStaff() {
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
	cout << "\n\n  +------------------------------------+" << endl;
	cout << "  |       CUSTOMER AUTHENTICATION       |" << endl;
	cout << "  +------------------------------------+" << endl;
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

void runStaffMenu(const Staff& staff, vector<Customer>& customers, vector<Bicycle>& bicycles, vector<Booking>& bookings, vector<Payment>& payments) {
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
				cout << "\n  [Customer Management Module]" << endl;
				// Call customer management submenu
				break;
			case 2:
				cout << "\n  [Bicycle Registration & Inventory Tracking Module]" << endl;
				// Call bicycle management submenu
				break;
			case 3:
				cout << "\n  [Rental Booking & Schedule Management Module]" << endl;
				// Call booking management submenu
				break;
			case 4:
				cout << "\n  [Billing & Statistics Reporting Module]" << endl;
				// Call billing and reporting functions (submenu)
				break;
			case 0:
				cout << "\n  Logging out..." << endl << endl;
				break;
			default:
				cout << "\n  [!] Invalid option. Please try again." << endl;
		}
	} while (choice != 0);
}

void runCustomerMenu(const Customer& cust, vector<Bicycle>& bicycles, vector<Booking>& bookings, vector<Payment>& payments) {

	int choice;
	do {
		cout << "\n\n  +-----------------------------------------+" << endl;
		cout << "  |               CUSTOMER PORTAL            |" << endl;
		cout << "  +------------------------------------------+" << endl;
		cout << "  |" << left << setw(40) << " Welcome, " + cust.name << " |" << endl;
		cout << "  |" << left << setw(40) << " Tier -> "  + cust.memberType << " |" << endl;
		cout << "  +------------------------------------------+" << endl;
		cout << "   1. Browse Available Bicycles" << endl;
		cout << "   2. Make a Rental Booking" << endl;
		cout << "   3. View My Active Bookings" << endl;
		cout << "   0. Logout" << endl;
		cout << "  --------------------------------------------" << endl;
		cout << "  Choice -> ";
		cin >> choice;

		switch (choice) {
		case 1:
			cout << "Option 1";
			// displayAllBicycles(bicycles);
			break;
		case 2:
			cout << "Option ";
			// createBooking(cust.id, bicycles, bookings);
			break;
		case 3:
			cout << "Option 3";
			// viewActiveBookings(cust.id, bookings);
			break;
		case 0:
			cout << "\n  Logging out..." << endl;
			break;
		default:
			cout << "\n  [!] Invalid option. Please try again." << endl;
		}
	} while (choice != 0);
}