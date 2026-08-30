#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <vector>
#include "globals.h"

// Helper Functions
string generateNextCustomerID(const vector<Customer> &customers);
string toLower(string str);
bool isUsernameTaken(const vector<CustomerLogin>& logins, const string& username);
bool isNumeric(const string &str);
bool isValidEmail(const string& email);
string formatPhoneNumber(const string& phone);

// Core Customer Functions
Customer* findCustomerById(vector<Customer>& customers, const string& customerID);
bool registerCustomer(vector<Customer>& customers, vector<CustomerLogin>& logins,
                      const string& name, const string& phone, const string& email,
                      const string& username, const string& password, bool isStaff = false);
void registerCustomerPrompt(vector<Customer>& customers, vector<CustomerLogin>& logins, bool isStaff = false);
void viewAllCustomer(const vector<Customer>& customers);
bool updateCustomerProfile(vector<Customer>& customers, const string& targetID,
                           const string& name, const string& phone, const string& email,
                           const string& tier, bool isActive);
void updateCustomerPrompt(vector<Customer>& customers, bool isStaff);
void searchCustomers(const vector<Customer>& customers, const string& query);
void searchCustomerPrompt(const vector<Customer>& customers);
bool deactivateCustomer(vector<Customer>& customers, const string& targetID);
void deactivateCustomerPrompt(vector<Customer>& customers);
void filterCustomerByTier(const vector<Customer>& customers, const string& tier);
void filterCustomerByTierPrompt(const vector<Customer>& customers);
void customerManagementMenu(vector<Customer>& customers, vector<CustomerLogin>& customerLogins, bool isStaff);

#endif 