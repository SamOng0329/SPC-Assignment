#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include "booking.h"
#include "customer.h"
#include "bicycle.h"
#include "date_utils.h"

using namespace std;

// const currentUserID means FUNCTION cannot modify the string
// & = pass by reference
void bookingMenu(const string &name, const string &currentUserID, const string &userTier, bool isStaff,
	vector<Booking> &bookings, vector<Bicycle> &bicycles, vector<Customer> &customers){
	
	int choice;
	
	do {
		cout << "\n\n  +-------------------------------------------+" << endl;
		cout << "  |       BOOKING & SCHEDULE MANAGEMENT       |" << endl;
		cout << "  +-------------------------------------------+" << endl;
		cout << "  | " << left << setw(41) << (" Welcome, " + name) << " |" << endl;
		cout << "  | " << left << setw(41) << (" Role -> " + string(isStaff ? "Staff" : "Customer")) << " |" << endl;
		if(!isStaff){
			cout << "  | " << left << setw(41) << (" Tier -> " + userTier) << " |" << endl;
		}
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
			cout << "\n  [!] Invalid input. Please enter a number." << endl << endl;
			continue;
		}

		if(choice == 1){
			string targetCustID = currentUserID;
			if(isStaff){
				cout << "\n  Enter Customer ID for this booking (e.g. C001) -> ";
				cin >> targetCustID;
			}

			createBooking(bookings, bicycles, customers, targetCustID);
		}
		else if(choice == 2){
			if(isStaff){
				string id;
				cout << "\n  Enter Booking ID to view -> ";
				cin >> id;
				if(!viewBookingRecord(bookings, id)){
					cout << "\n  [!] Booking not found. Please try again." << endl << endl;
				}
			}
			else{
				viewMyBookings(bookings, currentUserID);
			}
		}
		else if(choice == 3){
			bool hasBookings;
			if(isStaff){
				viewFullSchedule(bookings);
				hasBookings = !bookings.empty();
			} else{
				hasBookings = viewMyBookings(bookings, currentUserID);
			}
 
			if(hasBookings){
				string id;
				cout << "\n  Enter Booking ID to Modify -> ";
				cin >> id;
				if(!modifyBooking(bookings, bicycles, id, currentUserID, isStaff)){
					cout << "  [!] Booking not found, not active, or you do not have permission to modify." << endl << endl;
				}
			} else{
				cout << "\n  [!] There is no booking to modify." << endl << endl;
			}
		}
		else if(choice == 4){
			bool hasBookings;
			if(isStaff){
				viewFullSchedule(bookings);
				hasBookings = !bookings.empty();
			} else{
				hasBookings = viewMyBookings(bookings, currentUserID);
			}
 
			if(hasBookings){
				string id;
				cout << "\n  Enter Booking ID to Cancel -> ";
				cin >> id;
				cancelBooking(bookings, id, currentUserID, isStaff);
			} else{
				cout << "\n  [!] There is no booking to cancel." << endl << endl;
			}
		}
		else if(isStaff && choice == 5){
			viewFullSchedule(bookings);
		}
		else if(choice == 0){
			cout << "\n  Returning to Main Menu..." << endl << endl;
		}
		else{
			cout << "\n  [!]Invalid choice. Please try again." << endl << endl;
		}	

	} while (choice != 0);
}


// --- Main Functions ---
bool createBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles, vector<Customer> &customers, const string &customerID){
	// pass by address (*)
	Customer *cust = findCustomerById(customers, customerID);
	if(cust == nullptr || !(*cust).isActive){
		cout << "\n  [!] Customer not found or account inactive." << endl << endl;
		return false;
	}

	string bicycleID;
	Bicycle *bicycle = nullptr; // declare as empty first

	cout << "\n  Available Bicycles:" << endl;
	viewBicycleInventory(bicycles);
	cout << endl;

	// repeat until a valid bicycleID is entered
	do{
		cout << "  Enter Bicycle ID (e.g. BK001) -> ";
		cin >> bicycleID;
		bicycle = findBicycleByID(bicycles, bicycleID);

		if(bicycle == nullptr){
			cout << "\n  [!] Bicycle ID not found. Please try again." << endl << endl;
		}
		else if(!(*bicycle).isAvailable){
			cout << "\n  [!] This bicycle is under maintenance / not in service. Please try again." << endl << endl;
		}
	} while(bicycle == nullptr || !(*bicycle).isAvailable);

	int hours;
	// repeat until duration is within 1-72 hours
	do{
		cout << "  Enter Rental Duration in hours (1-72) -> ";
		cin >> hours;
		if(!isDurationValid(hours)){
			cout << "\n  [!] Invalid duration. Must be between " << MIN_DURATION_HOURS << " and " << MAX_DURATION_HOURS << " hours" << endl << endl;
		}
	} while(!isDurationValid(hours));

	string startDate, startTime;
	long long slotStart = 0, slotEnd = 0;

	// repeat until date+time is valid AND bicycle is actually free for that slot
	// (no overlap with an existing booking)
	bool slotOk = false;
	do {
		cout << "  Enter Start Date (DD/MM/YYYY) -> ";
		cin >> startDate;
		cout << "  Enter Start Time (HH:MM, 24-hour) -> ";
		cin >> startTime;

		if(!toTotalMinutes(startDate, startTime, slotStart)){
			cout << "\n  [!] Invalid date or time. Please check the format and values." << endl << endl;
			continue;
		}

		if(slotStart < getCurrentTotalMinutes()){
			cout << "\n  [!] Cannot book a date/time that has already passed." << endl;
			cout << "  [#] Please enter a current or future date/time." << endl << endl;
			continue;
		}

		slotEnd = slotStart + static_cast<long long>(hours) * 60;

		// -> access members of an object through a pointer
		if(!isBicycleFreeForSlot(bookings, bicycle->id, slotStart, slotEnd)){
			cout << "\n  [!] This bicycle is already booked during that time window." << endl;
			cout << "  [#] Please choose a different date or time." << endl << endl;
			continue;
		}

		slotOk = true;
	} while(!slotOk);

	Booking newBooking;
	newBooking.bookingID = generateBookingID(bookings);
	newBooking.customerID = cust->id;
	newBooking.bicycleID = bicycle->id;
    newBooking.durationHours = hours;
    newBooking.customerName = cust->name;
    newBooking.bicycleName = bicycle->brand + " " + bicycle->model;
    newBooking.startDate = startDate;
    newBooking.startTime = startTime;
    newBooking.status = "Active";
    newBooking.paymentMethod = "";      // set later by billing module
    newBooking.subtotal = bicycle->hourlyRate * hours;
    newBooking.discount = 0.0;     // billing module calculates this
    newBooking.tax = newBooking.subtotal * SST_RATE;
    newBooking.totalAmount = newBooking.subtotal + newBooking.tax;
    newBooking.amountPaid = 0.0;
    newBooking.change = 0.0;
    newBooking.hasDamage = false;

	bookings.push_back(newBooking);

    cout << "\n  +-------------------------------------------------------------+" << endl;
    cout << "  |                    BOOKING CONFIRMATION                     |" << endl;
    cout << "  +---------------------+---------------------------------------+" << endl;
    cout << "  | " << left << setw(19) << "Booking ID"       << " | " << setw(37) << newBooking.bookingID << " |" << endl;
    cout << "  | " << left << setw(19) << "Customer"         << " | " << setw(37) << newBooking.customerName << " |" << endl;
    cout << "  | " << left << setw(19) << "Bicycle"          << " | " << setw(37) << (newBooking.bicycleName + " (" + newBooking.bicycleID + ")") << " |" << endl;
    cout << "  | " << left << setw(19) << "Start"            << " | " << setw(37) << (newBooking.startDate + " " + newBooking.startTime) << " |" << endl;
    cout << "  | " << left << setw(19) << "Duration"         << " | " << setw(37) << (to_string(newBooking.durationHours) + " hour(s)") << " |" << endl;
    cout << "  +---------------------+---------------------------------------+" << endl;
    cout << "  | " << left << setw(19) << "Subtotal"         << " | RM " << right << setw(34) << newBooking.subtotal << " |" << endl;
    cout << "  | " << left << setw(19) << "Tax (6%)"         << " | RM " << right << setw(34) << newBooking.tax << " |" << endl;
    cout << "  | " << left << setw(19) << "Total Due"        << " | RM " << right << setw(34) << newBooking.totalAmount << " |" << endl;
    cout << "  +---------------------+---------------------------------------+" << endl;
    cout << "  | " << left << setw(19) << "Status"           << " | " << left << setw(37) << newBooking.status << " |" << endl;
    cout << "  +-------------------------------------------------------------+" << endl;
    cout << "  | [#] Please proceed to Billing & Payment in the main menu.   |" << endl;
    cout << "  +-------------------------------------------------------------+" << endl;

	return true;
}

// const vector<Booking> = cannot modify
bool viewMyBookings(const vector<Booking> &bookings, const string &customerID){
	bool hasBookings = false;
	for(int i = 0; i < bookings.size(); i++){
		if(isBookingOwnedByCustomer(bookings[i], customerID)){
			hasBookings = true;
			break;
		}
	}

	if(!hasBookings){
		cout << "\n  [!] You have no bookings yet." << endl << endl;
		return false;
	}

	cout << fixed << setprecision(2);
    cout << "\n  +----------------------------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "  |                                                              MY BOOKINGS                                                               |" << endl;
    cout << "  +-------+--------------------+----------------------+-------------------+----------+-------------+------------+-------------+------------+" << endl;
    cout << "  | ID    | Customer           | Bicycle              | Start Time        | Duration | Subtotal    | Tax (6%)   | Total (RM)  | Status     |" << endl;
    cout << "  +-------+--------------------+----------------------+-------------------+----------+-------------+------------+-------------+------------+" << endl;

    for (const auto& b : bookings) {
		if(!isBookingOwnedByCustomer(b, customerID)) continue; // skip other customer's bookings

        string bikeDisplay = b.bicycleName.empty() ? b.bicycleID : b.bicycleName + " (" + b.bicycleID + ")";
        string startDisplay = b.startDate + " " + b.startTime;

        cout << "  | " << left  << setw(5)  << b.bookingID
             << " | " << left  << setw(18) << (b.customerName.length() > 18 ? b.customerName.substr(0, 15) + "..." : b.customerName)
             << " | " << left  << setw(20) << (bikeDisplay.length() > 20 ? bikeDisplay.substr(0, 17) + "..." : bikeDisplay)
             << " | " << left  << setw(17) << startDisplay
             << " | " << right << setw(7)  << b.durationHours << "h"
             << " | RM" << right << setw(9)  << b.subtotal
             << " | RM" << right << setw(8)  << b.tax
             << " | RM" << right << setw(9)  << b.totalAmount
             << " | " << left  << setw(10) << b.status << " |" << endl;
    }

    cout << "  +-------+--------------------+----------------------+-------------------+----------+-------------+------------+-------------+------------+" << endl << endl;
    return true;
}

bool viewBookingRecord(const vector<Booking> &bookings, const string &bookingID){
	int index = findBookingIndex(bookings, bookingID);
	if(index == -1) return false;

	// read-only
	const Booking &b = bookings[index];

	cout << fixed << setprecision(2);

	cout << "\n  +-------------------------------------------------------------+\n";
	cout << "  |                      BOOKING RECORD                         |\n";
	cout << "  +---------------------+---------------------------------------+\n";
	cout << "  | " << left << setw(19) << "Booking ID"     << " | " << setw(37) << b.bookingID << " |\n";
	cout << "  | " << left << setw(19) << "Customer"       << " | " << setw(37) << (b.customerName + " (" + b.customerID + ")") << " |\n";
	cout << "  | " << left << setw(19) << "Bicycle"        << " | " << setw(37) << (b.bicycleName + " (" + b.bicycleID + ")") << " |\n";
	cout << "  | " << left << setw(19) << "Start"          << " | " << setw(37) << (b.startDate + " " + b.startTime) << " |\n";
	cout << "  | " << left << setw(19) << "Duration"       << " | " << setw(37) << (to_string(b.durationHours) + " hour(s)") << " |\n";
	cout << "  +---------------------+---------------------------------------+\n";
	cout << "  | " << left << setw(19) << "Subtotal"       << " | RM " << right << setw(34) << b.subtotal << " |\n";
	cout << "  | " << left << setw(19) << "Discount"       << " | RM " << right << setw(34) << b.discount << " |\n";
	cout << "  | " << left << setw(19) << "Tax (6%)"       << " | RM " << right << setw(34) << b.tax << " |\n";
	cout << "  | " << left << setw(19) << "Total Amount"   << " | RM " << right << setw(34) << b.totalAmount << " |\n";
	cout << "  | " << left << setw(19) << "Amount Paid"    << " | RM " << right << setw(34) << b.amountPaid << " |\n";
	cout << "  | " << left << setw(19) << "Change"         << " | RM " << right << setw(34) << b.change << " |\n";
	cout << "  +---------------------+---------------------------------------+\n";
	cout << "  | " << left << setw(19) << "Payment Method" << " | " << setw(37) << (b.paymentMethod.empty() ? "Not Paid Yet" : b.paymentMethod) << " |\n";
	cout << "  | " << left << setw(19) << "Has Damage"     << " | " << setw(37) << (b.hasDamage ? "Yes" : "No") << " |\n";
	cout << "  | " << left << setw(19) << "Status"         << " | " << setw(37) << b.status << " |\n";
	cout << "  +-------------------------------------------------------------+\n\n";

	return true;
}

bool modifyBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles, const string &bookingID, const string &requesterID, bool isStaff){
	int index = findBookingIndex(bookings, bookingID);
	// -1 means false (value return in findBookingIndex func)
	if(index == -1){
		return false;
	}

	Booking &b = bookings[index];

	if(b.status != "Active") return false; // can't modify a cancelled/completed booking
	if(!isStaff && !isBookingOwnedByCustomer(b, requesterID)) return false;

	Bicycle *bicycle = findBicycleByID(bicycles, b.bicycleID);
	if(bicycle == nullptr) return false;

	cout << "\n  Current Booking: " << b.startDate << " " << b.startTime << ", " << b.durationHours << " hour(s)" << endl;

	int hours;
	do{
		cout << "  Enter NEW Rental Duration in hours (1-72) -> ";
		cin >> hours;
		if(!isDurationValid(hours)){
			cout << "\n  [!] Invalid duration. Must be between " << MIN_DURATION_HOURS << " and " << MAX_DURATION_HOURS << " hours" << endl << endl;
		}
	} while(!isDurationValid(hours));

	string newDate, newTime;
	long long slotStart = 0, slotEnd = 0;
	bool slotOk = false;

	do{
		cout << "  Enter NEW Start Date (DD/MM/YYYY) -> ";
		cin >> newDate;
		cout << "  Enter NEW Start Time (HH:MM, 24-hour) -> ";
		cin >> newTime;

		if(!toTotalMinutes(newDate, newTime, slotStart)){
			cout << "\n  [!] Invalid date or time. Please check the format and values." << endl << endl;
			continue;
		}

		if(slotStart < getCurrentTotalMinutes()){
			cout << "\n  [!] Cannot move a booking to a date/time that has already passed." << endl;
			cout << "  [#] Please enter a current or future date/time." << endl << endl;
			continue;
		}

		slotEnd = slotStart + static_cast<long long>(hours) * 60;

		// exclude THIS booking's own existing slot from the overlap check to prevent checking itself
		if(!isBicycleFreeForSlot(bookings, b.bicycleID, slotStart, slotEnd, b.bookingID)){
			cout << "\n  [!] This bicycle is already booked during that time window." << endl;
			cout << "  [#] Please choose a different date/time." << endl << endl;
			continue;
		}

		slotOk = true;
	} while(!slotOk);

	// Update the details
	b.durationHours = hours;
	b.startDate = newDate;
	b.startTime = newTime;
	b.subtotal = bicycle->hourlyRate * hours;
	b.tax = b.subtotal * SST_RATE;
	b.totalAmount = b.subtotal + b.tax - b.discount;

	cout << fixed << setprecision(2);
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                       BOOKING UPDATED                       |\n";
    cout << "  +---------------------+---------------------------------------+\n";
    cout << "  | " << left << setw(19) << "Booking ID"       << " | " << setw(37) << b.bookingID << " |\n";
    cout << "  | " << left << setw(19) << "New Start"        << " | " << setw(37) << (b.startDate + " " + b.startTime) << " |\n";
    cout << "  | " << left << setw(19) << "Duration"         << " | " << setw(37) << (to_string(b.durationHours) + " hour(s)") << " |\n";
    cout << "  +---------------------+---------------------------------------+\n";
    cout << "  | " << left << setw(19) << "New Total"        << " | RM " << right << setw(34) << b.totalAmount << " |\n";
    cout << "  +-------------------------------------------------------------+\n";
    cout << "  | [DONE] Schedule Updated Successfully.                       |\n";
    cout << "  +-------------------------------------------------------------+\n\n";

	return true;
}

bool cancelBooking(vector<Booking> &bookings, const string &bookingID, const string &requesterID, bool isStaff){
	int index = findBookingIndex(bookings, bookingID);
	if(index == -1){
		cout << "\n  [!] Booking ID not found. Please try again." << endl << endl;
		return false;
	}

	if(!isStaff && !isBookingOwnedByCustomer(bookings[index], requesterID)){
		cout << "\n  [!] You do not have permission to cancel this booking." << endl << endl;
		return false;	// customers cannot cancel someone else's booking
	}


	// Booking is kept in the vector even after cancelling (soft delete)
	// it's intentional, so cancelled bookings still show up in statistics report
	// so need also need to check the cancel status
	if(bookings[index].status == "Cancelled"){
		cout << "\n  [!] This booking is already cancelled." << endl << endl;
		return false;
	}
	if(bookings[index].status == "Completed"){
		cout << "\n  [!] This booking is already completed and cannot be cancelled." << endl << endl;
		return false;
	}

	bookings[index].status = "Cancelled";
	cout << "\n  [DONE] Booking " << bookingID << " has been cancelled." << endl << endl;
	return true;
}

void viewFullSchedule(const vector<Booking> &bookings){
	if(bookings.empty()){
		cout << "\n  [!] No bookings found." << endl << endl;
		return;
	}

	cout << fixed << setprecision(2);
    cout << "\n  +---------------------------------------------------------------------------------------------------------------------------------------+" << endl;
    cout << "  |                                                          BOOKING SCHEDULE                                                             |" << endl;
    cout << "  +-------+--------------------+----------------------+-------------------+----------+-------------+------------+-------------+-----------+" << endl;
    cout << "  | ID    | Customer           | Bicycle              | Start Time        | Duration | Subtotal    | Tax (6%)   | Total (RM)  | Status    |" << endl;
    cout << "  +-------+--------------------+----------------------+-------------------+----------+-------------+------------+-------------+-----------+" << endl;

	for (const auto& b : bookings) {
        string bikeDisplay = b.bicycleName.empty() ? b.bicycleID : b.bicycleName + " (" + b.bicycleID + ")";
        string startDisplay = b.startDate + " " + b.startTime;

        cout << "  | " << left  << setw(5)  << b.bookingID
             << " | " << left  << setw(18) << (b.customerName.length() > 18 ? b.customerName.substr(0, 15) + "..." : b.customerName)
             << " | " << left  << setw(20) << (bikeDisplay.length() > 20 ? bikeDisplay.substr(0, 17) + "..." : bikeDisplay)
             << " | " << left  << setw(17) << startDisplay
             << " | " << right << setw(7)  << b.durationHours << "h"
             << " | RM" << right << setw(9)  << b.subtotal
             << " | RM" << right << setw(8)  << b.tax
             << " | RM" << right << setw(9)  << b.totalAmount
             << " | " << left  << setw(9)  << b.status << " |" << endl;
    }

    cout << "  +-------+--------------------+----------------------+-------------------+----------+-------------+------------+-------------+-----------+" << endl << endl;
}

// --- Helper Functions ---
bool isDurationValid(int hours){
	return hours >= MIN_DURATION_HOURS && hours <= MAX_DURATION_HOURS;
}

bool isBookingOwnedByCustomer(const Booking &bookings, const string &customerID){
	return bookings.customerID == customerID;
}

string generateBookingID(const vector<Booking> &bookings){
	int nextNum = static_cast<int>(bookings.size()) + 1;
	ostringstream oss; 	// perform output operations on string-based streams
	oss << "B" << setw(3) << setfill('0') << nextNum;
	return oss.str();
}

int findBookingIndex(const vector<Booking> &bookings, const string &bookingID){
	for(size_t i = 0; i < bookings.size(); i++){
		if(bookings[i].bookingID == bookingID){
			return static_cast<int>(i);
		}
	}
	return -1;	// -1 means false
}


// slotStartMin / slotEndMin = Requested rental start and end times in total minutes
// excludeBookingID = if editing an existing booking, skip its own ID so it doesn't clash with itself
bool isBicycleFreeForSlot(const vector<Booking> &bookings, const string &bicycleID, long long slotStartMin, long long slotEndMin, const string &excludeBookingID){
	// const = read-only reference, auto = auto detect data type
	for(const auto & b : bookings){
		// skip bookings for other bicycles
		if(b.bicycleID != bicycleID) continue;
		// skip completed or cancelled bookings (cuz only "Active" bookings can block time slots)
		if(b.status != "Active") continue;
		// if modifying existing reservation, ignore this bookingID so it dont block itself
		if(!excludeBookingID.empty() && b.bookingID == excludeBookingID) continue;

		// Convert existing booking's start date and time into total minutes
		long long existingStart, existingEnd;
		if(!toTotalMinutes(b.startDate, b.startTime, existingStart)){
			continue; // skip corrupted records
		}
		// Calculate when the existing booking ends
		existingEnd = existingStart + static_cast<long long>(b.durationHours) * 60;

		// Check for time overlap between requested slot and existing booking
		bool overlaps = (slotStartMin < existingEnd) && (existingStart < slotEndMin);
		if(overlaps) return false;
	}
	return true;
}