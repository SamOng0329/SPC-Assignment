#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <vector>
#include "globals.h"

Customer* findCustomerById(vector<Customer>& customers, const string& customerID);
void customerManagementMenu(vector<Customer>& customers, vector<CustomerLogin>& customerLogins, bool isStaff);
void registerCustomerPrompt(vector<Customer>& customers, vector<CustomerLogin>& logins);

#endif 