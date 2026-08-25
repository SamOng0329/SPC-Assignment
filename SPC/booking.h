#ifndef BOOKING_H
#define BOOKING_H

#include <vector>
#include "globals.h"

using namespace std;


void bookingMenu(const string &name, const string &currentUserID, const string &userTier, bool isStaff,
	                vector<Booking> &bookings, vector<Bicycle> &bicycles, vector<Customer> &customers);

// Create
bool createBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles, 
	                vector<Customer> &customers, const string &customerID);

// Read
bool viewMyBookings(const vector<Booking> &bookings, const string &customerID);
bool viewBookingRecord(const vector<Booking> &bookings, const string &bookingID);

// Update
bool modifyBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles,
                    const string &bookingID, const string &requesterID, bool isStaff);

// Delete
bool cancelBooking(vector<Booking> &bookings, const string &bookingID,
                    const string &requesterID, bool isStaff);

// Staff-view Only
void viewFullSchedule(const vector<Booking> &bookings);

// Helper methods
bool isDurationValid(int hours);
bool isBookingOwnedByCustomer(const Booking& b, const string& customerID);
string generateBookingID(const vector<Booking>& bookings);
int findBookingIndex(const vector<Booking>& bookings, const string& bookingID);


// Checks whether 
bool isBicycleFreeForSlot(const vector<Booking>& bookings, const string& bicycleID,
                            long long slotStartMin, long long slotEndMin,
                           const string& excludeBookingID = "");

#endif