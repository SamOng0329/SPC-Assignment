#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <algorithm>
#include "payment.h"
#include "booking.h"
#include "customer.h"
#include "bicycle.h"

using namespace std;

string generatePaymentID(const vector<Payment> &payments) {
    int maxID = 0;
    for (const Payment &p : payments) {
        if (p.paymentID.length() > 1 && p.paymentID[0] == 'P') {
            try {
                int curr = stoi(p.paymentID.substr(1));
                if (curr > maxID) maxID = curr;
            } catch (...) {}
        }
    }
    stringstream ss;
    ss << "P" << setfill('0') << setw(3) << (maxID + 1);
    return ss.str();
}

void printReceipt(const Booking &b, const Payment &p) {
    cout << fixed << setprecision(2);
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |                     OFFICIAL RECEIPT                        |\n";
    cout << "  +---------------------+---------------------------------------+\n";
    cout << "  | Receipt No.         | " << left << setw(37) << p.paymentID << " |\n";
    cout << "  | Booking ID          | " << left << setw(37) << b.bookingID << " |\n";
    cout << "  | Customer            | " << left << setw(37) << (b.customerName + " (" + b.customerID + ")") << " |\n";
    cout << "  | Bicycle             | " << left << setw(37) << (b.bicycleName + " (" + b.bicycleID + ")") << " |\n";
    cout << "  | Rental Duration     | " << left << setw(37) << (to_string(b.durationHours) + " hour(s)") << " |\n";
    cout << "  +---------------------+---------------------------------------+\n";
    cout << "  | Subtotal            | RM " << right << setw(34) << b.subtotal << " |\n";
    cout << "  | Member Discount     | -RM" << right << setw(34) << b.discount << " |\n";
    cout << "  | SST (6%)            | RM " << right << setw(34) << b.tax << " |\n";
    if (b.hasDamage) {
        cout << "  | Damage Fee (Extra)  | RM " << right << setw(34) << DAMAGE_FEE << " |\n";
    }
    cout << "  +---------------------+---------------------------------------+\n";
    cout << "  | Final Total Due     | RM " << right << setw(34) << b.totalAmount << " |\n";
    cout << "  | Payment Method      | " << left << setw(37) << b.paymentMethod << " |\n";
    cout << "  | Amount Paid         | RM " << right << setw(34) << b.amountPaid << " |\n";
    cout << "  | Change Returned     | RM " << right << setw(34) << b.change << " |\n";
    cout << "  +---------------------+---------------------------------------+\n";
    cout << "  | Status              | " << left << setw(37) << b.status << " |\n";
    cout << "  +-------------------------------------------------------------+\n";
    cout << "  |       Thank you for renting with us! Safe riding!           |\n";
    cout << "  +-------------------------------------------------------------+\n\n";
}

bool processPayment(vector<Booking> &bookings, vector<Payment> &payments, 
                    vector<Customer> &customers, vector<Bicycle> &bicycles, 
                    const string &targetBookingID) {
    int bIndex = findBookingIndex(bookings, targetBookingID);
    if (bIndex == -1) {
        cout << "  [!] Error: Booking ID not found.\n";
        return false;
    }

    Booking &b = bookings[bIndex];
    if (b.status != "Active") {
        cout << "  [!] Error: Booking is already " << b.status << " and cannot be paid.\n";
        return false;
    }

    Customer *cust = findCustomerById(customers, b.customerID);
    Bicycle *bike = findBicycleByID(bicycles, b.bicycleID);

    
    if (cust != nullptr && cust->memberType == "Premium") {
        b.discount = b.subtotal * PREMIUM_DISCOUNT_RATE;
    } else {
        b.discount = 0.0;
    }

   
    char damageChoice;
    cout << "  Did the bicycle incur any damage during rental? (Y/N) -> ";
    cin >> damageChoice;
    if (tolower(damageChoice) == 'y') {
        b.hasDamage = true;
        if (bike != nullptr) {
            bike->isAvailable = false;
            bike->condition = "Needs Repair";
        }
    } else {
        b.hasDamage = false;
    }

    double taxableAmount = b.subtotal - b.discount;
    b.tax = taxableAmount * SST_RATE;
    b.totalAmount = taxableAmount + b.tax + (b.hasDamage ? DAMAGE_FEE : 0.0);

  
    cout << fixed << setprecision(2);
    cout << "\n  +-------------------------------------------+\n";
    cout << "  |               PAYMENT SUMMARY             |\n";
    cout << "  +-------------------------------------------+\n";
    cout << "  | Subtotal         : RM " << right << setw(18) << b.subtotal << " |\n";
    cout << "  | Discount Applied : RM " << right << setw(18) << b.discount << " |\n";
    cout << "  | SST (6%)         : RM " << right << setw(18) << b.tax << " |\n";
    if (b.hasDamage) {
        cout << "  | Damage Penalty   : RM " << right << setw(18) << DAMAGE_FEE << " |\n";
    }
    cout << "  +-------------------------------------------+\n";
    cout << "  | TOTAL PAYABLE    : RM " << right << setw(18) << b.totalAmount << " |\n";
    cout << "  +-------------------------------------------+\n";

   
    int methodChoice;
    cout << "\n  Select Payment Method:\n";
    cout << "   1. Cash\n";
    cout << "   2. Credit / Debit Card\n";
    cout << "   3. E-Wallet (TnG / DuitNow)\n";
    cout << "  Choice -> ";
    cin >> methodChoice;

    if (methodChoice == 1) {
        b.paymentMethod = "Cash";
    } else if (methodChoice == 2) {
        b.paymentMethod = "Card";
    } else if (methodChoice == 3) {
        b.paymentMethod = "E-Wallet";
    } else {
        cout << "  [!] Invalid payment method selected. Process aborted.\n";
        return false;
    }


    double paid = 0.0;
    while (true) {
        cout << "  Enter Amount Paid (RM) -> ";
        cin >> paid;

        if (cin.fail() || paid <= 0) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  [!] Invalid amount. Please enter a valid number.\n";
            continue;
        }

        if (paid < b.totalAmount) {
            cout << "  [!] Insufficient amount. Total due is RM " << b.totalAmount << ".\n";
            continue;
        }
        break;
    }

    b.amountPaid = paid;
    b.change = paid - b.totalAmount;
    b.status = "Completed";

    
    if (cust != nullptr) {
        cust->totalRentals += 1;
    }

   
    Payment payRecord;
    payRecord.paymentID = generatePaymentID(payments);
    payRecord.bookingID = b.bookingID;
    payRecord.customerID = b.customerID;
    payRecord.paymentMethod = b.paymentMethod;
    payRecord.amountPaid = b.amountPaid;
    payRecord.totalAmount = b.totalAmount;
    payRecord.change = b.change;
    payRecord.paymentDate = b.startDate; 

    payments.push_back(payRecord);

    cout << "\n  [+] Payment processed successfully! Transaction Complete.\n";
    printReceipt(b, payRecord);
    return true;
}

void viewAllPayments(const vector<Payment> &payments) {
    if (payments.empty()) {
        cout << "  [!] No payment records found.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\n  +-----------------------------------------------------------------------------------------+\n";
    cout << "  |                                TRANSACTION PAYMENT HISTORY                              |\n";
    cout << "  +-------+------------+------------+---------------+-------------+-------------+-----------+\n";
    cout << "  | PayID | Booking ID | CustomerID | Method        | Total (RM)  | Paid (RM)   | Change(RM)|\n";
    cout << "  +-------+------------+------------+---------------+-------------+-------------+-----------+\n";

    for (const auto &p : payments) {
        cout << "  | " << left << setw(5) << p.paymentID
             << " | " << left << setw(10) << p.bookingID
             << " | " << left << setw(10) << p.customerID
             << " | " << left << setw(13) << p.paymentMethod
             << " | " << right << setw(11) << p.totalAmount
             << " | " << right << setw(11) << p.amountPaid
             << " | " << right << setw(9) << p.change << " |\n";
    }
    cout << "  +-------+------------+------------+---------------+-------------+-------------+-----------+\n\n";
}

void generateRevenueReport(const vector<Booking> &bookings, const vector<Payment> &payments) {
    double totalRevenue = 0.0;
    double totalTaxCollected = 0.0;
    double totalDiscountsGiven = 0.0;
    int completedCount = 0;
    int activeCount = 0;
    int cancelledCount = 0;

    for (const auto &b : bookings) {
        if (b.status == "Completed") {
            completedCount++;
            totalRevenue += b.totalAmount;
            totalTaxCollected += b.tax;
            totalDiscountsGiven += b.discount;
        } else if (b.status == "Active") {
            activeCount++;
        } else if (b.status == "Cancelled") {
            cancelledCount++;
        }
    }

    cout << fixed << setprecision(2);
    cout << "\n  +-------------------------------------------------------------+\n";
    cout << "  |               BUSINESS REVENUE & AUDIT REPORT               |\n";
    cout << "  +-----------------------------------+-------------------------+\n";
    cout << "  | Total Completed Transactions      | " << right << setw(23) << completedCount << " |\n";
    cout << "  | Total Active Bookings (Unpaid)    | " << right << setw(23) << activeCount << " |\n";
    cout << "  | Total Cancelled Bookings          | " << right << setw(23) << cancelledCount << " |\n";
    cout << "  +-----------------------------------+-------------------------+\n";
    cout << "  | Total Member Discounts Given      | RM " << right << setw(20) << totalDiscountsGiven << " |\n";
    cout << "  | Total SST Tax Collected (6%)      | RM " << right << setw(20) << totalTaxCollected << " |\n";
    cout << "  | Total Gross Revenue               | RM " << right << setw(20) << totalRevenue << " |\n";
    cout << "  +-----------------------------------+-------------------------+\n";
    cout << "  [DONE] Financial report generated successfully.\n\n";
}

void billingMenu(vector<Booking> &bookings, vector<Payment> &payments, 
                 vector<Customer> &customers, vector<Bicycle> &bicycles, bool isStaff) {
    int choice;
    do {
        cout << "\n  +-------------------------------------------+\n";
        cout << "  |      BILLING & PAYMENT MANAGEMENT         |\n";
        cout << "  +-------------------------------------------+\n";
        cout << "   1. Process Payment & Complete Booking\n";
        cout << "   2. View Payment Records\n";
        if (isStaff) {
            cout << "   3. Generate Revenue & Statistics Report\n";
        }
        cout << "   0. Back to Menu\n";
        cout << "  ---------------------------------------------\n";
        cout << "  Choice -> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "  [!] Invalid input. Please enter a number.\n";
            continue;
        }

        if (choice == 1) {
            string bID;
            cout << "\n  Enter Booking ID to Pay (e.g., B001) -> ";
            cin >> bID;
            processPayment(bookings, payments, customers, bicycles, bID);
        } else if (choice == 2) {
            viewAllPayments(payments);
        } else if (isStaff && choice == 3) {
            generateRevenueReport(bookings, payments);
        } else if (choice == 0) {
            cout << "  Returning to previous menu...\n";
        } else {
            cout << "  [!] Invalid choice.\n";
        }
    } while (choice != 0);
}