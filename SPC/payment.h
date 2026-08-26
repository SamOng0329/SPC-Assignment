#ifndef PAYMENT_H
#define PAYMENT_H

#include <vector>
#include <string>
#include "globals.h"

using namespace std;


void billingMenu(vector<Booking> &bookings, vector<Payment> &payments, 
                 vector<Customer> &customers, vector<Bicycle> &bicycles, bool isStaff);


bool processPayment(vector<Booking> &bookings, vector<Payment> &payments, 
                    vector<Customer> &customers, vector<Bicycle> &bicycles, 
                    const string &targetBookingID);

void viewAllPayments(const vector<Payment> &payments);
void printReceipt(const Booking &b, const Payment &p);
void generateRevenueReport(const vector<Booking> &bookings, const vector<Payment> &payments);


string generatePaymentID(const vector<Payment> &payments);
int findPaymentIndexByBooking(const vector<Payment> &payments, const string &bookingID);

#endif