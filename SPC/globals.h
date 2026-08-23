#ifndef GLOBALS_H   // ifndef = if not defined (if not defined, then define it)
#define GLOBALS_H

#include <string>
using namespace std;


// SHARED CONSTANTS
const int MIN_DURATION_HOURS = 1;                   // booking
const int MAX_DURATION_HOURS = 72;                  // booking 

const double SST_RATE = 0.06;                       // billing
const double DAMAGE_FEE = 20.0;                     // billing


// SHARED STRUCTS

struct Staff {
    string id;              // S001
    string name;           
    string username;        
    string password;
};

// ---- Customer Management ----
struct Customer {
    string id;              // C001
    string name;
    string phone;
    string email;
    string memberType;      // Regular, Premium
    int totalRentals;
    bool isActive;
};

// ---- Bicycle Registration and Inventory Tracking ----
struct Bicycle {
    string id;              // B001
    string brand;
    string model;
    string type;            // Mountain, Road, Hybrid
    string condition;       // Good, Maintenance
    double hourlyRate;
    bool isAvailable;       // false = under maintenance/retired

};

// ---- Rental Booking and Schedule Management ----
struct Booking {
    string bookingID;        // BK001
    string customerID;
    string bicycleID;
    int durationHours;
    string customerName;     
    string bicycleName;
    string startDate;        // DD/MM/YYYY
    string startTime;        // HH:MM
    string status;           // Active, Completed, Cancelled
    string paymentMethod;    // Cash, Card, E-Wallet
    double subtotal;
    double discount;
    double tax;
    double totalAmount;
    double amountPaid;
    double change;
    bool hasDamage;
};


// ---- Billing and Payment Processing ----
struct Payment { 

};

// continue more struct if got

#endif