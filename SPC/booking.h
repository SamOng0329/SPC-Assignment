#ifndef BOOKING_H
#define BOOKING_H

#include <vector>
#include "globals.h"

using namespace std;


void bookingMenu(vector<Booking> &bookings, vector<Bicycle> &bicycles, 
                    vector<Customer> &customers, const string &currentUserID, bool isStaff);

// Create
bool createBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles, 
	                vector<Customer> &customers, const string &customerID);

// Read
bool viewMyBookings(const vector<Booking> &bookings, const string &customerID);

// Update
bool modifyBooking(vector<Booking> &bookings, vector<Bicycle> &bicycles,
                    const string &bookingID, const string &userID, bool isStaff);

// Delete
bool cancelBooking(vector<Booking> &bookings, const string &bookingID,
                    const string &userID, bool isStaff);

// Staff-view Only
void viewFullSchedulles(const vector<Booking> &bookings);

#endif