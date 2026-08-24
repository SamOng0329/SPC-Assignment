#include <iostream>
#include <iomanip>
#include <sstream>
#include "booking.h"

using namespace std;

// const currentUserID means FUNCTION cannot modify the string
// & = pass by reference
void bookingMenu(vector<Booking> &bookings, vector<Bicycle> &bicycles, 
    vector<Customer> &customers, const string &currentUserID, bool isStaff){
	
	int choice;
	
	do {
		cout << "\n\n  +-------------------------------------------+" << endl;
		cout << "  |       BOOKING & SCHEDULE MANAGEMENT       |" << endl;
		cout << "  +-------------------------------------------+" << endl;
		cout << "  | " << left << setw(41) << "Welcome, Placeholder" << " |" << endl;
		cout << "  | " << left << setw(41) << "Role -> Placeholder" << " |" << endl;
		cout << "  +-------------------------------------------+" << endl;
		cout << "   1. Create Booking" << endl;
		cout << "   2. View " << (isStaff ? "a Booking Record" : "My Bookings") << endl;
		cout << "   3. Modify " << (isStaff ? "a Booking" : "My Booking") << endl;
		cout << "   4. Cancel " << (isStaff ? "a Booking" : "My Booking") << endl;

		if(isStaff) {
			cout << "   5. View Full Schedule (All Customers)" << endl;
			cout << "   0. Back to Main Menu" << endl;
		} 
		else {
			cout << "   0. Back to Main Menu" << endl;
		}

		cout << "  ---------------------------------------------" << endl;
		cout << "  Choice -> ";
		cin >> choice;

		// if cin reads anything rather than int(choice), cin.fail() = true
		if(cin.fail()){
			cin.clear();
			cin.ignore(1000, '\n');	// ignore the 1000 char in cin or until the '\n'
			cout << "  Invalid input. Please enter a number." << endl << endl;
			continue;
		}

		if(choice == 1){
			createBooking(bookings, bicycles, customers, currentUserID);
		}
		else if(choice == 2){
			viewMyBookings(bookings, currentUserID);
		}
		else if(choice == 3){
			string id;
			cout << "  Enter Booking ID to Modify -> ";
			cin >> id;
			if(!modifyBooking(bookings, bicycles, id, currentUserID, isStaff)){
				cout << "  [!] Booking not found, not active, or you do not have permission to modify." << endl << endl;
			}
		}
		else if(choice == 4){
		string id;
			cout << "  Enter Booking ID to Cancel -> ";
			cin >> id;
			if(!cancelBooking(bookings, id, currentUserID, isStaff)){
				cout << "  [!] Booking not found, or you do not have permission to cancel it." << endl << endl;
			}
		}
		else if(isStaff && choice == 5){
			viewFullSchedule(bookings);
		}
		else if(choice == 0){
			cout << "  Returning to Main Menu..." << endl << endl;
		}
		else{
			cout << "Invalid choice. Please try again." << endl << endl;
		}	

	} while (choice != 0);
}


// --- Main Functions ---
bool createBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles, vector<Customer> &customers, const string &customerID){

}

// const vector<Booking> = cannot modify
bool viewMyBookings(const vector<Booking> &bookings, const string &customerID){
	bool found = false;
	cout << fixed << setprecision(2);

	// const = read-only reference, auto = auto detect data type
	for(const auto& b : bookings){
		if(isBookingOwnedByCustomer(b, customerID)){
			cout << "  " << b.bookingID << " | " << b.bicycleName
				<< " | " << b.startDate << " | " << b.startTime
				<< " | " << b.durationHours << "h" << " | RM"
				<< b.totalAmount << " | " << b.status << endl;
			found = true;
		}
		if(!found){
			cout << "  [!] You have no bookings yet." << endl << endl;
		}
	}
}

bool modifyBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles, const string &bookingID, const string &userID, bool isStaff){

}

bool cancelBooking(vector<Booking> &bookings, const string &bookingID, const string &userID, bool isStaff){

}

void viewFullSchedule(const vector<Booking> &bookings){

}

// --- Helper Functions ---
bool isDurationValid(int hours){
	return hours >= MIN_DURATION_HOURS && hours <= MAX_DURATION_HOURS;
}

bool isBookingOwnedByCustomer(const Booking &bookings, const string &customerID){
	return bookings.customerID == customerID;
}